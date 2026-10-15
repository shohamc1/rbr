// Seed only reviewed boundaries before automatic analysis.
import ghidra.app.script.GhidraScript;
import ghidra.framework.Application;
import com.google.gson.*;
import java.util.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.*;

public class SeedFunctions extends GhidraScript {
    public void run() throws Exception {
        JsonObject settings = JsonParser.parseString(Files.readString(Path.of(getScriptArgs()[1]))).getAsJsonObject();
        if (!Application.getApplicationVersion().equals(settings.get("version").getAsString()))
            throw new IllegalStateException("Ghidra version differs from config/ghidra.json");
        Map<String, String> options = new HashMap<>();
        for (var entry : settings.getAsJsonObject("analyzers").entrySet())
            options.put(entry.getKey(), entry.getValue().getAsString());
        setAnalysisOptions(currentProgram, options);
        for (String line : Files.readAllLines(Path.of(getScriptArgs()[0]))) {
            String[] fields = line.split("\\t");
            Address start = toAddr(fields[0]);
            long size = Long.parseLong(fields[1]);
            disassemble(start);
            if (getFunctionAt(start) == null) {
                currentProgram.getFunctionManager().createFunction(fields[2], start,
                    new AddressSet(start, start.add(size - 1)), SourceType.USER_DEFINED);
            }
        }
    }
}
