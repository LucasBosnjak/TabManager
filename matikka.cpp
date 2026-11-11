#include "matikka.h"
#include <fstream>
#include <sstream>

// Loads users from the specified file
void LoadUsers(std::vector<User>& usersList, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return; 
    
    usersList.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, amountStr;
        
        if (std::getline(ss, name, ',') && std::getline(ss, amountStr)) {
            try {
                usersList.push_back({name, std::stod(amountStr)});
            } catch (...) {
                // Ignore formatting errors for a single line
            }
        }
    }
}

// Saves users to the specified file
void SaveUsers(const std::vector<User>& usersList, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    for (const auto& user : usersList) {
        file << user.name << "," << user.amount << "\n";
    }
}

// Handles user keyboard input via Raylib
void HandleTextInput(std::string& inputStr, int maxLength, bool onlyNumbers) {
    int key = GetCharPressed(); //[cite: 4]
    while (key > 0) {
        if (inputStr.length() < (size_t)maxLength) {
            if (onlyNumbers) {
                if ((key >= '0' && key <= '9') || key == '.') {
                    inputStr += (char)key;
                }
            } else {
                if (key >= 32 && key <= 125 && key != ',') {
                    inputStr += (char)key;
                }
            }
        }
        key = GetCharPressed(); //[cite: 4]
    }
    
    if (IsKeyPressed(259) && !inputStr.empty()) { // 259 is KEY_BACKSPACE[cite: 4]
        inputStr.pop_back();
    }
}

// Renders the stylized background panels for UI elements
void DrawPanel(Rectangle rec, Color bgColor, Color borderColor, bool isSelected) {
    DrawRectangleRounded({rec.x + 5, rec.y + 5, rec.width, rec.height}, 0.2f, 10, {0, 0, 0, 50}); //[cite: 6]
    DrawRectangleRounded(rec, 0.2f, 10, isSelected ? SKYBLUE : bgColor); //[cite: 6, 13]
    DrawRectangleRoundedLinesEx(rec, 0.2f, 10, 3.0f, isSelected ? BLUE : borderColor); //[cite: 6, 13]
}