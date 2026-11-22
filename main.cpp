#include <iostream>
using namespace std;

const int ROWS = 20;
const int COLS = 10;

int board[ROWS][COLS];

void InitializeBoard(){

  for(int i = 0; i<ROWS; i++){
    for(int j = 0; j<COLS; j++){
       board[i][j] = 0;
    }
  }
}
int main(){
  cout << "Hello World" << endl;
  return 0;

}
