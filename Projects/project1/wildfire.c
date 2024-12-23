/**
 * @file wildfire.c
 * @details implements the simulation of spreading fire.
 * @author Huy Le (hl9082)
*/

///The functions for generating random numbers and sleep delays are srandom(), random() and usleep(); 
///these require defining the C Preprocessor macro _DEFAULT_SOURCE before we include any header files.
#define _DEFAULT_SOURCE

#include<stdlib.h> ///library for random functions

#include<stdio.h> ///library for printf, fprintf, stderr, and stuff like that

#include<stdbool.h> /// library for boolean operations

#include<unistd.h> ///library for usleep function

#include<getopt.h> /// library for getopt function, and stuff like opt, opterr, and optarg

#include<string.h> /// library for string manipulation

#include<time.h> /// library for time function

#include "display.h" ///library for set_cur_pos() function

#define MAX_SIZE 40  ///maximum number of rows and columns the matrix can have

#define MIN_SIZE 5 ///minimum number of rows and columns a matrix can have

#define DEFAULT_SIZE   10            ///< used if no override on command line

#define BURN_CYCLES 3 ///the cycle that a tree can be burnt for before being burnt out

static const int seed=41; /// The seed value determines the sequence of random numbers generated; using the same seed each time the program executes will cause the generator to produced the same sequence of values, so the program's results are repeatable; this makes it much easier to compare one run of the program to another when you're debugging. Use the value 41 as the seed.

static const int reasonable_delay=750000; /// A reasonable value for the delay is 750000; this should give us time for analysis of the grid configuration.

static const char empty_cell=' '; ///value of an empty cell

static const char alive_tree='Y'; /// value for a cell with a living tree

static const char burning='*'; /// value for a cell with a burning tree

static const char burnt_out='.'; ///value for a burnt out tree

static const int max_catch_fire=100; /// maximum probability for a tree to catch fire

static const int min_catch_fire=1; /// minimum probability for a tree to catch fire

static const int catch_fire_def=30; /// default probability for a tree to catch fire

static const int burning_min=1; /// minimum proportion of trees burnt

static const int burning_max=100; /// maximum percentage of trees burnt

static const int burning_def=10; /// default percentage of trees burnt

static const int density_min=1; /// minimum density

static const int density_max=100; ///maximum density

static const int density_def=50; ///default density

static const int neighbor_effect_min=0; /// minimum neighbor's effect percentage

static const int neighbor_effect_max=100; /// maximum neighbor's effect percentage

static const int neighbor_effect_def=25; ///default neighbor's effect percentage

static const int state_number_min=0; /// minimum number of states of a forest

static const int state_number_max=10000; /// maximum number of states of a forest

static const int overlay_mode_default=-1; ///overlay mode

static const int lightning_min=0; ///minimum probability of being struck by lightning.

static const int lightning_threshold=50; /// the least probability for the tree to be thunderstruck.

static const int lightning_max=100; /// maximum probability of being struck by lightning.

static const int dampness_min=0; /// minimum dampness

static const int dampness_threshold=50; /// the least dampness for the tree to be immune to burning

static const int dampness_max=100; /// maximum dampness

static const char struck = '!'; ///what a tree looks like after being struck by lightning.

static const char damped_tree='d';///what a damped tree looks like.

static int lightning; ///global variable for lightning probability

static int dampness; ///global variable for dampness.


/**
 * This function initializes the initial state of the forst using Fisher-Yates algorithm
 * @param size the size of the forest.
 * @param arr the forest with 'size' rows and 'size' columns.
 * @param burning_percentage the percentage of burning trees.
 * @param density the percentage of trees in the matrix.
 * @exception does nothing and prints out error statement if density is less than burning percentage
 */
