#include "raylib.h"

struct Element {
  int spriteCount;
  int currentSpriteFrameIndex;
  Texture2D sprite;
  Rectangle spriteSlice;
  Vector2 position;
  Vector2 velocity;
  float updateTime;
  float runningTime;
};

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
  const int numsOfNebulaInX = 8;
  const int numsOfNebulaInY = 7;
  const Texture2D nebulaSprite = LoadTexture("textures/12_nebula_spritesheet.png");
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
  const Vector2 nebulaVelocity{
    -200.0,
    0
  };
  Vector2 nebulaCurrentFrame{
    0.0,
    0.0
  };
  const float nebulaUpdateTime{1.0 / 3.0};
  float nebulaRunningTime{};

  /**
   * @brief S C A R F Y
   * 
   */
  Element scarfy;
  scarfy.sprite = LoadTexture("textures/scarfy.png");
  scarfy.currentSpriteFrameIndex = 0;
  scarfy.spriteCount = 6;
  scarfy.spriteSlice.x = 0.0;
  scarfy.spriteSlice.y = 0.0;
  scarfy.spriteSlice.width = scarfy.sprite.width / scarfy.spriteCount;
  scarfy.spriteSlice.height = scarfy.sprite.height;
  scarfy.position.x = float(WINDOW_WIDTH / 2 - scarfy.spriteSlice.width / 2);
  scarfy.position.y = float(WINDOW_HEIGHT - scarfy.spriteSlice.height);
  scarfy.velocity.x = 0;
  scarfy.velocity.y = 0;
  scarfy.updateTime = 1.0 / 12.0;
  scarfy.runningTime = 0.0;
  bool scarfyIsInTheAir{false};

  const int scarfyGroundPosition{WINDOW_HEIGHT - int(scarfy.spriteSlice.height)};
  const float scarfyUpdateTime{1.0 / 12.0};
  float scarfyRunningTime{};

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    const float deltaTime{GetFrameTime()};

    BeginDrawing();
    ClearBackground(WHITE);

    /**
     * @brief A P P L Y   G R A V I T Y
     * 
     */
    if (scarfy.position.y >= scarfyGroundPosition) {
      scarfyIsInTheAir = false;
      scarfy.velocity.y = 0;
    } else {
      scarfyIsInTheAir = true;
      scarfy.velocity.y += gravity * deltaTime;
    }

    /**
     * @brief J U M P
     * 
     */
    if (IsKeyPressed(KEY_SPACE) && !scarfyIsInTheAir) {
      scarfy.velocity.y += scarfyJumpVelocity;
    }

    scarfy.position.y += scarfy.velocity.y * deltaTime;
    nebulaPosition.x += nebulaVelocity.x * deltaTime;

    /**
     * @brief N E B U L A   A N I M A T I O N
     * 
     */
    nebulaRunningTime += deltaTime;
    if (nebulaRunningTime >= nebulaUpdateTime) {
      nebulaRunningTime = 0.0;
      nebulaCurrentRec.x = nebulaCurrentFrame.x * nebulaCurrentRec.width;
      nebulaCurrentFrame.x++;
      if (nebulaCurrentFrame.x > numsOfNebulaInX - 1) {
        nebulaCurrentFrame.x = 0;
        nebulaCurrentFrame.y++;
        if (nebulaCurrentFrame.y > numsOfNebulaInY - 1) {
          nebulaCurrentFrame.y = 0;
        }
      }

    }

    /**
     * @brief S C A R F Y   A N I M A T I O N
     * 
     */
    if (scarfyIsInTheAir) {
      if (scarfy.spriteSlice.x != 0) {
        scarfy.spriteSlice.x = 0;
      }
    } else {
      scarfyRunningTime += deltaTime;
      if (scarfyRunningTime >= scarfyUpdateTime) {
        scarfyRunningTime = 0.0;
        scarfy.spriteSlice.x = scarfy.currentSpriteFrameIndex * scarfy.spriteSlice.width;
        scarfy.currentSpriteFrameIndex++;
        if (scarfy.currentSpriteFrameIndex > scarfy.spriteCount - 1) {
          scarfy.currentSpriteFrameIndex = 0;
        }
      }
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
    DrawTextureRec(scarfy.sprite, scarfy.spriteSlice, scarfy.position, WHITE);
    EndDrawing();
  }

  CloseWindow();

  UnloadTexture(scarfy.sprite);
  UnloadTexture(nebulaSprite);
  return 0;
}
