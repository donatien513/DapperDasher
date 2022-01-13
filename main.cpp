#include "raylib.h"

int main() {
  const int WINDOW_WIDTH{800};
  const int WINDOW_HEIGHT{600};

  const int gravity{1};

  const int characterWidth{50};
  const int characterHeight{80};

  int characterYVelocity{0};
  int characterXpos{WINDOW_WIDTH / 2};
  int characterYpos{WINDOW_HEIGHT - characterHeight};

  const int characterGroundPos{WINDOW_HEIGHT - characterHeight};


  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dapper dasher");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    if (characterYpos >= characterGroundPos) {
      characterYVelocity = 0;
    } else {
      characterYVelocity += gravity;
    }

    if (IsKeyPressed(KEY_SPACE)) {
      characterYVelocity -= 10;
    }

    characterYpos += characterYVelocity;

    DrawRectangle(characterXpos, characterYpos, characterWidth, characterHeight, BLUE);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
