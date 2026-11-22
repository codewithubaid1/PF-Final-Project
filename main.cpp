#include <iostream>
using namespace std;

const int ROWS = 20;
const int COLS = 10;

int board[ROWS][COLS];
int shapes[4][4][4]{
  {
    {1,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
  },
  {
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
  },
  {
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
  },
  
};

int shapeHeight[3] = {2,1,2};
int shapeHeight[3] = {2,4,3};
int currentShape; 
int nextShape;
int blockX;
int blockY;
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
  for (int i = 1; i <= COLS*2; i++)
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
      if (board[i][j] == 1)
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
  for (int i = 1; i <= COLS*2; i++)
  {
    cout << "-";
  }
  cout << "+";
}
int main()
{

  InitializeBoard();
  DrawBoard();
  return 0;
}
