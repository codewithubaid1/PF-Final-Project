#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;
bool isCurrentBlockCell(int, int);
bool canMoveTo(int, int);
void InitializeBoard();
void DrawBoard();
void handleInput();
void lockBlock();
bool spawnNextBlock();
void GameLoop();
void loadCurrentShape();
bool canPlaceShape(int, int, int[][4]);
void rotateCurrentBllock();

const int ROWS = 20;
const int COLS = 10;
int activeShape[4][4];
int board[ROWS][COLS];
int shapes[3][4][4]{
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },

};

int shapeHeight[3] = {2, 1, 2};
int shapeWidth[3] = {2, 4, 3};

int currentShape;
int nextShape;
int blockX;
int blockY = 0;

int main()
{

  srand(time(0));
  currentShape = rand() % 3;
  nextShape = rand() % 3;
  loadCurrentShape();
  blockX = (COLS - shapeWidth[currentShape]) / 2;
  blockY = 0;
  InitializeBoard();
  GameLoop();
  return 0;
}

void InitializeBoard()
{

  for (int i = 0; i < ROWS; i++)
  {
    for (int j = 0; j < COLS; j++)
    {
      board[i][j] = 0;
    }
  }
}

void DrawBoard()
{
  // Upper Boundary
  cout << "+";
  for (int i = 1; i <= COLS * 2; i++)
  {
    cout << "-";
  }
  cout << "+";
  cout << endl;

  // Inner Board
  for (int i = 0; i < ROWS; i++)
  {
    cout << "|";
    for (int j = 0; j < COLS; j++)
    {
      if (board[i][j] == 1 || isCurrentBlockCell(i, j))
      {
        cout << " #";
      }
      else
      {
        cout << " .";
      }
    }
    cout << "|";
    cout << endl;
  }

  // Lower Boundary
  cout << "+";
  for (int i = 1; i <= COLS * 2; i++)
  {
    cout << "-";
  }
  cout << "+";
}

bool isCurrentBlockCell(int row, int col)
{
  int r = row - blockY;
  int c = col - blockX;

  if (r < 0 || r >= 4 || c < 0 || c >= 4)
  {
    return false;
  }

  return activeShape[r][c] == 1;
}

bool canMoveTo(int newX, int newY)
{
  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      if (activeShape[r][c] == 1)
      {
        int row = newY + r;
        int col = newX + c;

        if (col < 0 || col >= COLS || row >= ROWS)
        {
          return false;
        }

        if (board[row][col] == 1)
        {
          return false;
        }
      }
    }
  }
  return true;
}

void handleInput()
{
  if (_kbhit())
  {
    char key = _getch();
    if (key == 'a' || key == 'A')
    {
      if (canMoveTo(blockX - 1, blockY))
      {
        blockX--;
      }
    }
    else if (key == 'd' || key == 'D')
    {
      if (canMoveTo(blockX + 1, blockY))
      {
        blockX++;
      }
    }

    else if (key == 's' || key == 'S')
    {
      if (canMoveTo(blockX, blockY + 1))
      {
        blockY++;
      }
    }
    else if(key == 'w' || key == 'W'){
      rotateCurrentBllock();
    }
  }
}

void lockBlock()
{

  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      if (shapes[currentShape][r][c] == 1)
      {
        int row = blockY + r;
        int col = blockX + c;

        board[row][col] = 1;
      }
    }
  }
}

bool spawnNextBlock()
{

  currentShape = nextShape;
  loadCurrentShape();
  nextShape = rand() % 3;
  blockY = 0;
  blockX = (COLS - shapeWidth[currentShape]) / 2;

  if (!canMoveTo(blockX, blockY))
  {

    return false;
  }

  return true;
}

void GameLoop()
{
  DWORD lastFall = GetTickCount();
  const DWORD fallInterval = 700;

  bool running = true;

  while (running)
  {
    handleInput();

    DWORD currentTime = GetTickCount();

    if (currentTime - lastFall >= fallInterval)
    {
      if (canMoveTo(blockX, blockY + 1))
      {
        blockY++;
      }
      else
      {
        lockBlock();

        if (!spawnNextBlock())
        {
          running = false;
        }
      }
      lastFall = currentTime;
    }
    system("cls");
    DrawBoard();
    Sleep(10);
  }

  cout << "\n\nGame Over";
}

void loadCurrentShape()
{

  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      activeShape[r][c] = shapes[currentShape][r][c];
    }
  }
}

bool canPlaceShape(int newX, int newY, int candidate[][4])
{
  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      if (candidate[r][c] == 1)
      {
        int row = newY + r;
        int col = newX + c;

        if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        {
          return false;
        }
        if (board[row][col] == 1)
        {
          return false;
        }
      }
    }
  }

  return true;
}

void rotateCurrentBllock(){

  int rotated[4][4] = {};
  int normalized[4][4] = {};

  for (int r = 0; r < 4; r++){
    for(int c = 0; c < 4; c++){
      rotated[c][c-r] = activeShape[r][c];
    }
  }

  int minR = 4, minC = 4;

  for(int r = 0; r < 4; r++){
    for(int c = 0; c < 4; c++){
      if(rotated[r][c] == 1){
        if(r < minR) {
          minR = r;
        }
        if(c < minC){
          minC = c;
        }
      }
    }
  }

  for(int r = 0; r < 4; r++){
    for(int c = 0; c < 4; c++){
      if(rotated[r][c] == 1){
        normalized[r-minR][c-minC] = 1;
      }
    }
  }

  if(canPlaceShape(blockX, blockY, normalized)){
    for(int r = 0; r < 4; r++){
      for(int c = 0; c < 4; c++){
        activeShape[r][c] = normalized[r][c];
      }
    }
  }
}