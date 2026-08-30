#include "input_ai_manager.hpp"

void InputAiManager::detectGamepad() {
    gamepadConnected = false;
    std::cout << "[Input System] Джойстик статус: " << (gamepadConnected ? "СВЪРЗАН (Тъч скрит)" : "Няма (Адаптивен тъч активен)") << "\n";
}

void InputAiManager::updateVisualState(GameState newState) {
    currentState = newState;
    renderAdaptiveControls();
}

void InputAiManager::renderAdaptiveControls() {
    if (gamepadConnected) return;
    std::cout << "[AI UI] Режим: ";
    if (currentState == GameState::MENU) std::cout << "Меню (Директен Touch)\n";
    else if (currentState == GameState::CUTSCENE) std::cout << "Филмче (100% чист екран)\n";
    else if (currentState == GameState::GAMEPLAY_ACTION) std::cout << "Екшън (Плаващ аналог + [X, O, [], ^])\n";
    else if (currentState == GameState::GAMEPLAY_DRIVING) std::cout << "Шофиране (Газ/Спирачка + Жироскоп)\n";
}
