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

#include <stdio.h>
#include <stdlib.h>

void header(void)
{
      printf("\n\t..Welcome to the Game of life..\n");
}

void survivalRule(char life[][20],int x,int y)
{
      int row, col;
      int neighbors = 0;
      for(row = 1; row<x; row++)
      {
         for(col = 1; col<y; col++) //fix 14 : use unused variables
         {
            if(life[row][col]== '*') //fix 1: change to value comparison with character instead of address comparison with string value
            {
               if(life[row - 1][col - 1] == '*' && row-1>=0 && col-1>=0)
                  ++neighbors;
               if(life[row - 1][col] == '*' && row-1>=0)
                  ++neighbors;
               if(life[row - 1][col + 1] == '*' && row-1>=0 && col+1<y)
                  ++neighbors;
               if(life[row][col - 1] == '*' && col-1>=0)
                  ++neighbors;
               if(life[row][col + 1] == '*' && col+1<y)
                  ++neighbors;
               if(life[row + 1][col - 1] == '*' && col-1>=0) // fix 6 : make sure it doesn't go out of bounds
                  ++neighbors;
               if(life[row + 1][col] == '*' && row+1<x)
                  ++neighbors;
               if(life[row + 1][col + 1] == '*' && row+1<x && col+1<y)
                  ++neighbors;
               if(neighbors == 2 || neighbors == 3)
               {
                  life[row][col] = '*';//fix 2 : assignment instead of comparison.
               }
            }
         }
      }
      return;
}

void birthRule(char life[][20], int x, int y)
{
      int row, col;
      int neighbors = 0;
      for(row = 1; row<x; row++)
      {
         for(col = 1; col<y; col++) //fix 13 : use unused variables
         {
            if(life[row][col]== ' ')//fix 7 : Comparison with value and character, not address or string
            {
               if(life[row - 1][col - 1] == '*')
                  neighbors++;
               if(life[row - 1][col] == '*')
                  neighbors++;
               if(life[row - 1][col + 1] == '*')
                  neighbors++;
               if(life[row][col - 1] == '*')
                  neighbors++;
               if(life[row][col + 1] == '*')
                  neighbors++;
               if(life[row + 1][col - 1] == '*')
                  neighbors++;
               if(life[row + 1][col] == '*')
                  neighbors++;
               if(life[row + 1][col + 1] == '*')
                  neighbors++;
               if(neighbors == 3)
               {
                   life[row][col] = '*'; //fix 8:remove statement with 0 effect
               }
            }
         }
      }
      return;
}


int main(int argc, char *args[])
{
      char life[20][20];
      int orgs; // fix 11: remove unused variable
      int i,  row, col;//fix 10: remove unused a and b variables
      int count = 0;
      int x = 19;
      int y = 19;
      
      // fix 9 : remove unused variable
      if (argc != 2) { return 1; }
      header();

      // convert command line argument to an int
      orgs = strtol( args[1], NULL, 10 );

      srand( 31 );

      for(i = 0; i<orgs; i++)
      {
          row = rand();
          row %= 20;
          col = rand();
          col %= 20;
          life[row][col] = '*'; //fix 3 :assignment instead of comparison
      }

      for(row = 0; row<20; row++)
      {
          for(col = 0; col<20; col++)
          {
              if(life[row][col] != '*')
                  life[row][col] = ' ';//fix 4 : assignment instead of comparison
          }
      }

      for(row = 0; row<20; row++)
      {
          for(col = 0; col<20; col++)
          {
              printf("%c", life[row][col]); //fix 5 : change to type char
          }
          puts(" ");
      }

      while ( 1 ) {
          birthRule(life, x, y);
          survivalRule(life, x, y);
          for(row = 0; row<20; row++)
          {
              for(col = 0; col<20; col++)
              {
                  printf("%c", life[row][col]); //fix 5 : change to type integer
              }
              puts(" ");
          }
          printf("\ngeneration: %d\n", count);
          count++;
          if(count==50){
            break;//fix 11 : stops when it reaches 50.
          }
      }

      return 0;
}

