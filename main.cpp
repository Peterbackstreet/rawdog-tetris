#include "include/raylib.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

vector<vector<Color>> board(22,vector<Color>(10 , WHITE));

int grid_size = 40;
int offset_side = 300;
int offset_top = 200;

double previousTick = 0;
double tickTime = 0.2;
double previousInputTick = 0;
double inputTime = 0.05;

unordered_map<char, vector<Vector2>> parts_template = {
  {'I', { {1,0}, {1,1}, {1,2}, {1,3} }},
  {'O', { {0,1}, {0,2}, {1,1}, {1,2} }},
  {'S', { {0,1}, {0,2}, {1,0}, {1,1} }},
  {'Z', { {0,0}, {0,1}, {1,1}, {1,2} }},
  {'J', { {0,0}, {1,0}, {1,1}, {1,2} }},
  {'L', { {0,2}, {1,0}, {1,1}, {1,2} }},
  {'T', { {0,1}, {1,0}, {1,1}, {1,2} }}
};

unordered_map<char,Color> colors = { 
  {'I', {120, 200, 200, 255}},
  {'O', {240, 230, 90, 255}},
  {'S', {250, 90, 120, 255}},
  {'Z', {100, 255, 160, 255}},
  {'J', {34, 109, 230, 255}},
  {'L', {255, 158, 84, 255}},
  {'T', {214, 13, 214, 255}}
};

bool compareColor(Color a, Color b) {
  return a.r == b.r && a.g == b.g && a.b == b.b;
}

bool checkGameTick() {
  double gameTime = GetTime();
  if (gameTime - previousTick >= tickTime) {
    previousTick = gameTime;
    return true;
  }
  return false;
}

bool checkInputTick() {
  double gameTime = GetTime();
  if (gameTime - previousInputTick >= inputTime) {
    previousInputTick = gameTime;
    return true;
  }
  return false;
}

Color BG = Color{187, 237, 221, 255};

class Piece {
  public:
    vector<char> piece_type = {'I','O','S','Z','J','L','T'};
    vector<Vector2> parts;
    char pieceName;
    Color color;
    int depth;
    int side;
    int stallingTick;
    bool onTop = false;

    Piece() {
      reset();
    }

    void reset() {
      int ran = rand() % 7;
      pieceName = piece_type[ran];
      parts = parts_template[pieceName];
      color = colors[pieceName];
      depth = 0;
      side = 3;
      stallingTick = 5;
      onTop = false;
    }

    void drawPiece() {
      for (Vector2 part : parts) {
        float horizontal = (part.y+side) * grid_size + (float)offset_side/2;
        float vertical = (part.x+depth - 2) * grid_size + (float)offset_top/2;
        DrawRectangle(horizontal, vertical, grid_size, grid_size, color);
      }
    }

    void paintBoard() {
      for (Vector2 part : parts) {
        board[part.x+depth][part.y+side] = color;
      }
    }

    void clearLine(int x) {
      if(x<2) return;
      for (int i=0; i<10 ; i++) {
        board[x][i] = board[x-1][i];
      }
      clearLine(x-1);
    }

    void checkLineClear() {
      set<int> height;
      for (Vector2 part : parts) {
        height.insert(part.x);
      }

      for (int x : height) {
        bool foundWhite = false;
        for (int i=0; i<10; i++) {
          if(compareColor(board[x+depth][i], WHITE)) {
            foundWhite = true;
            break;
          }
        }
        if(foundWhite) continue;
        clearLine(x+depth);
      }
    }

    //everytime dealing with the board array the x and y is flipped
    void fall() {
      for (Vector2 part : parts) {
        if (part.x+depth+1 > 21 || !compareColor(board[part.x+depth+1][part.y+side], WHITE)) {
          onTop = true;
          stallingTick--;
          if(stallingTick<=0) {
            paintBoard();
            checkLineClear();
            reset();
          }
          return;
        }
      }
      onTop = false;
      depth++;
    }

    void move(int direction) {
      for (Vector2 part : parts) {
        if (part.y+side+direction < 0 || part.y+side+direction > 9 || depth <= -1 || !compareColor(board[part.x+depth][part.y+side+direction], WHITE)) return;
      }
      side+=direction;
    }

    void spin() {
      if (pieceName == 'O') return;

      int dimension = (pieceName == 'I') ? 3 : 2;
      for (Vector2 part : parts) {
        int x = part.y+depth;
        int y = dimension-part.x+side;
        int deep = (x > 21) ? -1 : 0;
        int direction;
        if (y>=0 && y<10) direction = 0;
        else direction = (y<0) ? 1 : -1;

        for (Vector2 part : parts) {
          if (!compareColor(board[x+deep][y+direction], WHITE)) return;
        }
        if(y<0 || y>9) side += direction;
        if(x>21) depth -= 1;
      }

      for (Vector2 &part : parts) {
        int x = part.y;
        int y = dimension-part.x;
        part.x = x;
        part.y = y;
      }
    }
};



class Game {
  public:
    void getInputOnTick(Piece& piece) {
      if(IsKeyDown(KEY_DOWN) && !piece.onTop) tickTime = 0.05;
      else tickTime = 0.2;
      if(IsKeyDown(KEY_LEFT)) piece.move(-1);
      if(IsKeyDown(KEY_RIGHT)) piece.move(1);
    }

    void getInput(Piece &piece) {
      if(IsKeyPressed(KEY_UP)) piece.spin();
    }

    void makePieceFall(Piece& piece) {
      piece.fall();
    }
    void update(Piece& piece) {
      if (checkInputTick()) getInputOnTick(piece);
      if (checkGameTick()) makePieceFall(piece);
      getInput(piece);
    }

    void drawBoard() {
      for (int i = 2; i < 22; i++) {
        for (int j=0; j < 10; j++) {
          DrawRectangle(j * grid_size + offset_side/2, (i-2) * grid_size + offset_top/2, grid_size, grid_size, board[i][j]);
        }
      }
    }


    void draw(Piece piece) {
      drawBoard();
      piece.drawPiece();
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
