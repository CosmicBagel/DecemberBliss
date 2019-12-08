#include "main.h"

#define BLISS_TITLE "DecemberBliss"
#define BLISS_VERSION "v0.0.0 dev"
#define MAX_MEMORY 1024

// init gui state
enum
{
	EASY,
	HARD
};
static int op = EASY;
static float value = 0.6f;
static int i = 20;

int main(void)
{
	printf(BLISS_TITLE " " BLISS_VERSION "\n");
	InitWindow(800, 600, BLISS_TITLE " " BLISS_VERSION);

	Font fontRobotoMono = LoadFontEx("fonts/RobotoMono-Regular.ttf", 28, 0, 255);
	Font fontRobotoMonoSm = LoadFontEx("fonts/RobotoMono-Regular.ttf", 16, 0, 255);

	// GUI controls initialization
	//----------------------------------------------------------------------------------
	int dropdownBox000Active = 0;
	bool dropDown000EditMode = false;

	int dropdownBox001Active = 0;
	bool dropDown001EditMode = false;

	int spinner001Value = 0;
	bool spinnerEditMode = false;

	int valueBox002Value = 0;
	bool valueBoxEditMode = false;

	char textBoxText[64] = "Text box";
	bool textBoxEditMode = false;

	int listViewScrollIndex = 0;
	int listViewActive = -1;

	int listViewExScrollIndex = 0;
	int listViewExActive = 2;
	int listViewExFocus = -1;
	const char *listViewExList[8] = {"This", "is", "a", "list view", "with", "disable", "elements", "amazing!"};

	char multiTextBoxText[141] = "Multi text box";
	bool multiTextBoxEditMode = false;
	Color colorPickerValue = RED;

	int sliderValue = 50;
	int sliderBarValue = 60;
	float progressValue = 0.4f;

	bool forceSquaredChecked = false;

	float alphaValue = 0.5f;

	int comboBoxActive = 1;

	int toggleGroupActive = 0;

	Vector2 viewScroll = {0, 0};
	//----------------------------------------------------------------------------------

	// Custom GUI font loading
	Font font = LoadFontEx("fonts/rainyhearts16.ttf", 12, 0, 0);
	GuiSetFont(font);

	bool exitWindow = false;
	bool showMessageBox = false;

	char textInput[256] = {0};
	bool showTextInputBox = false;

	char textInputFileName[256] = {0};

	SetTargetFPS(60);
	// UIState state;
	// initUI(&state);

	while (!WindowShouldClose())
	{

		BeginDrawing();

		ClearBackground(WHITE);
		char *msg = BLISS_TITLE " " BLISS_VERSION;
		Vector2 textDim = MeasureTextEx(fontRobotoMono, msg, (float)fontRobotoMono.baseSize, 0);
		Vector2 textPos;
		textPos.x = (800 - textDim.x) / 2.0f;
		textPos.y = (600 - textDim.y) / 2.0f;

		DrawTextEx(fontRobotoMono, msg, textPos,
				   (float)fontRobotoMono.baseSize, 0, DARKGRAY);

		char fpsStr[50] = {'\0'};
		sprintf_s(fpsStr, 50, "FPS: %d", GetFPS());
		Vector2 fpsPos;
		fpsPos.x = 10;
		fpsPos.y = 10;
		DrawTextEx(fontRobotoMonoSm, fpsStr, fpsPos,
				   (float)fontRobotoMonoSm.baseSize, 0, RED);

		// drawUI(&state);

		EndDrawing();
	}

	UnloadFont(fontRobotoMono);
	CloseWindow();

	return 0;
}

void initUI(UIState *state)
{
	// GUI controls initialization
	//----------------------------------------------------------------------------------
	state->dropdownBox000Active = 0;
	state->dropDown000EditMode = false;

	state->dropdownBox001Active = 0;
	state->dropDown001EditMode = false;

	state->spinner001Value = 0;
	state->spinnerEditMode = false;

	state->valueBox002Value = 0;
	state->valueBoxEditMode = false;

	char textBoxText[64] = "Text box";
	bool textBoxEditMode = false;

	state->listViewScrollIndex = 0;
	state->listViewActive = -1;

	state->listViewExScrollIndex = 0;
	state->listViewExActive = 2;
	state->listViewExFocus = -1;

	Fixed256String list[] = {"This", "is", "a", "list view", "with", "disable", "elements", "amazing!"};
	state->listViewActive = list;

	state->multiTextBoxText[141] = "Multi text box";
	state->multiTextBoxEditMode = false;
	state->colorPickerValue = RED;

	state->sliderValue = 50;
	state->sliderBarValue = 60;
	state->progressValue = 0.4f;

	state->forceSquaredChecked = false;

	state->alphaValue = 0.5f;

	state->comboBoxActive = 1;

	state->toggleGroupActive = 0;

	Vector2 viewScroll = {0, 0};
	state->viewScroll = viewScroll;
	//----------------------------------------------------------------------------------

	// Custom GUI font loading
	state->font = LoadFontEx("fonts/rainyhearts16.ttf", 12, 0, 0);
	GuiSetFont(state->font);

	state->exitWindow = false;
	state->showMessageBox = false;

	// state->textInput;
	memset(state->textInput, 0, 256);

	state->showTextInputBox = false;

	// state->textInputFileName = textInputFileName;
	memset(state->textInputFileName, 0, 256);
}

