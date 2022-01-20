#include "raylib.h"

struct Element {
  int spriteCount;
  int currentSpriteFrameIndex;
  Texture2D *sprite;
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

  Texture2D scarfySprite{LoadTexture("textures/scarfy.png")};
  Texture2D nebulaSprite{LoadTexture("textures/12_nebula_spritesheet.png")};

  /**
   * @brief N E B U L A
   * 
   */
  Element nebula_1;
  nebula_1.sprite = &nebulaSprite;
  nebula_1.currentSpriteFrameIndex = 0;
  nebula_1.spriteCount = 8;
  nebula_1.spriteSlice.x = 0.0;
  nebula_1.spriteSlice.y = 0.0;
  nebula_1.spriteSlice.width = (*nebula_1.sprite).width / nebula_1.spriteCount;
  nebula_1.spriteSlice.height = (*nebula_1.sprite).height / nebula_1.spriteCount;
  nebula_1.position.x = float(WINDOW_WIDTH - 200);
  nebula_1.position.y = float(WINDOW_HEIGHT - nebula_1.spriteSlice.height);
  nebula_1.velocity.x = -200.0;
  nebula_1.velocity.y = 0;
  nebula_1.updateTime = 1.0 / 12.0;
  nebula_1.runningTime = 0.0;

  /**
   * @brief S C A R F Y
   * 
   */
  Element scarfy;
  scarfy.sprite =&scarfySprite;
  scarfy.currentSpriteFrameIndex = 0;
  scarfy.spriteCount = 6;
  scarfy.spriteSlice.x = 0.0;
  scarfy.spriteSlice.y = 0.0;
  scarfy.spriteSlice.width = (*scarfy.sprite).width / scarfy.spriteCount;
  scarfy.spriteSlice.height = (*scarfy.sprite).height;
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
    nebula_1.position.x += nebula_1.velocity.x * deltaTime;

    /**
     * @brief N E B U L A   A N I M A T I O N
     * 
     */
    nebula_1.runningTime += deltaTime;
    if (nebula_1.runningTime >= nebula_1.updateTime) {
      nebula_1.runningTime = 0.0;
      nebula_1.spriteSlice.x = nebula_1.currentSpriteFrameIndex * nebula_1.spriteSlice.width;
      nebula_1.currentSpriteFrameIndex++;
      if (nebula_1.currentSpriteFrameIndex > nebula_1.spriteCount - 1) {
        nebula_1.currentSpriteFrameIndex = 0;
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
    DrawTextureRec((*nebula_1.sprite), nebula_1.spriteSlice, nebula_1.position, WHITE);

    /**
     * @brief D R A W   S C A R F Y
     * 
     */
    DrawTextureRec((*scarfy.sprite), scarfy.spriteSlice, scarfy.position, WHITE);
    EndDrawing();
  }

  CloseWindow();

  UnloadTexture((*scarfy.sprite));
  UnloadTexture((*nebula_1.sprite));
  return 0;
}
