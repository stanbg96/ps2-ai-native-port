#pragma once
#include <string>
#include <iostream>

enum class GameState { MENU, GAMEPLAY_ACTION, GAMEPLAY_DRIVING, CUTSCENE };

class InputAiManager {
private:
    bool gamepadConnected = false;
    GameState currentState = GameState::MENU;
public:
    void detectGamepad();
    void updateVisualState(GameState newState);
    void renderAdaptiveControls();
};
