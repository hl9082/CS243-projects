/**
 * file: wildfire.c
 * implements the simulation of spreading fire.
 * @author Huy Le
*/

#define _DEFAULT_SOURCE

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>
#include<time.h>
#define MAX_SIZE 40

#define MIN_SIZE 5

#define DEFAULT_SIZE   10            ///< used if no override on command line

#define BURN_CYCLES 3

static const int seed=41;

static const int reasonable_delay=750000;

static const char empty_cell=' ';

static const char alive_tree='Y';

static const char burning='*';

static const char burnt_out='.';

static const int max_catch_fire=100;

static const int min_catch_fire=1;

static const int catch_fire_def=30;

static const int burning_min=1;

static const int burning_max=100;

static const int burning_def=10;

static const int density_min=1;

static const int density_max=100;

static const int density_def=50;

static const int neighbor_effect_min=0;

static const int neighbor_effect_max=100;

static const int neighbor_effect_def=25;

static const int state_number_min=0;

static const int state_number_max=10000;

static const int overlay_mode_default=-1;

//initialization operation (Fisher-Yate random shuffle)
void initialization(char arr[MAX_SIZE][MAX_SIZE],int burning_percentage, int density,int size){
    int cell_number=size*size;

    int num_trees=(density*cell_number)/density_max;

    int burning_trees=(burning_percentage*cell_number)/burning_max;

    srand(seed);
  
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      arr[i][j]=empty_cell;
    }
  }

  for(int i=0;i<num_trees;i++){
    int row,col;
    do{
      row=rand()%size;
      col=rand()%size;
    }while(arr[row][col]!=empty_cell);
    arr[row][col]=alive_tree;
  }

  int current_burning_trees=0;

  while(current_burning_trees<burning_trees){
    int row=rand()%size,col=rand()%size;
    if(arr[row][col]==alive_tree){
      arr[row][col]=burning;
      current_burning_trees++;
    }
  }
  
  return;
}

//to print instruction
void print_instruction(){
fprintf(stderr,"usage: wildfire [options]\n");
fprintf(stderr,"By default, the simulation runs in overlay display mode.\n");
fprintf(stderr,"The -pN option makes the simulation run in print mode for up to N states.\n");
fprintf(stderr,"\n");
fprintf(stderr,"Simulation Configuration Options:\n");
fprintf(stderr," -H  # View simulation options and quit.\n");
fprintf(stderr," -bN # proportion of trees that are already burning. 0 < N < 101.\n");
fprintf(stderr," -cN # probability that a tree will catch fire. 0 < N < 101.\n");
fprintf(stderr," -dN # density: the proportion of trees in the grid. 0 < N < 101.\n");
fprintf(stderr," -nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.\n");
fprintf(stderr," -pN # number of states to print before quitting. -1 < N < ...\n");
fprintf(stderr," -sN # simulation grid size. 4 < N < 41.\n");
}

//print matrix
void print_matrix(char arr[MAX_SIZE][MAX_SIZE], int size){
for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
        printf("%c ",arr[i][j]);
    }
    printf("\n");
}
}

//this function is to change the forest by burning it or burn it out.
void forest_burn(char matrix[MAX_SIZE][MAX_SIZE], int size, int catch_fire_prob, int neighbors_influenced){
  srand(seed);
//char dummy[size][size];
int flag=0;
for(int row_ind=0;row_ind<size;row_ind++){
  for(int col_ind=0;col_ind<size;col_ind++){
   // dummy[row_ind][col_ind]=matrix[row_ind][col_ind];
  
    if(matrix[row_ind][col_ind]==burning){
      flag++;
      if(flag==BURN_CYCLES-1){
        matrix[row_ind][col_ind]=burnt_out;
        flag=0;
      }
    }
    
    else if(matrix[row_ind][col_ind]==burnt_out || matrix[row_ind][col_ind]==empty_cell){
      continue;
    }
    else if(matrix[row_ind][col_ind]==alive_tree){
        //count living and burning neighbors
    int burn_neighbors=0,live_neighbors=0;
        //checking 8 neighbors
    for(int i=-1;i<2;i++){
      for(int j=-1;j<2;j++){
        if(i==0 || j==0){
          continue;//we ignore the current cell itself.
        }
        int row_neighbor=row_ind+i,col_neighbor=col_ind+j;
        //if it is within the boundaries
        if(row_neighbor>=0 && row_neighbor<size && col_neighbor>=0 && col_neighbor<size){
          if(matrix[row_neighbor][col_neighbor]==burning){
            burn_neighbors++;
          }
          else if(matrix[row_neighbor][col_neighbor]==alive_tree){
              live_neighbors++;
            }
          }
        }
      }
        //calculate the proportion of burning neighbors when there are neighboring living trees
        if(live_neighbors>0){
          int total_neighbors=burn_neighbors+live_neighbors;
          double burning_probability=(double)burn_neighbors/total_neighbors;
          
          int prob=rand()%(1+max_catch_fire);
          if(burning_probability>(double)neighbors_influenced/max_catch_fire
          && prob>catch_fire_prob){
            matrix[row_ind][col_ind]=burning;
          }
        }
    }
      
    
  }
}
 /** 
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      matrix[i][j]=dummy[i][j];
    }
  }
  **/
  
}