void drawUI(UIState *state)
{
	// raygui: controls drawing
	//----------------------------------------------------------------------------------
	if (state->dropDown000EditMode || state->dropDown001EditMode)
		GuiLock();
	//GuiDisable();

	// First GUI column
	//GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
	state->forceSquaredChecked = GuiCheckBox((Rectangle){25, 108, 15, 15}, "FORCE CHECK!", state->forceSquaredChecked);

	GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
	//GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
	if (GuiSpinner((Rectangle){25, 135, 125, 30}, NULL, &state->spinner001Value, 0, 100, state->spinnerEditMode))
		state->spinnerEditMode = !state->spinnerEditMode;
	if (GuiValueBox((Rectangle){25, 175, 125, 30}, NULL, &state->valueBox002Value, 0, 100, state->valueBoxEditMode))
		state->valueBoxEditMode = !state->valueBoxEditMode;
	GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
	if (GuiTextBox((Rectangle){25, 215, 125, 30}, state->textBoxText, 64, state->textBoxEditMode))
		state->textBoxEditMode = !state->textBoxEditMode;

	GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);

	if (GuiButton((Rectangle){25, 255, 125, 30}, GuiIconText(RICON_FILE_SAVE, "Save File")))
		state->showTextInputBox = true;

	GuiGroupBox((Rectangle){25, 310, 125, 150}, "STATES");
	GuiLock();
	GuiSetState(GUI_STATE_NORMAL);
	if (GuiButton((Rectangle){30, 320, 115, 30}, "NORMAL"))
	{
	}
	GuiSetState(GUI_STATE_FOCUSED);
	if (GuiButton((Rectangle){30, 355, 115, 30}, "FOCUSED"))
	{
	}
	GuiSetState(GUI_STATE_PRESSED);
	if (GuiButton((Rectangle){30, 390, 115, 30}, "#15#PRESSED"))
	{
	}
	GuiSetState(GUI_STATE_DISABLED);
	if (GuiButton((Rectangle){30, 425, 115, 30}, "DISABLED"))
	{
	}
	GuiSetState(GUI_STATE_NORMAL);
	GuiUnlock();

	state->comboBoxActive = GuiComboBox((Rectangle){25, 470, 125, 30}, "ONE;TWO;THREE;FOUR", state->comboBoxActive);

	// NOTE: GuiDropdownBox must draw after any other control that can be covered on unfolding
	GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
	if (GuiDropdownBox((Rectangle){25, 65, 125, 30}, "#01#ONE;#02#TWO;#03#THREE;#04#FOUR", &state->dropdownBox001Active, state->dropDown001EditMode))
		state->dropDown001EditMode = !state->dropDown001EditMode;

	GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
	if (GuiDropdownBox((Rectangle){25, 25, 125, 30}, "ONE;TWO;THREE", &state->dropdownBox000Active, state->dropDown000EditMode))
		state->dropDown000EditMode = !state->dropDown000EditMode;

	// Second GUI column
	state->listViewActive = GuiListView((Rectangle){165, 25, 140, 140}, "Charmander;Bulbasaur;#18#Squirtel;Pikachu;Eevee;Pidgey", &state->listViewScrollIndex, state->listViewActive);
	state->listViewExActive = GuiListViewEx((Rectangle){165, 180, 140, 200}, state->listViewExList, 8, &state->listViewExFocus, &state->listViewExScrollIndex, state->listViewExActive);

	state->toggleGroupActive = GuiToggleGroup((Rectangle){165, 400, 140, 25}, "#1#ONE\n#3#TWO\n#8#THREE\n#23#", state->toggleGroupActive);

	// Third GUI column
	if (GuiTextBoxMulti((Rectangle){320, 25, 225, 140}, state->multiTextBoxText, 141, state->multiTextBoxEditMode))
		state->multiTextBoxEditMode = !state->multiTextBoxEditMode;
	state->colorPickerValue = GuiColorPicker((Rectangle){320, 185, 196, 192}, state->colorPickerValue);

	state->sliderValue = GuiSlider((Rectangle){355, 400, 165, 20}, "TEST", TextFormat("%2.2f", (float)state->sliderValue), state->sliderValue, -50, 100);
	state->sliderBarValue = GuiSliderBar((Rectangle){320, 430, 200, 20}, NULL, TextFormat("%i", (int)state->sliderBarValue), state->sliderBarValue, 0, 100);
	state->progressValue = GuiProgressBar((Rectangle){320, 460, 200, 20}, NULL, NULL, state->progressValue, 0, 1);

	// NOTE: View rectangle could be used to perform some scissor test
	Rectangle view = GuiScrollPanel((Rectangle){560, 25, 100, 160}, (Rectangle){560, 25, 200, 400}, &state->viewScroll);

	GuiStatusBar((Rectangle){0, GetScreenHeight() - 20, GetScreenWidth(), 20}, "This is a status bar");

	state->alphaValue = GuiColorBarAlpha((Rectangle){320, 490, 200, 30}, state->alphaValue);

	if (state->showMessageBox)
	{
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
		int result = GuiMessageBox((Rectangle){GetScreenWidth() / 2 - 125, GetScreenHeight() / 2 - 50, 250, 100},
								   GuiIconText(RICON_EXIT, "Close Window"), "Do you really want to exit?", "Yes;No");

		if ((result == 0) || (result == 2))
			state->showMessageBox = false;
		else if (result == 1)
			state->exitWindow = true;
	}

	if (state->showTextInputBox)
	{
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
		int result = GuiTextInputBox((Rectangle){GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 - 60, 240, 140},
									 GuiIconText(RICON_FILE_SAVE, "Save file as..."), "Introduce a save file name", "Ok;Cancel", state->textInput);

		if (result == 1)
		{
			// TODO: Validate textInput value and save

			strcpy(state->textInputFileName, state->textInput);
		}

		if ((result == 0) || (result == 1) || (result == 2))
		{
			state->showTextInputBox = false;
			strcpy(state->textInput, "\0");
		}
	}

	GuiUnlock();
}