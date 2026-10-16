// Generated pseudocode stays in build/. It is not recovered or matching source.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.framework.Application;
import java.nio.file.*;
import java.util.*;
import com.google.gson.*;

public class DecompileCensus extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String sha = currentProgram.getExecutableSHA256();
        if (!sha.equalsIgnoreCase(args[1])) throw new IllegalStateException("Module hash mismatch");
        Path out = Path.of(args[0]);
        Files.createDirectories(out);
        Path receipt = out.resolve("complete.json");
        Files.deleteIfExists(receipt);
        DecompInterface decompiler = new DecompInterface();
        if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
        Gson gson = new Gson();
        List<String> entries = new ArrayList<>();
        List<String> failures = new ArrayList<>();
        try (var stream = Files.newBufferedWriter(out.resolve("pseudocode.jsonl"))) {
            for (var f : currentProgram.getFunctionManager().getFunctions(true)) {
                monitor.checkCancelled();
                String address = f.getEntryPoint().toString();
                entries.add(address);
                var result = decompiler.decompileFunction(f, 30, monitor);
                Map<String, Object> row = new LinkedHashMap<>();
                row.put("address", address);
                row.put("name", f.getName());
                row.put("size", f.getBody().getNumAddresses());
                row.put("completed", result.decompileCompleted());
                row.put("message", result.getErrorMessage());
                if (result.decompileCompleted()) row.put("pseudocode", result.getDecompiledFunction().getC());
                else failures.add(address);
                stream.write(gson.toJson(row));
                stream.newLine();
                if (entries.size() % 1000 == 0) {
                    stream.flush();
                    println("Decompiled " + entries.size() + "; failures " + failures.size());
                }
            }
        } finally { decompiler.dispose(); }
        Map<String, Object> metadata = new LinkedHashMap<>();
        metadata.put("sha256", sha);
        metadata.put("ghidra_version", Application.getApplicationVersion());
        metadata.put("entries", entries);
        metadata.put("failed_entries", failures);
        metadata.put("completed", entries.size() - failures.size());
        Files.writeString(receipt, new GsonBuilder().setPrettyPrinting().create().toJson(metadata) + "\n");
    }
}
