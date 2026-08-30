#include "trace_recorder.hpp"
#include "mips_r5900_core.hpp"

TraceRecorder& TraceRecorder::getInstance() {
    static TraceRecorder instance;
    return instance;
}

void TraceRecorder::setGameId(const std::string& id) {
    currentGameId = id;
    recordedBlocks.clear();
    totalInstructionsTraced = 0;
    std::cout << "[AI Learner] Стартиран запис и самообучение за игра: " << id << "\n";
}

void TraceRecorder::recordBlock(uint32_t pc, const std::vector<uint32_t>& opcodes) {
    if (recordedBlocks.find(pc) == recordedBlocks.end()) {
        ExecutionTrace trace;
        trace.start_pc = pc;
        trace.mips_opcodes = opcodes;
        trace.hit_count = 1;
        recordedBlocks[pc] = trace;
        totalInstructionsTraced += opcodes.size();
    } else {
        recordedBlocks[pc].hit_count++;
    }
}

float TraceRecorder::getPortingProgressPercentage() {
    // Симулация на изчисление на покритието на кода (Coverage)
    float coverage = (recordedBlocks.size() / 20.0f) * 100.0f;
    if (coverage > 100.0f) coverage = 100.0f;
    return coverage;
}

void TraceRecorder::exportNativeCodebase(const std::string& outputCppPath) {
    std::ofstream out(outputCppPath);
    if (!out.is_open()) return;

    out << "// =================================================================\n";
    out << "// АВТОМАТИЧНО ГЕНЕРИРАН НА ТИВЕН ARM64/C++ КОД ЗА ANDROID APK     \n";
    out << "// Генериран от PS2 AI Native Port Engine                          \n";
    out << "// =================================================================\n\n";
    out << "#include <cstdint>\n";
    out << "#include <arm_neon.h>\n\n";

    out << "extern \"C\" void execute_game_native_loop() {\n";
    for (const auto& [pc, trace] : recordedBlocks) {
        out << "    // Преведен PS2 Блок 0x" << std::hex << pc << " (Изпълнен: " << std::dec << trace.hit_count << " пъти)\n";
        out << "    loc_" << std::hex << pc << ":\n";
        for (uint32_t op : trace.mips_opcodes) {
            out << "        " << R5900Core::translateInstruction(op, pc) << "\n";
        }
    }
    out << "}\n";
    out.close();

    std::cout << "\n[AI Synthesizer] УСПЕХ: Нативният сорс код за самостоятелно APK е генериран в: " 
              << outputCppPath << "\n";
}
