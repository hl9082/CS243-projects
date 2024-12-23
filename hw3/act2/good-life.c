/// http://www.dreamincode.net/forums/topic/55572-conways-game-of-life/
/// downloaded by copy-paste on 8/19/2015 and modified to use an infinite loop
/// and have different Rule functions.
///
/// This code needs serious work in these areas:
/// <ol>
/// <li>  syntax: there are a number of warnings that cause errors.
/// </li>
/// <li>  semantics: the game algorithm implementation is incorrect.
/// </li>
/// <li>  design: the implementation needs function and value refactoring.
/// </li>
/// <li>  style: formatting is poor; the mix of TAB and spaces indentation
/// needs correction, and spacing between tokens is inconsistent.
/// The course style puts the '{' at the end of the function header, not
/// on a line of its own. You should make the formatting consistent.
/// </li>
/// <li>  documentation: documentation is non-existent except for this block.
/// </li>
/// </ol>
///Contributing author : Huy Le
/// Contributing author's email : hl9082@rit.edu
///

#include <stdio.h> /// < library for standard input/output.
#include <stdlib.h> /// < standard library in C.
#define MAXVAL 20 ///fix 10 : resolve magic number.

///Display the welcome message.
///@param void nothing is parameter.

void header(void)
{
      printf("\n\t..Welcome to the Game of life..\n");
}

///This is a helper's function to count the neighbors of the given cell in the (row,col) coordinate.
///@param life the life matrix.
///@param row the row index.
///@param col the column index.
///@return the number of living neighbors.
int count_live_neighbours(char life[][MAXVAL], int row, int col)
{
    int neighbors = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if ((i == row && j == col) || (i < 0 || j < 0)
                || (i >= MAXVAL || j >= MAXVAL)) {
                continue;
            }
            if (life[i][j] == '*') {
                neighbors++;
            }
        }
    }
    if(col==0){
    switch(row){
      case 0:
        if(life[row][MAXVAL-1]=='*'){
           neighbors++;
           }          
                    
        if(life[row+1][MAXVAL-1]=='*'){
          neighbors++;
        }
        if(life[MAXVAL-1][col]=='*'){
          neighbors++;
        }
        if(life[MAXVAL-1][MAXVAL-1]=='*'){
          neighbors++;
        }
        if(life[MAXVAL-1][col+1]=='*'){
          neighbors++;
        }
                    
        break;
      case MAXVAL-1:
        if(life[row][MAXVAL-1]=='*'){
          neighbors++;
          }
        if(life[row-1][MAXVAL-1]=='*'){
           neighbors++;
        }
        if(life[0][col]=='*'){
          neighbors++;
        }
        if(life[0][col+1]=='*'){
          neighbors++;
        }
        if(life[0][MAXVAL-1]=='*'){
          neighbors++;
        }         
        break;
      default:
        if(life[row][MAXVAL-1]=='*'){
           neighbors++;
           }
        if(life[row+1][MAXVAL-1]=='*'){
           neighbors++;
           }
                    
        if(life[row-1][MAXVAL-1]=='*'){
          neighbors++;
          }
        break;
    }
    }
    else if(col==MAXVAL-1){
    switch(row){
      case 0:
      if(life[row][0]=='*'){
         neighbors++;
      }
      if(life[row+1][0]=='*'){
         neighbors++;
      }
      if(life[MAXVAL-1][0]=='*'){
         neighbors++;
      }
      if(life[MAXVAL-1][col]=='*'){
         neighbors++;
      }
      if(life[MAXVAL-1][col-1]=='*'){
         neighbors++;
      }         
          break;
      case MAXVAL-1:
        if(life[row][0]=='*'){
          neighbors++;
          }
        if(life[row-1][0]=='*'){
           neighbors++;
           }
        if(life[0][col-1]=='*'){
          neighbors++;
        }
        if(life[0][col]=='*'){
          neighbors++;
        }
        if(life[0][0]=='*'){
          neighbors++;
        }         
        break;
      default:
        if(life[row][0]=='*'){
           neighbors++;
           }
        if(life[row+1][0]=='*'){
           neighbors++;
           }
                    
        if(life[row-1][0]=='*'){
          neighbors++;
          }
        break;
      }
    }
    else{
      switch(row){
        case 0:
          if(life[MAXVAL-1][col]=='*'){
            neighbors++;
          }
          if(life[MAXVAL-1][col-1]=='*'){
            neighbors++;
          }
          if(life[MAXVAL-1][col+1]=='*'){
            neighbors++;
          }
          break;
        case MAXVAL-1:
          if(life[0][col]=='*'){
            neighbors++;
          }
          if(life[0][col-1]=='*'){
            neighbors++;
          }
          if(life[0][col+1]=='*'){
            neighbors++;
          }
          break;
      }
    }
    return neighbors;
}

///Fills out the matrix based on the game's rule for survival and birth.
///@param life the matrix to be filled.
///@param x the last row index.
///@param y the last colum index.
void survival_and_birth_Rule(int x, char life[][MAXVAL])//fix 1: change the name of the function and change parameters
{
      int row, col;
      for(row = 0; row<=x; row++)
      {
         for(col = 0; col<=MAXVAL-1; col++)//fix 6 : Use the unused variables
         {
      int neighbors=count_live_neighbours(life,row,col);
         if(life[row][col]=='*'){
            if(neighbors<2 || neighbors>3){
              life[row][col]=' ';//fix 13: added the if-statement to make sure the logic is correct.
            }
         }
         else{
           if(neighbors==3){
             life[row][col]='*';
           }
         }
               }
                }
      return;
}


///The main function to play the game.
///@param argc the number of arguments.
///@param args the array of arguments.
///@return 0 if successful.
int main(int argc, char *args[])
{
      char life[MAXVAL][MAXVAL];
      int orgs;//fix 11 : remove the unused
      int i,  row, col;//fix 9 : remove the unused
      int count = 0;
      int x = 19;
      // fix 10 : remove the unused
      if (argc != 2) { return 1; }
      header();

      // convert command line argument to an int
      orgs = strtol( args[1], NULL, 10 );

      srand( 31 );

      for(i = 0; i<orgs; i++)
      {
          row = rand();
          row %= MAXVAL; // fix 11 : resolve magic number
          col = rand();
          col %= MAXVAL; //fix 12: resolve magic number
          life[row][col] = '*'; //fix 3 :assignment instead of comparison
      }

      for(row = 0; row<MAXVAL; row++)
      {
          for(col = 0; col<MAXVAL; col++)
          {
              if(life[row][col] != '*')
                  life[row][col] = ' ';//fix 4 : assignment instead of comparison
          }
      }

      for(row = 0; row<MAXVAL; row++)
      {
          for(col = 0; col<MAXVAL; col++)
          {
              printf("%c", life[row][col]); //fix 5 : change to type char
          }
          puts(" ");
      }

      while ( count<50 ) {//fix 15: simplify the logic
          survival_and_birth_Rule(x,life);
          for(row = 0; row<MAXVAL; row++)
          {
              for(col = 0; col<MAXVAL; col++)
              {
                  printf("%c", life[row][col]); //fix 5 : change to type char
              }
              puts(" ");
          }
          printf("\ngeneration: %d\n", count);
          count++;
      }

      return 0;
}

