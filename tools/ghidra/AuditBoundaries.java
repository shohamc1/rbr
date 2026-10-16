// Read-only boundary evidence. No bytes, assembly, or pseudocode leave build/.
import ghidra.app.util.headless.HeadlessScript;
import ghidra.framework.Application;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.app.decompiler.*;
import com.google.gson.*;
import java.nio.file.*;
import java.util.*;

public class AuditBoundaries extends HeadlessScript {
    private final List<Map<String, Object>> issues = new ArrayList<>();
    private void issue(String kind, Address from, Address to, Function owner) {
        Map<String, Object> row = new LinkedHashMap<>();
        row.put("kind", kind);
        row.put("from", from.toString());
        if (to != null) row.put("to", to.toString());
        if (to != null) {
            CodeUnit unit = currentProgram.getListing().getCodeUnitContaining(to);
            if (unit != null) row.put("target_code_unit", unit.toString());
        }
        if (owner != null) row.put("function", owner.getEntryPoint().toString());
        issues.add(row);
    }
    private List<Map<String, Object>> ranges(AddressSetView set) {
        List<Map<String, Object>> result = new ArrayList<>();
        for (AddressRange r : set.getAddressRanges()) {
            Map<String, Object> row = new LinkedHashMap<>();
            row.put("start", r.getMinAddress().toString());
            row.put("end_inclusive", r.getMaxAddress().toString());
            row.put("size", r.getLength());
            result.add(row);
        }
        return result;
    }
    public void run() throws Exception {
        if (analysisTimeoutOccurred()) throw new IllegalStateException("Analysis timed out");
        String[] args = getScriptArgs();
        if (!currentProgram.getExecutableSHA256().equalsIgnoreCase(args[1]))
            throw new IllegalStateException("Reference hash mismatch");
        var fm = currentProgram.getFunctionManager();
        var listing = currentProgram.getListing();
        var refs = currentProgram.getReferenceManager();
        AddressSet executable = new AddressSet();
        for (var block : currentProgram.getMemory().getBlocks())
            if (block.isExecute() && block.isInitialized()) executable.add(block.getStart(), block.getEnd());
        AddressSet bodies = new AddressSet();
        List<Map<String, Object>> functions = new ArrayList<>();
        for (Function f : fm.getFunctions(true)) {
            monitor.checkCancelled();
            Address entry = f.getEntryPoint();
            Map<String, Object> row = new LinkedHashMap<>();
            row.put("address", entry.toString());
            row.put("name", f.getName());
            row.put("size", f.getBody().getNumAddresses());
            row.put("thunk", f.isThunk());
            row.put("no_return", f.hasNoReturn());
            row.put("ranges", ranges(f.getBody()));
            row.put("overlap_bytes", bodies.intersect(f.getBody()).getNumAddresses());
            int calls = 0, jumps = 0, pointers = 0;
            for (Reference r : refs.getReferencesTo(entry)) {
                if (r.getReferenceType().isCall()) calls++;
                else if (r.getReferenceType().isJump()) jumps++;
                else if (r.getReferenceType().isData()) pointers++;
            }
            row.put("incoming_calls", calls);
            row.put("incoming_jumps", jumps);
            row.put("incoming_data", pointers);
            row.put("entry_instruction", listing.getInstructionAt(entry) != null);
            row.put("outside_executable_bytes", f.getBody().subtract(executable).getNumAddresses());
            bodies.add(f.getBody());
            functions.add(row);
        }
        AddressSet instructions = new AddressSet();
        AddressSet unowned = new AddressSet();
        List<String> returnSuccessors = new ArrayList<>();
        List<Map<String, Object>> computedCalls = new ArrayList<>();
        for (Instruction i : listing.getInstructions(executable, true)) {
            monitor.checkCancelled();
            Address pc = i.getAddress();
            instructions.add(i.getMinAddress(), i.getMaxAddress());
            Function owner = fm.getFunctionContaining(pc);
            if (owner == null) unowned.add(i.getMinAddress(), i.getMaxAddress());
            var flow = i.getFlowType();
            Address next = i.getMaxAddress().add(1);
            if (i.getMnemonicString().equals("RET") && (next.getOffset() & 15) == 0 &&
                    executable.contains(next) && fm.getFunctionContaining(next) == null)
                returnSuccessors.add(next.toString());
            if (flow.isComputed() && flow.isCall()) {
                Map<String, Object> call = new LinkedHashMap<>();
                call.put("address", pc.toString());
                if (owner != null) call.put("function", owner.getEntryPoint().toString());
                call.put("instruction", i.toString());
                List<String> targets = new ArrayList<>();
                for (Address target : i.getFlows()) targets.add(target.toString());
                call.put("resolved_flow_targets", targets);
                computedCalls.add(call);
            }
            if (flow.isComputed() && flow.isJump() && i.getFlows().length == 0)
                issue("unresolved_indirect_jump", pc, null, owner);
            for (Address target : i.getFlows()) {
                if (!executable.contains(target)) continue;
                Function dest = fm.getFunctionContaining(target);
                if (listing.getInstructionAt(target) == null)
                    issue("flow_to_noninstruction", pc, target, owner);
                if (flow.isCall() && fm.getFunctionAt(target) == null)
                    issue(dest == null ? "call_to_unowned" : "call_to_function_interior", pc, target, owner);
                if (flow.isJump() && owner != null && !owner.getBody().contains(target))
                    issue(dest == null ? "jump_to_unowned" : "cross_function_jump", pc, target, owner);
            }
            Address fall = i.getFallThrough();
            if (fall != null && executable.contains(fall) && owner != null && !owner.getBody().contains(fall))
                issue("fallthrough_outside_body", pc, fall, owner);
        }
        AddressSet data = new AddressSet();
        List<Map<String, Object>> dllStrings = new ArrayList<>();
        AddressSet strings = new AddressSet();
        Set<Address> loaderCallers = new TreeSet<>();
        for (Data d : listing.getDefinedData(true)) {
            if (executable.contains(d.getAddress())) data.add(d.getMinAddress(), d.getMaxAddress());
            if (!d.hasStringValue()) continue;
            strings.add(d.getMinAddress(), d.getMaxAddress());
            String value = String.valueOf(d.getValue());
            if (!value.toLowerCase(Locale.ROOT).contains(".dll")) continue;
            Map<String, Object> row = new LinkedHashMap<>();
            row.put("address", d.getAddress().toString());
            row.put("value", value);
            List<Map<String, String>> sites = new ArrayList<>();
            for (Reference r : refs.getReferencesTo(d.getAddress())) {
                Map<String, String> site = new LinkedHashMap<>();
                site.put("from", r.getFromAddress().toString());
                site.put("type", r.getReferenceType().toString());
                Function caller = fm.getFunctionContaining(r.getFromAddress());
                if (caller != null) {
                    site.put("function", caller.getEntryPoint().toString());
                    loaderCallers.add(caller.getEntryPoint());
                }
                sites.add(site);
            }
            row.put("references", sites);
            dllStrings.add(row);
        }
        List<Map<String, Object>> loaders = new ArrayList<>();
        for (Symbol s : currentProgram.getSymbolTable().getAllSymbols(true)) {
            String name = s.getName();
            if (!(name.contains("LoadLibrary") || name.contains("GetProcAddress") || name.contains("CoCreateInstance"))) continue;
            for (Reference r : refs.getReferencesTo(s.getAddress())) {
                Function caller = fm.getFunctionContaining(r.getFromAddress());
                Map<String, Object> row = new LinkedHashMap<>();
                row.put("symbol", name);
                row.put("from", r.getFromAddress().toString());
                if (caller != null) row.put("function", caller.getEntryPoint().toString());
                loaders.add(row);
                if (caller != null) loaderCallers.add(caller.getEntryPoint());
            }
        }
        List<Map<String, Object>> decompilations = new ArrayList<>();
        DecompInterface decompiler = new DecompInterface();
        if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
        try {
            for (Address entry : loaderCallers) {
                Function f = fm.getFunctionAt(entry);
                DecompileResults result = decompiler.decompileFunction(f, 30, monitor);
                Map<String, Object> row = new LinkedHashMap<>();
                row.put("address", entry.toString());
                row.put("completed", result.decompileCompleted());
                row.put("message", result.getErrorMessage());
                if (result.decompileCompleted()) row.put("pseudocode", result.getDecompiledFunction().getC());
                decompilations.add(row);
            }
        } finally { decompiler.dispose(); }
        List<Map<String, Object>> gaps = ranges(executable.subtract(bodies).subtract(data).subtract(instructions));
        for (Map<String, Object> gap : gaps) {
            Address start = toAddr((String)gap.get("start"));
            long size = ((Number)gap.get("size")).longValue();
            boolean padding = true, zero = true;
            for (long j = 0; j < size; j++) {
                int b = currentProgram.getMemory().getByte(start.add(j)) & 255;
                padding &= b == 0xcc || b == 0x90;
                zero &= b == 0;
            }
            gap.put("kind", padding ? "int3_nop_padding_candidate" : zero ? "zero_fill_candidate" : "unresolved");
        }
        Map<String, Object> report = new LinkedHashMap<>();
        report.put("sha256", currentProgram.getExecutableSHA256());
        report.put("ghidra_version", Application.getApplicationVersion());
        report.put("executable_bytes", executable.getNumAddresses());
        report.put("function_body_union_bytes", bodies.intersect(executable).getNumAddresses());
        report.put("instruction_bytes", instructions.getNumAddresses());
        report.put("data_outside_bodies_bytes", data.subtract(bodies).getNumAddresses());
        report.put("unowned_instruction_ranges", ranges(unowned));
        report.put("undefined_ranges", gaps);
        report.put("functions", functions);
        report.put("aligned_return_successors", returnSuccessors);
        report.put("computed_call_sites", computedCalls);
        report.put("issues", issues);
        report.put("dll_strings", dllStrings);
        report.put("string_ranges", ranges(strings));
        report.put("loader_sites", loaders);
        report.put("loader_functions", decompilations);
        List<Map<String, Object>> externals = new ArrayList<>();
        for (Function f : fm.getExternalFunctions()) {
            Map<String, Object> row = new LinkedHashMap<>();
            row.put("name", f.getName(true));
            row.put("no_return", f.hasNoReturn());
            var location = currentProgram.getExternalManager().getExternalLocation(f.getSymbol());
            if (location != null) row.put("original_import", location.getOriginalImportedName());
            externals.add(row);
        }
        report.put("external_functions", externals);
        Map<String, String> options = new TreeMap<>();
        var analysis = currentProgram.getOptions(Program.ANALYSIS_PROPERTIES);
        for (String name : analysis.getOptionNames()) options.put(name, String.valueOf(analysis.getObject(name, null)));
        report.put("analysis_options", options);
        Files.writeString(Path.of(args[0]), new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
    }
}
