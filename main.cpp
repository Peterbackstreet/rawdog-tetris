#include "include/raylib.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

vector<vector<Color>> board(10,vector<Color>(22 , WHITE));

int grid_size = 40;
int offset_side = 300;
int offset_top = 200;

double previousTick = 0;
double tickTime = 1;

unordered_map<char, vector<Vector2>> parts = {
  {'I', { {1,3}, {1,4}, {1,5}, {1,6} }},
  //    0123456789
  // 0: ..........
  // 1: ...####...
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

unordered_map<char,Color> colors = { 
  {'I', {120, 200, 200, 255}},
  {'O', {240, 230, 90, 255}},
  {'S', {250, 90, 120, 255}},
  {'Z', {100, 255, 160, 255}},
  {'J', {34, 109, 230, 255}},
  {'L', {255, 158, 84, 255}}
};

bool checkGameTick() {
  double gameTime = GetTime();
  if (gameTime - previousTick >= tickTime) {
    previousTick = gameTime;
    return true;
  }
  return false;
}

Color BG = Color{187, 237, 221, 255};

class Piece {
  public:
    vector<char> piece_type = {'I','O','S','Z','J','L'};
    char pieceName;
    Color color;
    int depth = 0;
    int side = 0;

    Piece() {
      reset();
    }

    void reset() {
      int ran = rand() % 6;
      this->pieceName = piece_type[ran];
      color = colors[pieceName];
      depth = 0;
      side = 0;
      for (Vector2 part : parts[pieceName]) {
      }
    }

    void drawPiece() {
      for (Vector2 part : parts[pieceName]) {
        float horizontal = (part.y+side) * grid_size + (float)offset_side/2;
        float vertical = (part.x+depth - 2) * grid_size + (float)offset_top/2;
        DrawRectangle(horizontal, vertical, grid_size, grid_size, color);
      }
    }

    void drawBoard() {
      for (Vector2 part : parts[pieceName]) {
        board[part.y+side][part.x+depth-1] = color;
      }
      reset();
    }

    void draw() {
      for (Vector2 part : parts[pieceName]) {
        float horizontal = (part.y+side) * grid_size + (float)offset_side/2;
        float vertical = (part.x+depth - 2) * grid_size + (float)offset_top/2;
        if (part.x+depth > 21) {
          drawBoard();
          return;
        }
      }
      drawPiece();
    }
};

void getInput() {
  if(IsKeyPressed(KEY_DOWN)) tickTime = 0.1;
}

class Game {
  public:

    void makePieceFall(Piece& piece) {
      if(piece.depth <21) piece.depth++;
      else piece.reset();
    }
    void update(Piece& piece) {
      getInput();
      if (checkGameTick()) makePieceFall(piece);
    }

    void drawBoard() {
      for (int i = 2; i < 22; i++) {
        for (int j=0; j < 10; j++) {
          DrawRectangle(j * grid_size + offset_side/2, (i-2) * grid_size + offset_top/2, grid_size, grid_size, board[j][i]);
        }
      }
    }

    void draw(Piece piece) {
      drawBoard();
      piece.draw();
    }
};

int main() {
  srand(time(NULL));//this is important when doing random (the seed will be the same no matter what if its pre-compiled)

  float grid_width = 10 * grid_size;
  float grid_height = 20 * grid_size;

  InitWindow(grid_width + offset_side, grid_height + offset_top, "hello raylib");
  SetTargetFPS(60);


  Game game;
  Piece piece;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG);
    game.update(piece);
    game.draw(piece);
    DrawFPS(10,10);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
