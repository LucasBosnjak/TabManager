#include "raylib.h"
#include "matikka.h" // Includes the custom functions, enums, and structs

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); //[cite: 3]
    InitWindow(1280, 720, "Enterprise User Management"); //[cite: 1]
    SetTargetFPS(60); //[cite: 2]

    AppState currentState = STATE_MAIN;
    ConfirmAction pendingAction = NONE;
    bool showHelp = false;
    float successTimer = 0.0f;

    std::vector<User> usersList;
    const std::string filename = "numbers.txt";
    
    // Load from file on startup
    LoadUsers(usersList, filename);
    
    if (usersList.empty()) {
        usersList = {
            {"Admin", 150.50}
        };
        SaveUsers(usersList, filename);
    }
    
    int selectedUserIndex = -1;
    double stagedAmount = 0.0; 
    double parsedCustomAmount = 0.0;
    float scrollOffset = 0.0f; 

    std::string passwordInput = "";
    std::string newUserName = "";
    std::string customAmountStr = "";

    std::vector<Button> buttons;
    double valueOptions[10] = {1.0, 5.0, 10.0, 20.0, 50.0, 100.0, 250.0, 500.0, 1000.0, 5000.0};
    
    for (int i = 0; i < 10; i++) {
        buttons.push_back({ {0,0,0,0}, valueOptions[i], TextFormat("+$%.0f", valueOptions[i]) }); //[cite: 9]
    }

    while (!WindowShouldClose()) { //[cite: 1]
        int sw = GetScreenWidth(); //[cite: 1]
        int sh = GetScreenHeight(); //[cite: 1]
        Vector2 mousePoint = GetMousePosition(); //[cite: 5]
        bool clicked = IsMouseButtonPressed(0); //[cite: 4]
        float wheel = GetMouseWheelMove(); //[cite: 5]

        if (successTimer > 0.0f) {
            successTimer -= GetFrameTime(); //[cite: 2]
        }

        // --- DYNAMIC UI LAYOUT CALCULATIONS ---
        float leftPaneW = (sw * 0.45f > 400.0f) ? sw * 0.45f : 400.0f;
        float rightPaneX = leftPaneW + 100.0f; 
        
        float btnW = 160.0f; 
        float btnH = 65.0f;
        float gapX = 20.0f;
        float gapY = 20.0f;

        for (int i = 0; i < 10; i++) {
            int col = i % 2; 
            int row = i / 2; 
            buttons[i].rec = {rightPaneX + (col * (btnW + gapX)), 150.0f + (row * (btnH + gapY)), btnW, btnH}; //[cite: 13]
        }

        Rectangle confirmBtn = { rightPaneX, 150.0f + 5 * (btnH + gapY), btnW * 1.5f, btnH * 1.2f }; //[cite: 13]
        Rectangle resetBtn = { rightPaneX + btnW * 1.5f + gapX, 150.0f + 5 * (btnH + gapY), btnW * 0.8f, btnH * 1.2f }; //[cite: 13]
        
        Rectangle helpBtn = { (float)sw - 140.0f, 25.0f, 110.0f, 50.0f }; //[cite: 13]
        Rectangle managerBtn = { helpBtn.x - 240.0f, 25.0f, 220.0f, 50.0f }; //[cite: 13]
        Rectangle backBtn = { 30.0f, 25.0f, 150.0f, 50.0f }; //[cite: 13]

        Rectangle addUserBtn = { rightPaneX, 150.0f, btnW * 2 + gapX, btnH }; //[cite: 13]
        Rectangle removeUserBtn = { rightPaneX, 150.0f + btnH + gapY, btnW * 2 + gapX, btnH }; //[cite: 13]
        Rectangle addCustomBtn = { rightPaneX, 420.0f, btnW, btnH }; //[cite: 13]
        Rectangle subCustomBtn = { rightPaneX + btnW + gapX, 420.0f, btnW, btnH }; //[cite: 13]

        Rectangle modalRec = { sw * 0.25f, sh * 0.25f, sw * 0.5f, sh * 0.5f }; //[cite: 13]
        Rectangle modalYes = { modalRec.x + modalRec.width * 0.1f, modalRec.y + modalRec.height - 90, modalRec.width * 0.35f, 60 }; //[cite: 13]
        Rectangle modalNo = { modalRec.x + modalRec.width * 0.55f, modalRec.y + modalRec.height - 90, modalRec.width * 0.35f, 60 }; //[cite: 13]

        // --- SCROLLING LOGIC ---
        if (mousePoint.x < leftPaneW + 50.0f) scrollOffset += wheel * 50.0f;
        float maxScroll = (usersList.size() * 105.0f) - (sh - 150.0f);
        if (maxScroll < 0) maxScroll = 0;
        if (scrollOffset < -maxScroll) scrollOffset = -maxScroll;
        if (scrollOffset > 0) scrollOffset = 0;

        // --- LOGIC & INPUT HANDLING ---
        if (showHelp) {
            if (clicked) showHelp = false; 
        } 
        else if (pendingAction != NONE) {
            if (clicked) {
                if (CheckCollisionPointRec(mousePoint, modalYes)) { //[cite: 6]
                    if (pendingAction == CONFIRM_REMOVE) {
                        usersList.erase(usersList.begin() + selectedUserIndex);
                        selectedUserIndex = -1;
                    } else if (pendingAction == CONFIRM_ADD_CUSTOM) {
                        usersList[selectedUserIndex].amount += parsedCustomAmount;
                    } else if (pendingAction == CONFIRM_SUB_CUSTOM) {
                        usersList[selectedUserIndex].amount -= parsedCustomAmount;
                    } else if (pendingAction == CONFIRM_NEW_USER) {
                        usersList.push_back({newUserName, 0.0});
                        currentState = STATE_MANAGER;
                    }
                    
                    SaveUsers(usersList, filename);
                    successTimer = 1.0f; 
                    
                    customAmountStr = "";
                    pendingAction = NONE;
                }
                if (CheckCollisionPointRec(mousePoint, modalNo)) { //[cite: 6]
                    pendingAction = NONE; 
                }
            }
        } 
        else {
            if (clicked && CheckCollisionPointRec(mousePoint, helpBtn)) showHelp = true; //[cite: 6]

            switch (currentState) {
                case STATE_MAIN: {
                    Rectangle listArea = { 50.0f, 110.0f, leftPaneW, (float)sh - 110.0f }; //[cite: 13]
                    if (CheckCollisionPointRec(mousePoint, listArea)) { //[cite: 6]
                        for (size_t i = 0; i < usersList.size(); i++) {
                            Rectangle userRec = { 50.0f, 140.0f + (i * 105.0f) + scrollOffset, leftPaneW, 90.0f }; //[cite: 13]
                            if (clicked && CheckCollisionPointRec(mousePoint, userRec)) { //[cite: 6]
                                selectedUserIndex = (int)i;
                                stagedAmount = 0.0; 
                            }
                        }
                    }

                    if (selectedUserIndex != -1 && clicked) {
                        for (const auto& btn : buttons) {
                            if (CheckCollisionPointRec(mousePoint, btn.rec)) stagedAmount += btn.addValue; //[cite: 6]
                        }
                        if (stagedAmount > 0.0 && CheckCollisionPointRec(mousePoint, confirmBtn)) { //[cite: 6]
                            usersList[selectedUserIndex].amount += stagedAmount;
                            stagedAmount = 0.0;
                            
                            SaveUsers(usersList, filename);
                            successTimer = 1.0f; 
                        }
                        if (CheckCollisionPointRec(mousePoint, resetBtn)) stagedAmount = 0.0; //[cite: 6]
                    }

                    if (clicked && CheckCollisionPointRec(mousePoint, managerBtn)) { //[cite: 6]
                        currentState = STATE_PASSWORD;
                        passwordInput = "";
                    }
                } break;

                case STATE_PASSWORD: {
                    HandleTextInput(passwordInput, 10, false);
                    if (IsKeyPressed(257)) { // KEY_ENTER[cite: 4]
                        if (passwordInput == "admin") { 
                            currentState = STATE_MANAGER;
                            selectedUserIndex = -1;
                        } else currentState = STATE_MAIN;
                    }
                    if (clicked && CheckCollisionPointRec(mousePoint, backBtn)) currentState = STATE_MAIN; //[cite: 6]
                } break;

                case STATE_MANAGER: {
                    if (clicked && CheckCollisionPointRec(mousePoint, backBtn)) currentState = STATE_MAIN; //[cite: 6]
                    
                    Rectangle listArea = { 50.0f, 110.0f, leftPaneW, (float)sh - 110.0f }; //[cite: 13]
                    if (CheckCollisionPointRec(mousePoint, listArea)) { //[cite: 6]
                        for (size_t i = 0; i < usersList.size(); i++) {
                            Rectangle userRec = { 50.0f, 140.0f + (i * 105.0f) + scrollOffset, leftPaneW, 90.0f }; //[cite: 13]
                            if (clicked && CheckCollisionPointRec(mousePoint, userRec)) selectedUserIndex = (int)i; //[cite: 6]
                        }
                    }

                    if (clicked && CheckCollisionPointRec(mousePoint, addUserBtn)) { //[cite: 6]
                        currentState = STATE_ADD_USER;
                        newUserName = "";
                    }

                    if (selectedUserIndex != -1) {
                        HandleTextInput(customAmountStr, 8, true); 

                        if (clicked) {
                            if (CheckCollisionPointRec(mousePoint, removeUserBtn)) { //[cite: 6]
                                pendingAction = CONFIRM_REMOVE;
                            } else {
                                parsedCustomAmount = (double)TextToFloat(customAmountStr.c_str()); //[cite: 10]
                                if (parsedCustomAmount > 0) {
                                    if (CheckCollisionPointRec(mousePoint, addCustomBtn)) pendingAction = CONFIRM_ADD_CUSTOM; //[cite: 6]
                                    if (CheckCollisionPointRec(mousePoint, subCustomBtn)) pendingAction = CONFIRM_SUB_CUSTOM; //[cite: 6]
                                }
                            }
                        }
                    }
                } break;

                case STATE_ADD_USER: {
                    HandleTextInput(newUserName, 15, false);
                    if (IsKeyPressed(257) && !newUserName.empty()) { //[cite: 4]
                        pendingAction = CONFIRM_NEW_USER;
                    }
                    if (clicked && CheckCollisionPointRec(mousePoint, backBtn)) currentState = STATE_MANAGER; //[cite: 6]
                } break;
            }
        }

        // --- DRAWING ---
        BeginDrawing(); //[cite: 2]
        ClearBackground(RAYWHITE); //[cite: 2, 13]

        DrawRectangle(0, 0, sw, 100, DARKBLUE); //[cite: 6, 13]
        DrawPanel(helpBtn, PURPLE, VIOLET);
        DrawText("HELP", helpBtn.x + 25, helpBtn.y + 13, 24, WHITE); //[cite: 9, 13]

        if (currentState == STATE_MAIN || currentState == STATE_MANAGER) {
            
            if (currentState == STATE_MAIN) {
                DrawText("DASHBOARD", 50, 35, 34, WHITE); //[cite: 9, 13]
                DrawPanel(managerBtn, ORANGE, RED);
                DrawText("MANAGER LOGIN", managerBtn.x + 15, managerBtn.y + 13, 22, WHITE); //[cite: 9, 13]
            } else {
                DrawText("MANAGER CONSOLE", 50, 35, 34, WHITE); //[cite: 9, 13]
                DrawPanel(backBtn, GRAY, DARKGRAY);
                DrawText("<- LOGOUT", backBtn.x + 15, backBtn.y + 13, 22, WHITE); //[cite: 9, 13]
            }

            BeginScissorMode(40, 105, leftPaneW + 20, sh - 105); //[cite: 2]
            for (size_t i = 0; i < usersList.size(); i++) {
                Rectangle userRec = { 50.0f, 140.0f + (i * 105.0f) + scrollOffset, leftPaneW, 90.0f }; //[cite: 13]
                DrawPanel(userRec, WHITE, LIGHTGRAY, (selectedUserIndex == (int)i));
                DrawText(usersList[i].name.c_str(), userRec.x + 20, userRec.y + 15, 32, BLACK); //[cite: 9, 13]
                DrawText(TextFormat("Balance: $%.2f", usersList[i].amount), userRec.x + 20, userRec.y + 55, 24, DARKGRAY); //[cite: 9, 13]
            }
            EndScissorMode(); //[cite: 2]

            if (currentState == STATE_MAIN) {
                for (const auto& btn : buttons) {
                    Color btnColor = (selectedUserIndex == -1) ? LIGHTGRAY : (CheckCollisionPointRec(mousePoint, btn.rec) ? BLUE : DARKBLUE); //[cite: 6, 13]
                    DrawPanel(btn.rec, btnColor, btnColor);
                    int tWidth = MeasureText(btn.label.c_str(), 28); //[cite: 9]
                    DrawText(btn.label.c_str(), btn.rec.x + (btn.rec.width/2) - (tWidth/2), btn.rec.y + 18, 28, WHITE); //[cite: 9, 13]
                }

                Color cColor = (stagedAmount > 0) ? GREEN : LIGHTGRAY; //[cite: 13]
                DrawPanel(confirmBtn, cColor, cColor);
                DrawText((stagedAmount > 0) ? TextFormat("CONFIRM +$%.0f", stagedAmount) : "CONFIRM", confirmBtn.x + 20, confirmBtn.y + 25, 26, WHITE); //[cite: 9, 13]

                DrawPanel(resetBtn, RED, MAROON);
                DrawText("RESET", resetBtn.x + 20, resetBtn.y + 25, 24, WHITE); //[cite: 9, 13]
            }
            else {
                DrawPanel(addUserBtn, GREEN, DARKGREEN);
                DrawText("+ ADD NEW USER", addUserBtn.x + 30, addUserBtn.y + 18, 26, WHITE); //[cite: 9, 13]

                if (selectedUserIndex != -1) {
                    DrawPanel(removeUserBtn, RED, MAROON);
                    DrawText("- REMOVE USER", removeUserBtn.x + 35, removeUserBtn.y + 18, 26, WHITE); //[cite: 9, 13]

                    DrawText("Custom Adjust Amount:", rightPaneX, 330.0f, 26, DARKGRAY); //[cite: 9, 13]
                    Rectangle inputRec = { rightPaneX, 365.0f, btnW * 2 + gapX, 45.0f }; //[cite: 13]
                    DrawRectangleRec(inputRec, LIGHTGRAY); //[cite: 6, 13]
                    DrawText(customAmountStr.c_str(), inputRec.x + 10, inputRec.y + 10, 26, BLACK); //[cite: 9, 13]

                    DrawPanel(addCustomBtn, BLUE, DARKBLUE);
                    DrawText("ADD", addCustomBtn.x + 50, addCustomBtn.y + 18, 26, WHITE); //[cite: 9, 13]
                    
                    DrawPanel(subCustomBtn, ORANGE, RED);
                    DrawText("MINUS", subCustomBtn.x + 40, subCustomBtn.y + 18, 26, WHITE); //[cite: 9, 13]
                } else {
                    DrawText("Select a user from the list to edit", rightPaneX, 330.0f, 22, GRAY); //[cite: 9, 13]
                }
            }
        }
        else if (currentState == STATE_PASSWORD) {
            DrawText("MANAGER AUTHENTICATION", 50, 35, 34, WHITE); //[cite: 9, 13]
            DrawPanel(backBtn, GRAY, DARKGRAY);
            DrawText("<- BACK", backBtn.x + 25, backBtn.y + 13, 24, WHITE); //[cite: 9, 13]

            Rectangle pwBox = { sw / 2.0f - 250, sh / 2.0f - 50, 500, 90 }; //[cite: 13]
            DrawPanel(pwBox, WHITE, LIGHTGRAY);
            std::string masked(passwordInput.length(), '*');
            DrawText("Enter Password and press ENTER:", pwBox.x, pwBox.y - 35, 24, DARKGRAY); //[cite: 9, 13]
            DrawText(masked.c_str(), pwBox.x + 20, pwBox.y + 25, 40, BLACK); //[cite: 9, 13]
        }
        else if (currentState == STATE_ADD_USER) {
            DrawText("ADD NEW USER", 50, 35, 34, WHITE); //[cite: 9, 13]
            DrawPanel(backBtn, GRAY, DARKGRAY);
            DrawText("<- BACK", backBtn.x + 25, backBtn.y + 13, 24, WHITE); //[cite: 9, 13]

            Rectangle nameBox = { sw / 2.0f - 250, sh / 2.0f - 50, 500, 90 }; //[cite: 13]
            DrawPanel(nameBox, WHITE, LIGHTGRAY);
            DrawText("Type Name and press ENTER:", nameBox.x, nameBox.y - 35, 24, DARKGRAY); //[cite: 9, 13]
            DrawText(newUserName.c_str(), nameBox.x + 20, nameBox.y + 25, 34, BLACK); //[cite: 9, 13]
        }

        // --- DRAW MODAL OVERLAYS ---
        if (showHelp) {
            DrawRectangle(0, 0, sw, sh, {0, 0, 0, 150}); //[cite: 6, 13]
            DrawPanel(modalRec, RAYWHITE, DARKGRAY);
            DrawText("HELP & INSTRUCTIONS", modalRec.x + 30, modalRec.y + 30, 34, DARKBLUE); //[cite: 9, 13]
            
            float textY = modalRec.y + 100;
            if (currentState == STATE_MAIN) {
                DrawText("- Scroll to view users. Click a user to select.", modalRec.x + 30, textY, 24, BLACK); //[cite: 9, 13]
                DrawText("- Click number buttons to stage funds.", modalRec.x + 30, textY += 40, 24, BLACK); //[cite: 9, 13]
                DrawText("- Click CONFIRM to apply, or RESET to cancel.", modalRec.x + 30, textY += 40, 24, BLACK); //[cite: 9, 13]
                DrawText("- Click MANAGER LOGIN to manage accounts.", modalRec.x + 30, textY += 40, 24, BLACK); //[cite: 9, 13]
            } else {
                DrawText("- Scroll to view users. Click a user to select.", modalRec.x + 30, textY, 24, BLACK); //[cite: 9, 13]
                DrawText("- Type custom numbers to ADD or MINUS.", modalRec.x + 30, textY += 40, 24, BLACK); //[cite: 9, 13]
                DrawText("- Click ADD NEW USER to create an account.", modalRec.x + 30, textY += 40, 24, BLACK); //[cite: 9, 13]
                DrawText("- All actions will prompt for confirmation.", modalRec.x + 30, textY += 40, 24, BLACK); //[cite: 9, 13]
            }
            DrawText("(Click anywhere to close)", modalRec.x + 30, modalRec.y + modalRec.height - 50, 22, GRAY); //[cite: 9, 13]
        } 
        else if (pendingAction != NONE) {
            DrawRectangle(0, 0, sw, sh, {0, 0, 0, 150}); //[cite: 6, 13]
            DrawPanel(modalRec, RAYWHITE, DARKGRAY);
            DrawText("CONFIRM ACTION", modalRec.x + 30, modalRec.y + 30, 34, RED); //[cite: 9, 13]

            const char* promptText = "";
            if (pendingAction == CONFIRM_REMOVE) promptText = TextFormat("Remove user '%s'?", usersList[selectedUserIndex].name.c_str()); //[cite: 9]
            if (pendingAction == CONFIRM_ADD_CUSTOM) promptText = TextFormat("Add $%.2f to '%s'?", parsedCustomAmount, usersList[selectedUserIndex].name.c_str()); //[cite: 9]
            if (pendingAction == CONFIRM_SUB_CUSTOM) promptText = TextFormat("Subtract $%.2f from '%s'?", parsedCustomAmount, usersList[selectedUserIndex].name.c_str()); //[cite: 9]
            if (pendingAction == CONFIRM_NEW_USER) promptText = TextFormat("Create new user '%s'?", newUserName.c_str()); //[cite: 9]

            DrawText(promptText, modalRec.x + 30, modalRec.y + 100, 28, BLACK); //[cite: 9, 13]

            DrawPanel(modalYes, GREEN, DARKGREEN);
            DrawText("YES", modalYes.x + (modalYes.width/2) - 20, modalYes.y + 18, 28, WHITE); //[cite: 9, 13]
            
            DrawPanel(modalNo, RED, MAROON);
            DrawText("NO", modalNo.x + (modalNo.width/2) - 15, modalNo.y + 18, 28, WHITE); //[cite: 9, 13]
        }

        if (successTimer > 0.0f) {
            Color glowColor = Fade(LIME, successTimer); //[cite: 8, 13]
            DrawRectangleLinesEx({0, 0, (float)sw, (float)sh}, 20.0f, glowColor); //[cite: 6]
        }

        EndDrawing(); //[cite: 2]
    }

    CloseWindow(); //[cite: 1]
    return 0;
}