// Refine only the separate census project. Never alter matching manifests.
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.nio.file.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.*;
import com.google.gson.*;

public class RefineBoundaries extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        JsonObject evidence = JsonParser.parseString(Files.readString(Path.of(args[0]))).getAsJsonObject();
        if (!currentProgram.getExecutableSHA256().equalsIgnoreCase(evidence.get("sha256").getAsString()))
            throw new IllegalStateException("Discovery reference mismatch");
        var fm = currentProgram.getFunctionManager();
        if (evidence.has("jump_overrides")) {
            for (var item : evidence.getAsJsonArray("jump_overrides")) {
                JsonObject d = item.getAsJsonObject();
                Address from = toAddr(d.get("from").getAsString());
                Address target = toAddr(d.get("target").getAsString());
                Instruction i = getInstructionAt(from);
                if (i == null) throw new IllegalStateException("Missing reviewed branch");
                byte[] raw = i.getBytes();
                if (raw.length != 5 || (raw[0] & 255) != 0xe9 ||
                        !from.add(5L + ByteBuffer.wrap(raw, 1, 4).order(ByteOrder.LITTLE_ENDIAN).getInt()).equals(target))
                    throw new IllegalStateException("Reviewed direct JMP bytes disagree");
                i.setFlowOverride(FlowOverride.NONE);
            }
        }
        List<String> merged = new ArrayList<>();
        if (evidence.has("fragment_merges")) {
            for (var item : evidence.getAsJsonArray("fragment_merges")) {
                JsonObject d = item.getAsJsonObject();
                Function owner = fm.getFunctionAt(toAddr(d.get("owner").getAsString()));
                Function fragment = fm.getFunctionAt(toAddr(d.get("fragment").getAsString()));
                if (owner == null) throw new IllegalStateException("Missing reviewed owner");
                if (fragment == null) continue;
                for (Reference r : currentProgram.getReferenceManager().getReferencesTo(fragment.getEntryPoint()))
                    if ((r.getReferenceType().isCall() || r.getReferenceType().isJump() || r.getReferenceType().isData()) &&
                            !(r.getReferenceType().isJump() && owner.getBody().contains(r.getFromAddress())))
                        throw new IllegalStateException("Reviewed fragment acquired an independent reference");
                AddressSet body = new AddressSet(owner.getBody());
                body.add(fragment.getBody());
                merged.add(fragment.getEntryPoint().toString());
                fm.removeFunction(fragment.getEntryPoint());
                owner.setBody(body);
            }
        }
        List<String> codeCorrections = new ArrayList<>();
        List<String> corrected = new ArrayList<>();
        if (evidence.has("returning_imports")) {
            for (Function f : fm.getExternalFunctions()) {
                var location = currentProgram.getExternalManager().getExternalLocation(f.getSymbol());
                if (location == null) continue;
                for (var decision : evidence.getAsJsonArray("returning_imports")) {
                    JsonObject d = decision.getAsJsonObject();
                    if (!location.getLibraryName().equalsIgnoreCase(d.get("library").getAsString()) ||
                        !d.get("symbol").getAsString().equals(location.getOriginalImportedName())) continue;
                    f.setNoReturn(false);
                    f.setSignatureSource(SourceType.USER_DEFINED);
                    corrected.add(f.getName(true));
                    Set<Address> targets = new HashSet<>();
                    targets.add(f.getEntryPoint());
                    for (Function local : fm.getFunctions(true)) {
                        if (local.isThunk() && f.equals(local.getThunkedFunction(true))) {
                            local.setNoReturn(false);
                            targets.add(local.getEntryPoint());
                        }
                    }
                    for (Address target : targets) {
                        for (Reference r : currentProgram.getReferenceManager().getReferencesTo(target)) {
                            Instruction i = getInstructionAt(r.getFromAddress());
                            if (i != null && i.getFlowOverride() == FlowOverride.CALL_RETURN) {
                                i.setFlowOverride(FlowOverride.NONE);
                                Address next = i.getFallThrough();
                                if (next != null) disassemble(next);
                            }
                        }
                    }
                }
            }
        }
        List<String> rejected = new ArrayList<>();
        int created = 0, fixed = 0;
        Set<Address> newEntries = new HashSet<>();
        Set<String> seeds = new TreeSet<>();
        if (evidence.has("reviewed_entries"))
            for (var item : evidence.getAsJsonArray("reviewed_entries"))
                seeds.add(item.getAsJsonObject().get("address").getAsString());
        for (var item : evidence.getAsJsonArray("vtable_seeds")) seeds.add(item.getAsString());
        if (evidence.has("padded_leaf_seeds"))
            for (var item : evidence.getAsJsonArray("padded_leaf_seeds"))
                seeds.add(item.getAsJsonObject().get("address").getAsString());
        Set<String> explicitSeeds = new HashSet<>(seeds);
        if (evidence.has("bounded_cfg_seeds"))
            for (var item : evidence.getAsJsonArray("bounded_cfg_seeds"))
                seeds.add(item.getAsJsonObject().get("address").getAsString());
        for (String item : seeds) {
            monitor.checkCancelled();
            Address entry = toAddr(item);
            if (fm.getFunctionAt(entry) != null) continue;
            if (fm.getFunctionContaining(entry) != null ||
                (getInstructionContaining(entry) != null && getInstructionAt(entry) == null) ||
                currentProgram.getListing().getDefinedDataContaining(entry) != null) {
                rejected.add(entry.toString());
                continue;
            }
            disassemble(entry);
            if (getInstructionAt(entry) != null &&
                new CreateFunctionCmd(entry, false).applyTo(currentProgram, monitor)) {
                created++;
                newEntries.add(entry);
            }
            else rejected.add(entry.toString());
        }
        analyzeAll(currentProgram);
        if (evidence.has("code_data_corrections")) {
            for (var item : evidence.getAsJsonArray("code_data_corrections")) {
                JsonObject d = item.getAsJsonObject();
                Address entry = toAddr(d.get("address").getAsString());
                Instruction call = getInstructionAt(toAddr(d.get("call_site").getAsString()));
                if (call == null || !call.getFlowType().isCall() || !Arrays.asList(call.getFlows()).contains(entry))
                    throw new IllegalStateException("Reviewed call target not decoded as expected");
                Data data = currentProgram.getListing().getDefinedDataContaining(entry);
                if (data != null) {
                    clearListing(data.getMinAddress(), data.getMaxAddress());
                    disassemble(entry);
                    if (!new CreateFunctionCmd(entry, false).applyTo(currentProgram, monitor))
                        throw new IllegalStateException("Could not create reviewed call target");
                    codeCorrections.add(entry.toString());
                }
            }
        }
        List<String> repairedFlows = new ArrayList<>();
        if (evidence.has("flow_repairs")) {
            for (var item : evidence.getAsJsonArray("flow_repairs")) {
                JsonObject d = item.getAsJsonObject();
                Address target = toAddr(d.get("target").getAsString());
                Address end = toAddr(d.get("clear_end").getAsString());
                Instruction source = getInstructionAt(toAddr(d.get("from").getAsString()));
                if (source == null || !(target.equals(source.getFallThrough()) ||
                        Arrays.asList(source.getFlows()).contains(target)))
                    throw new IllegalStateException("Reviewed flow no longer reaches target");
                if (getInstructionAt(target) != null) continue;
                AddressSet span = new AddressSet(target, end);
                if (fm.getFunctionsOverlapping(span).hasNext())
                    throw new IllegalStateException("Flow repair would clear an existing body");
                clearListing(target, end);
                disassemble(target);
                if (getInstructionAt(target) == null)
                    throw new IllegalStateException("Reviewed flow still has no instruction");
                repairedFlows.add(target.toString());
            }
        }
        // Analysis can leave a body stale when later passes decode more instructions.
        for (Function f : fm.getFunctions(true)) {
            monitor.checkCancelled();
            if (CreateFunctionCmd.fixupFunctionBody(currentProgram, f, monitor)) fixed++;
        }
        analyzeAll(currentProgram);
        if (evidence.has("bounded_cfg_seeds")) {
            for (var item : evidence.getAsJsonArray("bounded_cfg_seeds")) {
                JsonObject d = item.getAsJsonObject();
                Address entry = toAddr(d.get("address").getAsString());
                if (!newEntries.contains(entry) || explicitSeeds.contains(entry.toString())) continue;
                Function f = fm.getFunctionAt(entry);
                AddressSet expected = new AddressSet(entry, entry.add(d.get("size").getAsLong() - 1));
                if (f == null || !f.getBody().equals(expected)) {
                    if (f != null) fm.removeFunction(entry);
                    rejected.add(entry.toString() + ": Ghidra body differs from bounded CFG");
                    created--;
                }
            }
        }
        Map<String, Object> result = new LinkedHashMap<>();
        result.put("sha256", currentProgram.getExecutableSHA256());
        result.put("created_from_evidence_seeds", created);
        result.put("body_fixups", fixed);
        result.put("corrected_returning_imports", corrected);
        result.put("merged_fragments", merged);
        result.put("corrected_code_data_targets", codeCorrections);
        result.put("repaired_flow_targets", repairedFlows);
        result.put("rejected_seeds", rejected);
        Files.writeString(Path.of(args[1]), new GsonBuilder().setPrettyPrinting().create().toJson(result) + "\n");
    }
}