bool can_catch_fire(char matrix[MAX_SIZE][MAX_SIZE],int size, int row, int col
,int neighbors_influenced,int catch_fire_prob){
  bool flag=false;
  if(row<0||row>=size||col<0||col>=size||size>MAX_SIZE||size<MIN_SIZE){
    return flag;
  }
  if(matrix[row][col]!=alive_tree){
    return flag;
  }
  srand(seed);
  int burn_neighbors=0,live_neighbors=0;
  for(int i=-1;i<2;i++){
      for(int j=-1;j<2;j++){
        if(i==0 || j==0){
          continue;//we ignore the current cell itself.
        }
        int row_neighbor=row+i,col_neighbor=col+j;
        //if it is within the boundaries
        if(row_neighbor>=0 && row_neighbor<size && col_neighbor>=0 && col_neighbor<size){
          if(matrix[row_neighbor][col_neighbor]==burning){
            burn_neighbors++;
          }
          else if(matrix[row_neighbor][col_neighbor]==alive_tree){
              live_neighbors++;
            }
          }
        }
      }
  if(live_neighbors>0){
      int total_neighbors=burn_neighbors+live_neighbors;
      double burning_probability=(double)burn_neighbors/total_neighbors;
      int prob=rand()%(1+max_catch_fire);
      if(burning_probability>(double)neighbors_influenced/max_catch_fire 
      && prob>catch_fire_prob){
        flag=true;
      }
  }
  return flag;
}

