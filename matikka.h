#ifndef MATIKKA_H
#define MATIKKA_H

#include "raylib.h"
#include <vector>
#include <string>

// App States
enum AppState {
    STATE_MAIN,
    STATE_PASSWORD,
    STATE_MANAGER,
    STATE_ADD_USER
};

// Actions requiring confirmation
enum ConfirmAction {
    NONE,
    CONFIRM_REMOVE,
    CONFIRM_ADD_CUSTOM,
    CONFIRM_SUB_CUSTOM,
    CONFIRM_NEW_USER
};

struct User {
    std::string name;
    double amount;
};

struct Button {
    Rectangle rec;
    double addValue;
    std::string label;
};

// Function declarations
void LoadUsers(std::vector<User>& usersList, const std::string& filename);
void SaveUsers(const std::vector<User>& usersList, const std::string& filename);
void HandleTextInput(std::string& inputStr, int maxLength, bool onlyNumbers = false);
void DrawPanel(Rectangle rec, Color bgColor, Color borderColor, bool isSelected = false);

#endif