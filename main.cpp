#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
bool isCurrentBlockCell(int, int);
bool canMoveTo(int, int);
void InitializeBoard();
void DrawBoard();


const int ROWS = 20;
const int COLS = 10;

int board[ROWS][COLS];
int shapes[3][4][4]{
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
int shapeWidth[3] = {2,4,3};

int currentShape;
int nextShape;
int blockX;
int blockY = 0;

int main()
{

  currentShape = rand() % 3;
  nextShape = rand() % 3;
  blockX = (COLS - shapeWidth[currentShape])/2;
  blockY = 0;
  srand(time(0));
  InitializeBoard();
  DrawBoard();
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
      if (board[i][j] == 1 || isCurrentBlockCell(i,j))
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

bool isCurrentBlockCell(int row, int col){
  int r = row - blockY;
  int c = col - blockX;
  
  if (r < 0 || r >= 4 || c < 0 || c >= 4){
     return false;
  }

  return shapes[currentShape][r][c] == 1;
}

bool canMoveTo(int newX, int newY){
  for (int r = 0; r < 4; r++){
      for(int c = 0; c < 4; c++){
        if(shapes[currentShape][r][c] == 1){
          int row = newY + r; 
          int col = newX + c;
          
          if( col < 0 || col >= COLS || row >= ROWS){
            return false;
          }

          if(board[row][col] == 1){
            return false;
          }
        }
      } 
  }
  return true;
}