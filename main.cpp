#include "raylib.h"

int main() {
  const int WINDOW_WIDTH{800};
  const int WINDOW_HEIGHT{600};

  const int gravity{2'000};
  const int characterJumpVelocity{-1000};

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dapper dasher");

  Texture2D characterSprite = LoadTexture("textures/scarfy.png");
  Rectangle characterCurrentRec;
  Vector2 characterPos;

  const int characterWidth{characterSprite.width / 6};
  const int characterHeight{characterSprite.height};

  characterCurrentRec.width = characterWidth;
  characterCurrentRec.height = characterHeight;
  characterCurrentRec.x = 0;
  characterCurrentRec.y = 0;

  int characterYVelocity{0};
  characterPos.x = WINDOW_WIDTH / 2 - characterWidth / 2;
  characterPos.y = WINDOW_HEIGHT - characterHeight;

  bool characterIsInAir{false};

  const int characterGroundPos{WINDOW_HEIGHT - characterHeight};
  int characterCurrentFrame{};
  float updateTime{1.0 / 10.0};
  float runningTime{};

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    const float deltaTime{GetFrameTime()};
    runningTime += deltaTime;

    BeginDrawing();
    ClearBackground(WHITE);

    if (characterPos.y >= characterGroundPos) {
      characterIsInAir = false;
      characterYVelocity = 0;
    } else {
      characterIsInAir = true;
      characterYVelocity += gravity * deltaTime;
    }

    if (IsKeyPressed(KEY_SPACE) && !characterIsInAir) {
      characterYVelocity += characterJumpVelocity;
    }

    characterPos.y += characterYVelocity * deltaTime;

    if (characterCurrentFrame > 5) {
      characterCurrentFrame = 0;
    }

    if (runningTime >= updateTime) {
      runningTime = 0.0;
      characterCurrentRec.x = characterCurrentFrame * characterCurrentRec.width;
      characterCurrentFrame++;
    }

    DrawTextureRec(characterSprite, characterCurrentRec, characterPos, WHITE);

    EndDrawing();
  }
  CloseWindow();
  UnloadTexture(characterSprite);
  return 0;
}