void initialization(int size,char arr[size][size],int burning_percentage, int density){
    
    if(burning_percentage>density){
      fprintf(stderr,"Error: Burning proportion must not be greater than density!");
      return;
    }

    int cell_number=size*size;

    char temp[cell_number];

    int num_trees=(int)(((double)(density-burning_percentage)/density_max)*cell_number);

    int burning_trees=(int)(((double)burning_percentage/burning_max)*cell_number);

    int empty=cell_number-num_trees-burning_trees;

    for(int i=0;i<num_trees;i++){
      temp[i]=(dampness<=dampness_threshold)?alive_tree:damped_tree;
    }  
    for(int i=0;i<burning_trees;i++){
      temp[i+num_trees]=burning;
    }

    for(int i=0;i<empty;i++){
      temp[i+num_trees+burning_trees]=empty_cell;
    }

     for (int i = cell_number - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char tmp = temp[i];
        temp[i] = temp[j];
        temp[j] = tmp;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if(i*size+j<cell_number){
              arr[i][j] = temp[i * size + j];
            }
        }
    }
  
  return;
}

/**
 * This function prints out the instruction for each option in the command prompt.
 */
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

/**
 * This function prints out the forest line by line.
 * @param size the size of the forest.
 * @param arr the forest itself.
 */
void print_matrix(int size,char arr[size][size]){
for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
        printf("%c",arr[i][j]);
    }
    printf("\n");
}
}

/**
 * @brief clears the terminal line.
 */
void clear_line(){
  printf("\033[K");
  fflush(stdout);
}

/**
 * This function prints out the forest line by line in the overlay mode.
 * @param size the size of the forest.
 * @param arr the forest itself.
 */
void print_matrix_overlay(int size,char arr[size][size]){
for(int i=0;i<size;i++){
  set_cur_pos(i+1,1);//move the cursor to the start of each row
    for(int j=0;j<size;j++){
        put(arr[i][j]);
    }
    printf("\033[K");
}
fflush(stdout);
}

/**
 * This function implements the updating and spreading operations.
 * Update operation:
 * Fire spreads to adjacent cells throughout the grid in all directions using eight-way connectivity, and stops at grid edges and cells that do not have any trees.
 * The update operation uses the results of the spread function to modify the grid and change the cells' states to produce the next configuration state for the simulation time step.
 * The decision on whether one tree catches fire depends on the state of its neighbors, and those neighbors also undergo possible state changes. The spread function must check the current of neighboring trees, not the new state of those neighbors. The decision to catch fire must be based on the state of neighboring trees at the start of the current cycle, not the state to which they might have already changed due to the update operation in progress.
 * The update function takes the size of the matrix and the current grid as parameters (possibly along with other data). The function could operate on the grid in-place, returning the next grid configuration through the grid parameter; however, this approach has a problem:
 * Neighbors of a cell could change state before update reaches a later neighbor and cause an inaccurate reading of the neighbors' states.
 * Spread operation:
 * The spread operation uses eight-way connectivity of neighbors to decide upon a state change for a single tree cell.
 * The spread operation first must check that the proportion of neighbors that are burning is above the level at which a tree becomes susceptible to catching fire. (This is controlled by the -nN option.)
 * To compute the proportion of neighbors that are burning, determine how many neighboring cells contain either a tree or a burning tree. (Remember that some cells (i.e., those along the grid edges) don't have the full complement of neighboring cells.) Divide the number of burning neighbors by the total number of neighboring trees to calculate the proportion of burning neighboring trees. If the proportion of neighbors burning is above the -nN value, the neighbor proportion threshold, then the tree is susceptible to catching fire.
 * When a tree is susceptible to catching fire, whether or not it does is determined by the catch fire probability and a randomly-generated number. Generate a random value in the range 0.0 to 1.0. If that value is less than the probability of catching fire, then the tree should catch fire. (This is the case because, as the liklihood of any single tree catching fire grows, the number of values less than that also grows.)
 * Trees that catch fire go through a burn process that takes multiple cycles. This means when a tree starts burning, and changes to the burning state for the first time, it remains in the burning state for the next several time steps. Eventually a burning tree burns out and becomes a tree in the burned-out state.
 * We will model this multi-cycle burn process as follows. When a tree changes from not burning to burning, that is one transition during an update cycle. The tree remains burning for the next two update cycles after that; it burns out after the third cycle. 
 * @param size the size of the forest.
 * @param matrix the forest.
 * @param catch_fire_prob the probability of catching fire.
 * @param neighbors_influenced if the actual probability is greater than this, the tree should catch fire.
 * @param current_change the integer pointer pointing to the variable to count the number of states changed in the current state.
 * @param cumulative_change the sum of all current_change values in all cycles.
 */
