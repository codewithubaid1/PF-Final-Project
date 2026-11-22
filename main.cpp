#include <iostream>
using namespace std;

const int ROWS = 20;
const int COLS = 10;

int board[ROWS][COLS];

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
  for (int i = 1; i <= ROWS; i++)
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
  for (int i = 1; i <= ROWS; i++)
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
