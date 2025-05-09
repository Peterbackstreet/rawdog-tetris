#include "include/raylib.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

vector<vector<bool>> board(10,vector<bool>(22 , 0));

int grid_size = 40;
int offset_side = 280;
int offset_top = 120;

Color BG = Color{187, 237, 221, 255};

class Piece {
  public:
    char pieceName;
    Color color;
    unordered_map<char, vector<Vector2>> parts = {
      {'I', { {0,3}, {0,4}, {0,5}, {0,6} }},
      //    0123456789
      // 0: ...####...
      // 1: ..........
      // 2: ..........
      // 3: ..........
      {'O', { {0,4}, {0,5}, {1,4}, {1,5} }},
      //    0123456789
      // 0: ....##....
      // 1: ....##....
      // 2: ..........
      // 3: ..........
      {'S', { {0,4}, {0,5}, {1,3}, {1,4} }},
      //    0123456789
      // 0: ....##....
      // 1: ...##.....
      // 2: ..........
      // 3: ..........
      {'Z', { {0,3}, {0,4}, {1,4}, {1,5} }},
      //    0123456789
      // 0: ...##.....
      // 1: ....##....
      // 2: ..........
      // 3: ..........
      {'J', { {0,3}, {1,3}, {1,4}, {1,5} }},
      //    0123456789
      // 0: ...#......
      // 1: ...###....
      // 2: ..........
      // 3: ..........
      {'L', { {0,5}, {1,3}, {1,4}, {1,5}}}
      //    0123456789
      // 0: .....#....
      // 1: ...###....
      // 2: ..........
      // 3: ..........
    };

    unordered_map<char,Color> colors = { {'I', {120, 200, 200, 255}},
                                         {'O', {240, 230, 90, 255}},
                                         {'S', {250, 90, 120, 255}},
                                         {'Z', {100, 255, 160, 255}},
                                         {'J', {34, 109, 230, 255}},
                                         {'L', {255, 158, 84, 255}}
    };


    Piece(char pieceName) {
      this->pieceName = pieceName;
      color = colors[pieceName];
    }

    void draw() {
      for (Vector2 part : parts[pieceName]) {
        DrawRectangle(part.y * grid_size + offset_side/2, part.x * grid_size + offset_top/2, grid_size, grid_size, color);
      }
    }
};


class Game {
  public:
    void update() {

    }
};

int main() {
  srand(time(NULL));//this is important when doing random (the seed will be the same no matter what if its pre-compiled)
                    //
  vector<char> piece_type = {'I','O','S','Z','J','L'};
  float grid_width = 10 * grid_size;
  float grid_height = 20 * grid_size;

  InitWindow(grid_width + offset_side, grid_height + offset_top, "hello raylib");
  SetTargetFPS(60);

  int ran = rand() % 6;
  char type = piece_type[ran];

  Piece piece(type);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG);
    DrawRectangle(offset_side/2, offset_top/2,  grid_width,  grid_height, WHITE);
    piece.draw();
    DrawFPS(10,10);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
