#ifndef __MAIN_H__
#define __MAIN_H__

//#include <stdio.h>
//#include <stdbool.h>
#include "rayIncludes.h"

// #include "imgui-1.74/imgui.h"
// #include "imgui-1.74/imgui_internal.h"

//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

int main(void);

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