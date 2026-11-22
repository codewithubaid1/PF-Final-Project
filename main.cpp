#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <string>
#include <iomanip>
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
void checkCompletedPatterns();
int calculateScore(int, int);
void gotoXY(int, int);
void hideCursor();
void updateLevel();
int getFallInterval();
const int ROWS = 20;
const int COLS = 10;
bool isPatternFull(int, int, int, int, bool[][COLS]);
void markPattern(int, int, int, int, bool[][COLS]);
void applyGravity(bool marked[][COLS]);
void DrawStatistics();

bool isPaused = false;
bool quitRequested = false;
string playerName;
int level = 1;
int highestScore = 0;
string highestScorePlayer;
int lastScore = 0;
string lastScorePlayer;
int score = 0;
int activeShape[4][4];
int board[ROWS][COLS];
int shapes[5][4][4]{
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
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
    },

};

int shapeHeight[5] = {2, 1, 2, 3, 3};
int shapeWidth[5] = {2, 4, 3, 3, 3};

int currentShape;
int nextShape;
int blockX;
int blockY = 0;

int main()
{

  cout << "Enter your name: ";
  getline(cin, playerName);
  srand(time(0));
  hideCursor();
  currentShape = rand() % 5;
  nextShape = rand() % 5;
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
  gotoXY(0, 0);
  cout << "          FALLING BLOCKS GAME";

  // Upper boundary
  gotoXY(0, 2);
  cout << "+";
  for (int i = 0; i < COLS * 2; i++)
    cout << "-";
  cout << "+";

  // Inner board
  for (int i = 0; i < ROWS; i++)
  {
    gotoXY(0, 3 + i);
    cout << "|";

    for (int j = 0; j < COLS; j++)
    {
      if (board[i][j] == 1 || isCurrentBlockCell(i, j))
        cout << " #";
      else
        cout << " .";
    }

    cout << "|";
  }

  // Lower boundary
  gotoXY(0, 23);
  cout << "+";
  for (int i = 0; i < COLS * 2; i++)
    cout << "-";
  cout << "+";

  DrawStatistics();
  DrawLegend();
  DrawNextShape();
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
    if (key == 'p' || key == 'P')
    {
      isPaused = !isPaused;
      return;
    }

    if (key == 'q' || key == 'Q')
    {
      quitRequested = true;
      return;
    }

    if (isPaused)
      return;
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
    else if (key == 'w' || key == 'W')
    {
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
      if (activeShape[r][c] == 1)
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
  nextShape = rand() % 5;
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

  bool running = true;

  while (running)
  {
    updateLevel();
    handleInput();
    DWORD currentTime = GetTickCount();

    if (quitRequested)
    {
      running = false;
      break;
    }

    if (isPaused)
    {
      lastFall = GetTickCount();

      DrawBoard();
      Sleep(30);

      continue;
    }

    const DWORD fallInterval = getFallInterval();

    if (currentTime - lastFall >= fallInterval)
    {
      if (canMoveTo(blockX, blockY + 1))
      {
        blockY++;
      }
      else
      {
        lockBlock();
        checkCompletedPatterns();

        if (!spawnNextBlock())
        {
          running = false;
        }
      }
      lastFall = currentTime;
    }

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

void rotateCurrentBllock()
{

  int rotated[4][4] = {};
  int normalized[4][4] = {};

  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      rotated[c][3 - r] = activeShape[r][c];
    }
  }

  int minR = 4, minC = 4;

  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      if (rotated[r][c] == 1)
      {
        if (r < minR)
        {
          minR = r;
        }
        if (c < minC)
        {
          minC = c;
        }
      }
    }
  }

  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      if (rotated[r][c] == 1)
      {
        normalized[r - minR][c - minC] = 1;
      }
    }
  }

  if (canPlaceShape(blockX, blockY, normalized))
  {
    for (int r = 0; r < 4; r++)
    {
      for (int c = 0; c < 4; c++)
      {
        activeShape[r][c] = normalized[r][c];
      }
    }
  }
}

