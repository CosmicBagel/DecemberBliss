#include <stdio.h>
#include "raylib.h"

#define BLISS_TITLE "DecemberBliss"
#define BLISS_VERSION "v0.0.0 dev"

int main()
{
	printf(BLISS_TITLE " " BLISS_VERSION "\n");
	InitWindow(800, 600, "DecemberBliss " BLISS_VERSION);

	Font fontRobotoMono = LoadFontEx("fonts/RobotoMono-Regular.ttf", 28, 0, 255);

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);
		char *msg = BLISS_TITLE " " BLISS_VERSION;
		Vector2 textDim = MeasureTextEx(fontRobotoMono, msg, (float)fontRobotoMono.baseSize, 0);
		DrawTextEx(fontRobotoMono,
				   msg,
				   (Vector2){(800 - textDim.x) / 2.0f, (600 - textDim.y) / 2.0f},
				   (float)fontRobotoMono.baseSize, 0, DARKGRAY);
		EndDrawing();
	}

	UnloadFont(fontRobotoMono);
	CloseWindow();

	return 0;
}