void forest_burn(int size,char matrix[size][size], int catch_fire_prob, int neighbors_influenced, 
int *current_change, int *cumulative_change,int cycles[size][size],bool visited[size][size]){

if(lightning>lightning_threshold){
    int randomRow = rand() % size;
    int randomCol = rand() % size;
    if(matrix[randomRow][randomCol]==alive_tree){
      matrix[randomRow][randomCol] = struck; 
    }
}

for(int row_ind=0;row_ind<size;row_ind++){
  for(int col_ind=0;col_ind<size;col_ind++){
    if(matrix[row_ind][col_ind]==burning && !visited[row_ind][col_ind]){
      cycles[row_ind][col_ind]++;
      visited[row_ind][col_ind]=true;
      if(cycles[row_ind][col_ind]==BURN_CYCLES-1){
        matrix[row_ind][col_ind]=burnt_out;
        (*current_change)++;
      }
    }
    
    else if(matrix[row_ind][col_ind]==struck){
      matrix[row_ind][col_ind]=burning;
      (*current_change)+=1;
    }

    else if(matrix[row_ind][col_ind]==burnt_out || matrix[row_ind][col_ind]==empty_cell){
      continue;
    }
    else if(matrix[row_ind][col_ind]==alive_tree){
        
    int burn_neighbors=0,total_neighbors=0;
        
    for(int i=-1;i<2;i++){
      for(int j=-1;j<2;j++){
        if(i==0 || j==0){
          continue;//we ignore the current cell itself.
        }
        int row_neighbor=row_ind+i,col_neighbor=col_ind+j;
        //if it is within the boundaries
        if(row_neighbor>=0 && row_neighbor<size && col_neighbor>=0 && col_neighbor<size){
          if(matrix[row_neighbor][col_neighbor]==burning || matrix[row_neighbor][col_neighbor]==alive_tree){
            total_neighbors++;
          }
          if(matrix[row_neighbor][col_neighbor]==burning){
            burn_neighbors++;
          }
          }
        }
      }
        //calculate the proportion of burning neighbors when there are neighboring living trees
        int burning_proportion=(int)(((double)burn_neighbors/total_neighbors)*burning_max);
        int prob=rand()%(1+max_catch_fire);
        if(burning_proportion>=neighbors_influenced && prob<catch_fire_prob && dampness<=dampness_threshold){
          matrix[row_ind][col_ind]=burning;
          (*current_change)+=1;
        }

        
        
        
    }
      
    
  }
}
 
(*cumulative_change)+=(*current_change); 
  
}

/**
 * This function checks if the current tree can catch fire.
 * @param size the size of the forest.
 * @param matrix the forest itself.
 * @param row the tree's row's index.
 * @param col the tree's column index.
 * @param neighbors_influenced if the actual probability is greater than this, then the tree should catch fire.
 * @param catch_fire_prob probability that the tree can catch fire.
 * @exception if either row index or column index goes out of bounds, or size goes out of bounds, or the tree is not unscathed, then it is false.
 * @return true if the tree can catch fire, and false if otherwise.
 */
