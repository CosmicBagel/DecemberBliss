#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include "raylib.h"

int main(void);

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui/raygui.h"
#undef RAYGUI_IMPLEMENTATION // Avoid including raygui implementation again

#include "raygui/ricons.h"

typedef char Fixed256String[255];

typedef struct UIState
{
    int dropdownBox000Active;
    bool dropDown000EditMode;

    int dropdownBox001Active;
    bool dropDown001EditMode;

    int spinner001Value;
    bool spinnerEditMode;

    int valueBox002Value;
    bool valueBoxEditMode;

    char textBoxText[64];
    bool textBoxEditMode;

    int listViewScrollIndex;
    int listViewActive;

    int listViewExScrollIndex;
    int listViewExActive;
    int listViewExFocus;
    Fixed256String listViewExList[8];

    char multiTextBoxText[141];
    bool multiTextBoxEditMode;
    Color colorPickerValue;

    int sliderValue;
    int sliderBarValue;
    float progressValue;

    bool forceSquaredChecked;

    float alphaValue;

    int comboBoxActive;

    int toggleGroupActive;

    Vector2 viewScroll;
    //----------------------------------------------------------------------------------

    // Custom GUI font loading
    Font font;

    bool exitWindow;
    bool showMessageBox;

    char textInput[256];
    bool showTextInputBox;

    char textInputFileName[256];
} UIState;

void initUI(UIState *state);
void drawUI(UIState *state);

#endif