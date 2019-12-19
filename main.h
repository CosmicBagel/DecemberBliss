#ifndef __MAIN_H__
#define __MAIN_H__

//don't give warnings for external stuff that can't be controlled
#ifdef _MSC_VER //msvc
#pragma warning(push, 0)
#else //gcc or clang
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W"
#endif

#include <stdio.h>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION // Avoid including raygui implementation again

#include "ricons.h"

#ifdef _MSC_VER //msvc
#pragma warning(pop)
#else //gcc or clang
#pragma GCC diagnostic pop
#endif

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