bool can_catch_fire(int size,char matrix[size][size], int row, int col
,int neighbors_influenced,int catch_fire_prob){
  bool flag=false;
  if(row<0||row>=size||col<0||col>=size||size>MAX_SIZE||size<MIN_SIZE){
    return flag;
  }
  if(matrix[row][col]!=alive_tree){
    flag=(matrix[row][col]==struck);
    return flag;
  }
  int burn_neighbors=0,total_neighbors=0;
  for(int i=-1;i<2;i++){
      for(int j=-1;j<2;j++){
        if(i==0 || j==0){
          continue;//we ignore the current cell itself.
        }
        int row_neighbor=row+i,col_neighbor=col+j;
        //if it is within the boundaries
        if(row_neighbor>=0 && row_neighbor<size && col_neighbor>=0 && col_neighbor<size){
          if(matrix[row_neighbor][col_neighbor]==burning || matrix[row_neighbor][col_neighbor]==alive_tree){
            total_neighbors++;
          }
          if(matrix[row_neighbor][col_neighbor]==burning){
            burn_neighbors++;
          }
          }
        }
      }
    int burning_proportion=(int)(((double)burn_neighbors/total_neighbors)*burning_max);
    int prob=rand()%(1+max_catch_fire);
    if(burning_proportion>=neighbors_influenced && prob<catch_fire_prob && dampness<=dampness_threshold) {
      flag=true;
    }
  
  return flag;
}

/**
 * This function checks if the forest can continue to be burnt, using the can_catch_fire function above.
 * @param size the size of the forest.
 * @param matrix the forest itself.
 * @param neighbors_influenced if the actual probability is greater than this, then the tree should catch fire.
 * @param catch_fire_prob the probability that a tree can catch fire.
 * @return true if the forest can continue to be burnt, and false if otherwise.
 */
bool is_burning(int size, char matrix[size][size],int neighbors_influenced,int catch_fire_prob){
  for(int row=0;row<size;row++){
    for(int col=0;col<size;col++){
      if(matrix[row][col]==burning){
        for(int i=-1;i<2;i++){
          for(int j=-1;j<2;j++){
            int row_neighbor=row+i,col_neighbor=col+j;
            if(row_neighbor>=0 && row_neighbor<size && col_neighbor>=0 && col_neighbor<size){
              if(can_catch_fire(size,matrix,row_neighbor,col_neighbor,neighbors_influenced,catch_fire_prob)){
                  return true;
              }
            }
          }
        }
        
      }
  }
}
  return false;
}

/**
 * @brief This function checks if there are any burning trees left in the forest.
 * @param size the size of the forest.
 * @param matrix the forest itself.
 * @return true if there's a burning tree, and false if otherwise.
 */
bool has_fire(int size,char matrix[size][size]){
  bool flag=false;
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      if(matrix[i][j]==burning){
        flag=true;
        break;
      }
    }
    if(flag){
      break;
    }
  }
  return flag;
}

/**
 * @brief prints out the information of the current matrix.
 * @param size matrix's size.
 * @param catch catch fire probability.
 * @param density the forest density.
 * @param burning proportion of burning trees.
 * @param neighbor neighboring effect percentage.
 * @param current_change current change number.
 * @param cumulative_change cumulative change number.
 * @param cycle the current cycle number.
 */
void print_info(int size,int catch,int density,int burnt,int neighbor, int current_change, int cumulative_change, int cycle){
  printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f\n",size,
      (double)catch/max_catch_fire, (double)density/density_max, 
      (double)burnt/burning_max, (double)neighbor/neighbor_effect_max);
    printf("cycle %d, current changes %d, cumulative changes %d.\n",cycle,current_change,cumulative_change);
}

/**
 * @brief This is where we implement everything with the forest, from initializing to burning.
 * @param argc the number of line arguments.
 * @param args the list of line arguments.
 * @exception we return EXIT_FAILURE if either the number of arguments is less than 1, or something's wrong with passing values, or wrong argument options passed in.
 * @return EXIT_SUCCESS if everything is implemented correctly. Otherwise, return EXIT_FAILURE.
 */

