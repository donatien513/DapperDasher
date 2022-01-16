#include "raylib.h"

int main() {
  const int WINDOW_WIDTH{800};
  const int WINDOW_HEIGHT{600};

  const int gravity{2'000};
  const int scarfyJumpVelocity{-1000};

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dapper dasher");

  /**
   * @brief N E B U L A
   * 
   */
  Texture2D nebulaSprite = LoadTexture("textures/12_nebula_spritesheet.png");
  Rectangle nebulaCurrentRec{
    0.0,
    0.0,
    float(nebulaSprite.width / 8),
    float(nebulaSprite.height / 8)
  };
  Vector2 nebulaPosition{
    float(WINDOW_WIDTH),
    float(WINDOW_HEIGHT - nebulaCurrentRec.height)
  };
  const float nebulaVelocity{-600};

  /**
   * @brief S C A R F Y
   * 
   */
  Texture2D scarfySprite = LoadTexture("textures/scarfy.png");
  Rectangle scarfyCurrentRec{
    0.0,
    0.0, 
    float(scarfySprite.width / 6),
    float(scarfySprite.height)
  };
  Vector2 scarfyPosition{
    float(WINDOW_WIDTH / 2 - scarfyCurrentRec.width / 2),
    float(WINDOW_HEIGHT - scarfyCurrentRec.height)
  };

  int scarfyYVelocity{0};
  bool scarfyIsInAir{false};

  const int scarfyGroundPosition{WINDOW_HEIGHT - int(scarfyCurrentRec.height)};
  int scarfyCurrentFrame{};
  float updateTime{1.0 / 12.0};
  float runningTime{};

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    const float deltaTime{GetFrameTime()};
    runningTime += deltaTime;

    BeginDrawing();
    ClearBackground(WHITE);

    /**
     * @brief A P P L Y   G R A V I T Y
     * 
     */
    if (scarfyPosition.y >= scarfyGroundPosition) {
      scarfyIsInAir = false;
      scarfyYVelocity = 0;
    } else {
      scarfyIsInAir = true;
      scarfyYVelocity += gravity * deltaTime;
    }

    /**
     * @brief J U M P
     * 
     */
    if (IsKeyPressed(KEY_SPACE) && !scarfyIsInAir) {
      scarfyYVelocity += scarfyJumpVelocity;
    }

    scarfyPosition.y += scarfyYVelocity * deltaTime;
    nebulaPosition.x += nebulaVelocity * deltaTime;

    /**
     * @brief S C A R F Y   A N I M A T I O N
     * 
     */
    if (scarfyCurrentFrame > 5) {
      scarfyCurrentFrame = 0;
    }
    if (runningTime >= updateTime) {
      runningTime = 0.0;
      scarfyCurrentRec.x = scarfyCurrentFrame * scarfyCurrentRec.width;
      scarfyCurrentFrame++;
    }

    /**
     * @brief D R A W   N E B U L A
     * 
     */
    DrawTextureRec(nebulaSprite, nebulaCurrentRec, nebulaPosition, WHITE);

    /**
     * @brief D R A W   S C A R F Y
     * 
     */
    DrawTextureRec(scarfySprite, scarfyCurrentRec, scarfyPosition, WHITE);
    EndDrawing();
  }

  CloseWindow();

  UnloadTexture(scarfySprite);
  UnloadTexture(nebulaSprite);
  return 0;
}