bool is_burning(char matrix[MAX_SIZE][MAX_SIZE],int size,int neighbors_influenced,int catch_fire_prob){
  //bool flag =false;
  for(int row=0;row<size;row++){
    for(int col=0;col<size;col++){
      if(matrix[row][col]==burning){
        for(int i=-1;i<2;i++){
          for(int j=-1;j<2;j++){
            int row_neighbor=row+i,col_neighbor=col+j;
            if(row_neighbor>=0 && row_neighbor<size && col_neighbor>=0 && col_neighbor<size){
              if(can_catch_fire(matrix,size,row_neighbor,col_neighbor,neighbors_influenced,catch_fire_prob)){
                  return true;
              }
            }
          }
        }
        
      }
  }
  /**
  if(flag){
      break;
  }**/
}
  return false;
}
int main(int argc, char *args[]){
    

    if(argc<=1){
        return EXIT_FAILURE;
    }
    extern int opterr;
    int opt;
    
    int burnt=burning_def,
    catch_fire=catch_fire_def,
    density=density_def,
    neighbors_influenced=neighbor_effect_def,number_of_states=overlay_mode_default,
    grid_size=DEFAULT_SIZE;
    //number of states being -1 means it's in overlay mode.
    //solution: we can make an array to store arguments in the form of strings.
    opterr = 0;
    while((opt=getopt(argc,args,"H::b::c::d::n::p::s::")) != -1){
      //char tmp_str[3];
      
      switch(opt){

        case 'H':
          print_instruction();
          exit(EXIT_SUCCESS);
        break;//this is where we break out of the switch-case.
        case 'b':
         //strncpy(tmp_str,arg_list[i]+2,strlen(arg_list[i])-2);
          burnt=atoi(optarg);
          if(burnt<burning_min||burnt>burning_max){
            fprintf(stderr,"(-bN) proportion already burning must be an integer in [1...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
         case 'c':
          //strncpy(tmp_str,arg_list[i]+2,strlen(arg_list[i])-2);
          catch_fire=atoi(optarg);
          if(catch_fire<min_catch_fire||catch_fire>max_catch_fire){
            fprintf(stderr,"(-cN) probability a tree will catch fire must be an integer in [1...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);  
          }
         break;
        case 'd':
          //strncpy(tmp_str,arg_list[i]+2,strlen(arg_list[i])-2);
          density=atoi(optarg);
          if(density<density_min||density>density_max){
            fprintf(stderr,"(-dN) density of trees in the grid must be an integer in [1...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
        case 'n':
          //strncpy(tmp_str,arg_list[i]+2,strlen(arg_list[i])-2);
          neighbors_influenced=atoi(optarg);
          if(neighbors_influenced<neighbor_effect_min||neighbors_influenced>neighbor_effect_max){
            fprintf(stderr,"-nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.(-nN) percentage of neighbors influence catching fire must be an integer in [0...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
        case 'p':
          //strncpy(tmp_str,arg_list[i]+2,strlen(arg_list[i])-2);
          number_of_states=atoi(optarg);
          if(number_of_states<state_number_min || number_of_states>state_number_max){
            fprintf(stderr,"(-pN) number of states to print must be an integer in [0...10000].\n");
            print_instruction();
           exit(EXIT_FAILURE);
         }
          break;
        case 's':
          //strncpy(tmp_str,arg_list[i]+2,strlen(arg_list[i])-2);
          grid_size=atoi(optarg);
          printf("size: %d\n",grid_size);
          if(grid_size<MIN_SIZE||grid_size>MAX_SIZE){
            fprintf(stderr,"(-sN) simulation grid size must be an integer in [5...40].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
        default:
            // some unknown, possibly unacceptable option flag
            fprintf( stderr, "Bad option causes failure; ignored.\n" );
            print_instruction();
            exit(EXIT_FAILURE); 
          break;
      }
    
    }

    char forest[MAX_SIZE][MAX_SIZE];
   
    
    initialization(forest,burnt,density,grid_size);
    //printf("Initialized Forest State:\n");
    //print_matrix(forest, grid_size);

   int flag=0;//cycle - for the simulation of states of the matrix
   int current_change=0,cumulative_change=0;
   if(number_of_states!=overlay_mode_default){
    printf("===========================\n");
    printf("======== Wildfire =========\n");
    printf("===========================\n");
    printf("=== Print  %d Time Steps ===\n",number_of_states);
    printf("===========================\n");
   }
   else{
    printf("The program cleared the screen here.\n");
    //print_matrix(forest,grid_size,grid_size);
   }
    while(is_burning(forest,grid_size,neighbors_influenced,catch_fire)==true
    && (number_of_states==overlay_mode_default || flag<number_of_states)){
        if(number_of_states!=overlay_mode_default){
          //print the forest only when print mode is enabled.
          print_matrix(forest,grid_size);
          printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f\n",grid_size,
          (double)catch_fire/max_catch_fire, (double)density/density_max, 
          (double)burnt/burning_max, (double)neighbors_influenced/neighbor_effect_max);
          printf("cycle %d, current changes %d, cumulative changes %d.\n",flag,
          current_change,cumulative_change);
        }

         //overlay mode
        else{
          usleep(reasonable_delay);
        }
        forest_burn(forest,grid_size,catch_fire,neighbors_influenced);
        //change the matrix for the next cycle
       current_change++;
       cumulative_change+=current_change;
       
       flag++;
    }
    //printf("I am here, 3");
    //print last states if we only have overlay mode
    if(number_of_states==overlay_mode_default || flag==0){
      print_matrix(forest,grid_size);
      printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f\n",grid_size,
          (double)catch_fire/max_catch_fire, (double)density/density_max, 
          (double)burnt/burning_max, (double)neighbors_influenced/neighbor_effect_max);
      printf("cycle %d, current changes %d, cumulative changes %d.\n",flag,
          current_change,cumulative_change);
    }
    printf("Fires are out.\n");
    return EXIT_SUCCESS;
}