void checkCompletedPatterns()
{

  bool marked[ROWS][COLS] = {};
  bool isPatternDetected = false;

  // row detection
  for (int r = 0; r < ROWS; r++)
  {
    bool complete = true;
    for (int c = 0; c < COLS; c++)
    {
      if (board[r][c] == 0)
      {
        complete = false;
        break;
      }
    }

    if (complete)
    {
      score += calculateScore(1, COLS);
      isPatternDetected = true;
      for (int c = 0; c < COLS; c++)
      {
        marked[r][c] = true;
      }
    }
  }

  for (int area = ROWS * COLS; area >= 6; area--)
  {
    for (int height = 2; height <= ROWS; height++)
    {
      for (int width = 2; width <= COLS; width++)
      {
        if (height * width != area)
        {
          continue;
        }

        if (height == 2 && width == 2)
        {
          continue;
        }

        for (int r = 0; r <= ROWS - height; r++)
        {
          for (int c = 0; c <= COLS - width; c++)
          {
            if (isPatternFull(r, c, height, width, marked))
            {
              score += calculateScore(height, width);
              isPatternDetected = true;
              markPattern(r, c, height, width, marked);
            }
          }
        }
      }
    }
  }

  if (isPatternDetected)
  {
    applyGravity(marked);
  }
}

void applyGravity(bool marked[][COLS])
{
  for (int c = 0; c < COLS; c++)
  {
    int writeRow = ROWS - 1;
    for (int r = ROWS - 1; r >= 0; r--)
    {
      if (!marked[r][c] && board[r][c] == 1)
      {
        board[writeRow][c] = 1;
        writeRow--;
      }
    }

    for (int r = writeRow; r >= 0; r--)
    {
      board[r][c] = 0;
    }
  }
}

void markPattern(int startR, int startC, int height, int width, bool marked[][COLS])
{
  for (int r = 0; r < height; r++)
  {
    for (int c = 0; c < width; c++)
    {
      marked[startR + r][startC + c] = true;
    }
  }
}

bool isPatternFull(int startR, int startC, int height, int width, bool marked[][COLS])
{
  for (int r = 0; r < height; r++)
  {
    for (int c = 0; c < width; c++)
    {
      if (board[startR + r][startC + c] == 0 || marked[startR + r][startC + c])
      {
        return false;
      }
    }
  }
  return true;
}

int calculateScore(int height, int width)
{

  if (height == 1 && width == COLS)
  {
    return 100;
  }
  else if (height == width)
  {
    return (100 * height) - 150;
  }
  return height * width * 20;
}

void gotoXY(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void DrawStatistics()
{
  gotoXY(28, 2);
  cout << "------ GAME INFORMATION ------";

  gotoXY(28, 4);
  cout << "Player Name : " << setw(20) << playerName << "          ";

  gotoXY(28, 5);
  cout << "Score       : " << setw(10) << score << "          ";

  gotoXY(28, 6);
  cout << "Level       : " << level;

  gotoXY(28, 7);

  cout << "Speed       : ";

  if (level == 1)
    cout << "Slow  ";
  else if (level == 2)
    cout << "Medium";
  else
    cout << "Fast  ";

  gotoXY(28, 10);
  cout << "-------- HIGHEST SCORE -------";

  gotoXY(28, 12);
  cout << "Highest Score : " << setw(10) << highestScore << "          ";

  gotoXY(28, 13);
  cout << "Player Name   : " << setw(20) << highestScorePlayer << "          ";

  gotoXY(28, 14);
  cout << "-------- CONTROLS ------------";

  gotoXY(28, 16);
  cout << "A : Move Left";

  gotoXY(28, 17);
  cout << "D : Move Right";

  gotoXY(28, 18);
  cout << "S : Move Down Faster";

  gotoXY(28, 19);
  cout << "W : Rotate";

  gotoXY(28, 20);
  cout << "Q : Quit";
}

void hideCursor()
{
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursorInfo;
  cursorInfo.dwSize = 1;
  cursorInfo.bVisible = FALSE;

  SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void updateLevel()
{
  if (score >= 1000)
    level = 3;
  else if (score >= 300)
    level = 2;
  else
    level = 1;
}

int getFallInterval()
{
  if (level == 1)
    return 700;
  else if (level == 2)
    return 450;
  else
    return 250;
}

void DrawLegend()
{
  gotoXY(28, 23);
  cout << "-------- BLOCK LEGEND --------";

  gotoXY(28, 25);
  cout << "O : Square";

  gotoXY(28, 26);
  cout << "I : Line";

  gotoXY(28, 27);
  cout << "Z : Zigzag";

  gotoXY(28, 28);
  cout << "T : T-Shape";

  gotoXY(28, 29);
  cout << "L : L-Shape";
}

void DrawNextShape()
{
  gotoXY(28, 32);
  cout << "-------- NEXT BLOCK --------";

  for (int r = 0; r < 4; r++)
  {
    gotoXY(32, 34 + r);

    for (int c = 0; c < 4; c++)
    {
      if (shapes[nextShape][r][c] == 1)
        cout << "[]";
      else
        cout << "  ";
    }
  }
}