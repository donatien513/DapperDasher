#include "raylib.h"
#include<cstdio>

struct Background {
  Texture2D *texture;
  Vector2 position;
  Vector2 velocity;
  float rotation;
  float scale;
  float updateTime;
  float runningTime;
};
struct SpriteAnimation {
  int spriteCount;
  int currentSpriteFrameIndex;
  Texture2D *sprite;
  Rectangle spriteSlice;
  Vector2 position;
  Vector2 velocity;
  float updateTime;
  float runningTime;
};

void loopSpriteInTime(SpriteAnimation *element, float *deltaTime) {
  (*element).runningTime += *deltaTime;
  if ((*element).runningTime >= (*element).updateTime) {
    (*element).runningTime = 0.0;
    (*element).spriteSlice.x = (*element).currentSpriteFrameIndex * (*element).spriteSlice.width;
    (*element).currentSpriteFrameIndex++;
    if ((*element).currentSpriteFrameIndex > (*element).spriteCount - 1) {
      (*element).currentSpriteFrameIndex = 0;
    }
  }
}

int main() {
  const int WINDOW_WIDTH{800};
  const int WINDOW_HEIGHT{600}; 

  const int gravity{2'000};
  const int scarfyJumpVelocity{-1000};

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dapper dasher");

  Texture2D scarfySprite{LoadTexture("textures/scarfy.png")};
  Texture2D nebulaSprite{LoadTexture("textures/12_nebula_spritesheet.png")};
  Texture2D farBuildings{LoadTexture("textures/far-buildings.png")};

  /**
   * @brief B A C K G R O U N D
   * 
   */
  int farBuildingsBackgroundRepeatCount{2};
  Background farBuildingsBackground[farBuildingsBackgroundRepeatCount]{};
  for (int i = 0; i < farBuildingsBackgroundRepeatCount; i++){
    farBuildingsBackground[i].texture = &farBuildings;
    farBuildingsBackground[i].velocity.x = -80.0;
    farBuildingsBackground[i].scale = 3.2;
    farBuildingsBackground[i].rotation = 0.0;
    farBuildingsBackground[i].position.x = i * ((*farBuildingsBackground[i].texture).width * farBuildingsBackground[i].scale);
    farBuildingsBackground[i].position.y = 0;
  }
  

  /**
   * @brief N E B U L A
   * 
   */
  int numberOfNubela{25};
  SpriteAnimation nebulae[numberOfNubela]{};
  for (int i = 0; i < numberOfNubela; i++) {
    nebulae[i].sprite = &nebulaSprite;
    nebulae[i].currentSpriteFrameIndex = 0;
    nebulae[i].spriteCount = 8;
    nebulae[i].spriteSlice.x = 0.0;
    nebulae[i].spriteSlice.y = 0.0;
    nebulae[i].spriteSlice.width = (*nebulae[i].sprite).width / nebulae[i].spriteCount;
    nebulae[i].spriteSlice.height = (*nebulae[i].sprite).height / nebulae[i].spriteCount;
    nebulae[i].position.x = float(WINDOW_WIDTH + 100 * i);
    nebulae[i].position.y = float(WINDOW_HEIGHT - nebulae[i].spriteSlice.height);
    nebulae[i].velocity.x = -200.0;
    nebulae[i].velocity.y = 0;
    nebulae[i].updateTime = 1.0 / 12.0;
    nebulae[i].runningTime = 0.0;
  }

  /**
   * @brief S C A R F Y
   * 
   */
  SpriteAnimation scarfy;
  scarfy.sprite = &scarfySprite;
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

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float deltaTime{GetFrameTime()};

    BeginDrawing();
    ClearBackground(WHITE);

    /**
     * @brief D R A W   B A C K G R O U N D
     * 
     */
    for (size_t i = 0; i < farBuildingsBackgroundRepeatCount; i++) {
      farBuildingsBackground[i].position.x += farBuildingsBackground[i].velocity.x * deltaTime;
      DrawTextureEx(
        (*farBuildingsBackground[i].texture),
        farBuildingsBackground[i].position,
        farBuildingsBackground[i].rotation,
        farBuildingsBackground[i].scale,
        WHITE
      );
    }
    

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

    /**
     * @brief S C A R F Y   A N I M A T I O N
     * 
     */
    scarfy.position.y += scarfy.velocity.y * deltaTime;
    if (scarfyIsInTheAir) {
      if (scarfy.spriteSlice.x != 0) {
        scarfy.spriteSlice.x = 0;
      }
    } else {
      loopSpriteInTime(&scarfy, &deltaTime);
    }

    /**
     * @brief N E B U L A   A N I M A T I O N    &     D R A W   N E B U L A
     * 
     */
    for (int i = 0; i < numberOfNubela; i++) {
      nebulae[i].position.x += nebulae[i].velocity.x * deltaTime;
      nebulae[i].runningTime += deltaTime;
      if (nebulae[i].runningTime >= nebulae[i].updateTime) {
        loopSpriteInTime(&nebulae[i], &deltaTime);
      }
      DrawTextureRec((*nebulae[i].sprite), nebulae[i].spriteSlice, nebulae[i].position, WHITE);
    }

    /**
     * @brief D R A W   S C A R F Y
     * 
     */
    DrawTextureRec((*scarfy.sprite), scarfy.spriteSlice, scarfy.position, WHITE);
    EndDrawing();
  }

  CloseWindow();

  UnloadTexture(scarfySprite);
  UnloadTexture(nebulaSprite);

  UnloadTexture(farBuildings);
  return 0;
}
