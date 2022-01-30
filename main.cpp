#include "raylib.h"
#include<cstdio>

struct EnvironmentScene {
  Texture2D *texture;
  Vector2 position;
  Vector2 velocity;
  Rectangle boundary;
  float rotation;
  float scale;
  float finalSize;
};
struct SpriteAnimation {
  int spriteCount;
  int currentSpriteFrameIndex;
  Texture2D *sprite;
  Rectangle spriteSlice;
  Rectangle boundary;
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

  Texture2D backgroundTexture{LoadTexture("textures/far-buildings.png")};
  Texture2D midgroundTexture{LoadTexture("textures/back-buildings.png")};
  Texture2D foregroundTexture{LoadTexture("textures/foreground.png")};

  /**
   * @brief B A C K G R O U N D
   * 
   */
  int backgroundRepeatCount{2};
  EnvironmentScene background[backgroundRepeatCount]{};
  for (int i = 0; i < backgroundRepeatCount; i++){
    background[i].texture = &backgroundTexture;
    background[i].velocity.x = -40.0;
    background[i].scale = 3.2;
    background[i].rotation = 0.0;
    background[i].finalSize = ((*background[i].texture).width * background[i].scale);
    background[i].position.x = i * background[i].finalSize;
    background[i].position.y = 0;
  }
  
  /**
   * @brief M I D G R O U N D
   * 
   */
  int midgroundRepeatCount{2};
  EnvironmentScene midground[midgroundRepeatCount]{};
  for (int i = 0; i < midgroundRepeatCount; i++){
    midground[i].texture = &midgroundTexture;
    midground[i].velocity.x = -80.0;
    midground[i].scale = 3.2;
    midground[i].rotation = 0.0;
    midground[i].finalSize = ((*midground[i].texture).width * midground[i].scale);
    midground[i].position.x = i * midground[i].finalSize;
    midground[i].position.y = 0;
  }

  /**
   * @brief F O R E G R O U N D
   * 
   */
  int foregroundRepeatCount{2};
  EnvironmentScene foreground[foregroundRepeatCount]{};
  for (int i = 0; i < foregroundRepeatCount; i++){
    foreground[i].texture = &foregroundTexture;
    foreground[i].velocity.x = -160.0;
    foreground[i].scale = 3.2;
    foreground[i].rotation = 0.0;
    foreground[i].finalSize = ((*foreground[i].texture).width * foreground[i].scale);
    foreground[i].position.x = i * foreground[i].finalSize;
    foreground[i].position.y = 40;
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
    nebulae[i].boundary.x = nebulae[i].position.x;
    nebulae[i].boundary.y = nebulae[i].position.y;
    nebulae[i].boundary.width = nebulae[i].spriteSlice.width;
    nebulae[i].boundary.height = nebulae[i].spriteSlice.height;
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
  scarfy.boundary.x = scarfy.position.x;
  scarfy.boundary.y = scarfy.position.y;
  scarfy.boundary.width = scarfy.spriteSlice.width;
  scarfy.boundary.height = scarfy.spriteSlice.height;
  bool scarfyIsInTheAir{false};

  const int scarfyGroundPosition{WINDOW_HEIGHT - int(scarfy.spriteSlice.height)};
  float finishLine{nebulae[numberOfNubela - 1].position.x};
  bool collision{false};

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float deltaTime{GetFrameTime()};
    finishLine += nebulae[numberOfNubela - 1].position.x;

    BeginDrawing();
    ClearBackground(WHITE);

    /**
     * @brief D R A W   B A C K G R O U N D
     * 
     */
    for (int i = 0; i < backgroundRepeatCount; i++) {
      background[i].position.x += background[i].velocity.x * deltaTime;
      if (background[i].position.x <= -(background[i].finalSize)) {
        background[i].position.x = background[i].finalSize - 2;
      }
      DrawTextureEx(
        (*background[i].texture),
        background[i].position,
        background[i].rotation,
        background[i].scale,
        WHITE
      );
    }

    /**
     * @brief D R A W   M I D G R O U N D
     * 
     */
    for (int i = 0; i < midgroundRepeatCount; i++) {
      midground[i].position.x += midground[i].velocity.x * deltaTime;
      if (midground[i].position.x <= -(midground[i].finalSize)) {
        midground[i].position.x = midground[i].finalSize - 2;
      }
      DrawTextureEx(
        (*midground[i].texture),
        midground[i].position,
        midground[i].rotation,
        background[i].scale,
        WHITE
      );
    }

    /**
     * @brief D R A W   F O R E G R O U N D
     * 
     */
    for (int i = 0; i < foregroundRepeatCount; i++) {
      foreground[i].position.x += foreground[i].velocity.x * deltaTime;
      if (foreground[i].position.x <= -(foreground[i].finalSize)) {
        foreground[i].position.x = foreground[i].finalSize - 4;
      }
      DrawTextureEx(
        (*foreground[i].texture),
        foreground[i].position,
        foreground[i].rotation,
        background[i].scale,
        WHITE
      );
    }

    if (collision) {
      DrawText("Game over", (WINDOW_WIDTH / 3,14), WINDOW_HEIGHT / 2, 84, WHITE);
    } else {
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
      scarfy.boundary.y = scarfy.position.y;
      if (scarfyIsInTheAir) {
        if (scarfy.spriteSlice.x != 0) {
          scarfy.spriteSlice.x = 0;
        }
      } else {
        loopSpriteInTime(&scarfy, &deltaTime);
      }

      /**
       * @brief N E B U L A   A N I M A T I O N   &   D R A W   N E B U L A
       * 
       */
      for (int i = 0; i < numberOfNubela; i++) {
        nebulae[i].position.x += nebulae[i].velocity.x * deltaTime;
        nebulae[i].boundary.x = nebulae[i].position.x;
        nebulae[i].runningTime += deltaTime;
        if (CheckCollisionRecs(scarfy.boundary, nebulae[i].boundary)) {
          collision = true;
        }
        if (nebulae[i].runningTime >= nebulae[i].updateTime) {
          loopSpriteInTime(&nebulae[i], &deltaTime);
        }
        DrawTextureRec((*nebulae[i].sprite), nebulae[i].spriteSlice, nebulae[i].position, WHITE);
      }
      finishLine += nebulae[numberOfNubela - 1].velocity.x * deltaTime;

      /**
       * @brief D R A W   S C A R F Y
       * 
       */
      DrawTextureRec((*scarfy.sprite), scarfy.spriteSlice, scarfy.position, WHITE);
    }
    EndDrawing();
  }

  CloseWindow();

  UnloadTexture(scarfySprite);
  UnloadTexture(nebulaSprite);

  UnloadTexture(backgroundTexture);
  UnloadTexture(midgroundTexture);
  UnloadTexture(foregroundTexture);
  return 0;
}