int main(int argc, char *args[]){
    
    srand(seed);
    
    extern int opterr;
    int opt;
    
    int burnt=burning_def,
    catch_fire=catch_fire_def,
    density=density_def,
    neighbors_influenced=neighbor_effect_def,number_of_states=overlay_mode_default,
    grid_size=DEFAULT_SIZE;
    
    
    opterr = 0;
    while((opt=getopt(argc,args,"H::b::c::d::n::p::s::L::D::")) != -1){
      
      
      switch(opt){

        case 'H':
          print_instruction();
          exit(EXIT_SUCCESS);
        break;
        case 'b':
         
          burnt=atoi(optarg);
          if(burnt<burning_min||burnt>burning_max){
            fprintf(stderr,"(-bN) proportion already burning must be an integer in [1...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
         case 'c':
          
          catch_fire=atoi(optarg);
          if(catch_fire<min_catch_fire||catch_fire>max_catch_fire){
            fprintf(stderr,"(-cN) probability a tree will catch fire must be an integer in [1...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);  
          }
         break;
        case 'd':
          
          density=atoi(optarg);
          if(density<density_min||density>density_max){
            fprintf(stderr,"(-dN) density of trees in the grid must be an integer in [1...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
        case 'n':
          
          neighbors_influenced=atoi(optarg);
          if(neighbors_influenced<neighbor_effect_min||neighbors_influenced>neighbor_effect_max){
            fprintf(stderr,"-nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.(-nN) percentage of neighbors influence catching fire must be an integer in [0...100].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
        case 'p':
          
          number_of_states=atoi(optarg);
          if(number_of_states<state_number_min || number_of_states>state_number_max){
            fprintf(stderr,"(-pN) number of states to print must be an integer in [0...10000].\n");
            print_instruction();
           exit(EXIT_FAILURE);
         }
          break;
        case 's':
         
          grid_size=atoi(optarg);
          if(grid_size<MIN_SIZE||grid_size>MAX_SIZE){
            fprintf(stderr,"(-sN) simulation grid size must be an integer in [5...40].\n");
            print_instruction();
            exit(EXIT_FAILURE);
          }
          break;
        case 'L':
          lightning=atoi(optarg);
          if(lightning<lightning_min||lightning>lightning_max){
            lightning=lightning_min;
          }
          break;
        case 'D':
          dampness=atoi(optarg);
          if(dampness<dampness_min || dampness>dampness_max){
            dampness=dampness_min;
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
    
  char forest[grid_size][grid_size];
   
    
  initialization(grid_size,forest,burnt,density);
  
  int cycles[grid_size][grid_size];

  

  bool visited[grid_size][grid_size];

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
  printf("\033[H\033[J"); // Clear the screen
  }

  while(is_burning(grid_size,forest,neighbors_influenced,catch_fire)==true
  && (flag<=number_of_states||number_of_states==overlay_mode_default)){

    for(int i=0;i<grid_size;i++){
      for(int j=0;j<grid_size;j++){
        visited[i][j]=false;
      }
    }
    if(number_of_states!=overlay_mode_default){
          //print the forest only when print mode is enabled.
      print_matrix(grid_size,forest);
      print_info(grid_size,catch_fire, density,burnt, neighbors_influenced, current_change, cumulative_change, flag);
  }

         //overlay mode
    else{
      clear();
      set_cur_pos(1,1);
      print_matrix_overlay(grid_size,forest);
      set_cur_pos(grid_size+1,1);
      clear_line();
      print_info(grid_size,catch_fire, density,burnt, neighbors_influenced,  current_change,  cumulative_change, flag);
      usleep(reasonable_delay);
    }
    current_change=0;
    forest_burn(grid_size,forest,catch_fire,neighbors_influenced,&current_change,&cumulative_change,cycles,visited);
    flag++;
    }

    set_cur_pos(grid_size+4,1);

    if(!has_fire(grid_size,forest)){
      printf("Fires are out.\n");
    }
    return EXIT_SUCCESS;
}