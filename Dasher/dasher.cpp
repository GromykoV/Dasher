#include "raylib.h"

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
	return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
	// update running time
	data.runningTime += deltaTime;
	if (data.runningTime >= data.updateTime)
	{
		data.runningTime = 0.0;
		// update animation frame
		data.rec.x = data.frame * data.rec.width;
		data.frame++;
		if (data.frame > maxFrame)
		{
			data.frame = 0;
		}
	}
	return data;
}

int main()
{
	// Window dimensions array
	int windowDimentions[2];
	windowDimentions[0] = 512 * 2;
	windowDimentions[1] = 380 * 2;

	// initialize the window
	InitWindow(windowDimentions[0], windowDimentions[1], "Dapper Dasher!");

	// acceleration due to gravity (pixels/frame)/frame
	const int gravity{ 1'000 };

	// Nebula ver
	Texture2D nebula = LoadTexture("D:/VisualStudioProjects/Dasher/textures/12_nebula_spritesheet.png");

	const int sizeOfNebulae{ 5 };

	AnimData nebulae[sizeOfNebulae]{};

	for (int i = 0; i < sizeOfNebulae; i++)
	{
		nebulae[i].rec.x = 0.0;
		nebulae[i].rec.y = 0.0;
		nebulae[i].rec.width = nebula.width / 8;
		nebulae[i].rec.height = nebula.height / 8;
		nebulae[i].pos.y = windowDimentions[1] - nebula.height / 8;
		nebulae[i].frame = 0;
		nebulae[i].updateTime = 0.0;
		nebulae[i].runningTime = 0.0;
		nebulae[i].pos.x = windowDimentions[0] + i * 400 - i * 40;
	}

	float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

	// nebula x velosity
	int nebVel{ -300 };

	// Scarfy ver
	Texture2D scarfy = LoadTexture("D:/VisualStudioProjects/Dasher/textures/scarfy.png");
	AnimData scarfyData;
	scarfyData.rec.width = scarfy.width / 6;
	scarfyData.rec.height = scarfy.height;
	scarfyData.rec.x = 0;
	scarfyData.rec.y = 0;
	scarfyData.pos.x = windowDimentions[0] / 2 - scarfyData.rec.width / 2;
	scarfyData.pos.y = windowDimentions[1] - scarfyData.rec.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0.0;

	// is the rectanlge in the air?
	bool isInAir{};

	// jump velocity pix/sec 
	const int jumpVel{ -500 };
	int velocity{ 0 };

	Texture2D background = LoadTexture("D:/VisualStudioProjects/Dasher/textures/far-buildings.png");
	float bgX{};

	Texture2D midground = LoadTexture("D:/VisualStudioProjects/Dasher/textures/back-buildings.png");
	float mgX{};

	Texture2D foreground = LoadTexture("D:/VisualStudioProjects/Dasher/textures/foreground.png");
	float fgX{};

	bool collision{ false };

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		
		// Delta time
		const float dT{ GetFrameTime() };

		// Start drawing
		BeginDrawing();
		ClearBackground(WHITE);

		bgX -= 20 * dT;
		if (bgX <= -background.width * 2)
		{
			bgX = 0.0;
		}

		mgX -= 40 * dT;
		if (mgX <= -midground.width * 2)
		{
			mgX = 0.0;
		}

		fgX -= 80 * dT;
		if (fgX <= -foreground.width * 2)
		{
			fgX = 0.0;
		}

		// Draw the background
		Vector2 bg1Pos{ bgX, 0.0 };
		DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
		Vector2 bg2Pos{ bgX + background.width * 2 };
		DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
		Vector2 mg1Pos{ mgX, 0.0 };
		DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
		Vector2 mg2Pos{ mgX + midground.width * 2, 0.0 };
		DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);
		Vector2 fg1Pos{ fgX, 0.0 };
		DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
		Vector2 fg2Pos{ fgX + foreground.width * 2, 0.0 };
		DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

		// perform ground check
		if (isOnGround(scarfyData, windowDimentions[1]))
		{
			// rectangle is on the ground
			velocity = 0;
			isInAir = false;
		}
		else
		{
			// rectangle is in the air
			velocity += gravity * dT;
			isInAir = true;
		}

		// jump check
		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			velocity += jumpVel;
		}

		for (int i = 0; i < sizeOfNebulae; i++)
		{
			// update position of each nebula
			nebulae[i].pos.x += nebVel * dT;
		}

		// update finish line
		finishLine += nebVel * dT;

		// update the position
		scarfyData.pos.y += velocity * dT;


		if (!isInAir)
		{
			scarfyData = updateAnimData(scarfyData, dT, 5);
		}

		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i] = updateAnimData(nebulae[i], dT, 7);
		}

		for (AnimData nebula : nebulae)
		{
			float pad{ 40 };
			Rectangle nebRec{
					nebula.pos.x + pad,
					nebula.pos.y + pad,
					nebula.rec.width - 2 * pad,
					nebula.rec.height - 2 * pad
			};
			Rectangle scarfyRec{
					scarfyData.pos.x,
					scarfyData.pos.y,
					scarfyData.rec.width,
					scarfyData.rec.height
			};
			if (CheckCollisionRecs(nebRec, scarfyRec))
			{
				collision = true;
			}
		}

		if (collision)
		{
			// lose
			DrawText("GAME OVER", windowDimentions[0]/2-120, windowDimentions[1]/2-20, 40, WHITE);
		}
		else if (scarfyData.pos.x >= finishLine)
		{
			DrawText("YOU WIN!", windowDimentions[0] / 2 - 100, windowDimentions[1] / 2 - 20, 40, GREEN);
		}
		else
		{
			for (int i = 0; i < sizeOfNebulae; i++)
			{
				// draw nebula
				DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
			}

			// draw scarfy
			DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
		}

		// stop drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	CloseWindow();
}


