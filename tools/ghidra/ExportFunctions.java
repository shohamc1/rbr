// Export analysis hypotheses, never matching claims. Output stays in build/.
import ghidra.app.util.headless.HeadlessScript;
import ghidra.app.decompiler.*;
import ghidra.framework.Application;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import com.google.gson.*;
import java.nio.file.*;
import java.util.*;

public class ExportFunctions extends HeadlessScript {
    private Map<String, Object> summary(Function f) {
        Map<String, Object> row = new LinkedHashMap<>();
        row.put("address", f.getEntryPoint().toString());
        row.put("name", f.getName());
        row.put("size", f.getBody().getNumAddresses());
        row.put("prototype", f.getPrototypeString(false, false));
        row.put("calling_convention", f.getCallingConventionName());
        row.put("thunk", f.isThunk());
        row.put("external", f.isExternal());
        List<String> ranges = new ArrayList<>();
        for (AddressRange range : f.getBody().getAddressRanges()) ranges.add(range.toString());
        row.put("body_ranges", ranges);
        row.put("confidence", "Analysis hypothesis; review boundaries and types against instructions and callers");
        return row;
    }
    private Map<String, String> reference(Reference r) {
        Map<String, String> row = new LinkedHashMap<>();
        row.put("from", r.getFromAddress().toString());
        row.put("to", r.getToAddress().toString());
        row.put("type", r.getReferenceType().toString());
        Function caller = getFunctionContaining(r.getFromAddress());
        if (caller != null) row.put("source_function", caller.getEntryPoint().toString());
        Symbol symbol = getSymbolAt(r.getToAddress());
        if (symbol != null) row.put("target_symbol", symbol.getName(true));
        Data data = getDataAt(r.getToAddress());
        if (data != null) row.put("target_data", data.toString());
        return row;
    }
    private void write(Path path, Object value) throws Exception {
        Files.writeString(path, new GsonBuilder().setPrettyPrinting().create().toJson(value) + "\n");
    }
    public void run() throws Exception {
        if (analysisTimeoutOccurred()) throw new IllegalStateException("Analysis timed out");
        String[] args = getScriptArgs();
        Path out = Path.of(args[0]);
        Files.createDirectories(out);
        String sha = currentProgram.getExecutableSHA256();
        if (!args[1].equalsIgnoreCase(sha)) throw new IllegalStateException("Project reference hash mismatch");
        Map<String, Object> metadata = new LinkedHashMap<>();
        metadata.put("reference_sha256", sha);
        metadata.put("ghidra_version", Application.getApplicationVersion());
        metadata.put("language", currentProgram.getLanguageID().toString());
        metadata.put("compiler_spec", currentProgram.getCompilerSpec().getCompilerSpecID().toString());
        Map<String, String> options = new TreeMap<>();
        var analysis = currentProgram.getOptions(Program.ANALYSIS_PROPERTIES);
        for (String name : analysis.getOptionNames()) options.put(name, String.valueOf(analysis.getObject(name, null)));
        metadata.put("analysis_options", options);
        List<Map<String, Object>> index = new ArrayList<>();
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) index.add(summary(f));
        write(out.resolve("functions.json"), index);
        DecompInterface decompiler = new DecompInterface();
        if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
        try {
            for (int i = 2; i < args.length; i++) {
                Address address = toAddr(args[i]);
                Function f = getFunctionAt(address);
                if (f == null) throw new IllegalArgumentException("No function entry at " + address);
                Map<String, Object> row = summary(f);
                row.put("reference_sha256", sha);
                List<String> assembly = new ArrayList<>();
                List<Map<String, String>> outgoing = new ArrayList<>();
                for (Instruction instruction : currentProgram.getListing().getInstructions(f.getBody(), true)) {
                    assembly.add(instruction.getAddress() + ": " + instruction);
                    for (Reference r : instruction.getReferencesFrom()) outgoing.add(reference(r));
                }
                List<Map<String, String>> incoming = new ArrayList<>();
                for (Reference r : currentProgram.getReferenceManager().getReferencesTo(address)) incoming.add(reference(r));
                row.put("assembly", assembly);
                row.put("incoming_references", incoming);
                row.put("outgoing_references", outgoing);
                DecompileResults result = decompiler.decompileFunction(f, 60, monitor);
                row.put("decompile_completed", result.decompileCompleted());
                row.put("decompiler_message", result.getErrorMessage());
                if (!result.decompileCompleted()) throw new IllegalStateException(result.getErrorMessage());
                row.put("pseudocode", result.getDecompiledFunction().getC());
                write(out.resolve(address.toString() + ".json"), row);
            }
        } finally { decompiler.dispose(); }
        // Written last: the driver rejects incomplete runs even if headless exits zero.
        write(out.resolve("complete.json"), metadata);
    }
}
