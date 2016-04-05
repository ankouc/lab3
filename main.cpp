#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <cstdlib>
#include <time.h>

#include <random>

#define MIN(x,y) ((x)<(y)?(x):(y))

using namespace std;


int global_timeout = 60*60*4;
// returns the number of students that can not see
unsigned int cost_function(unsigned int* students,unsigned int* solution,unsigned  int size){
  unsigned int cost = 1;
  unsigned int current = students[solution[0]];
  for(unsigned int i= 1 ; i < size ; i++){
    if(students[solution[i]] > current)
      current = students[solution[i]];
    else
      cost++;
  }
  return cost;
}

int main(int argc, char** argv){
  srand(time(0));
  int verbose = 0;
  if(argc > 3)
    verbose = 1;

  ifstream f;
  f.open(argv[2]);
  unsigned int n_of_students;
  f >> n_of_students; 

  unsigned int n_of_safe_pairs;
  f >> n_of_safe_pairs;

  unsigned int* students = new unsigned int[n_of_students];
  unsigned int** adjacency_matrix = new unsigned int*[n_of_students];
  unsigned int max_heigth = 0;
  for(unsigned int i= 0; i < n_of_students; i++){
    f >> students[i];
    if(max_heigth < students[i])
      max_heigth = students[i];
    adjacency_matrix[i] = new unsigned int[n_of_students];
    for(unsigned int j = 0 ; j < n_of_students; j++)
      adjacency_matrix[i][j] = 0;
  }

  for(unsigned int i= 0; i < n_of_safe_pairs; i++){
    int a, b;
    f >> a >> b;
    adjacency_matrix[a-1][b-1] = 1;
    adjacency_matrix[b-1][a-1] = 1;
  }

  if(verbose){
    for(unsigned int i= 0; i < n_of_students; i++)
      cout << setw(2) << i ;
    cout << endl;
    for(unsigned int i= 0; i < n_of_students; i++){
      for(unsigned int j = 0; j < n_of_students; j++)
        cout << setw(2) << adjacency_matrix[i][j] ;
      cout << "--" << i << endl;
    }
  }

  unsigned int *solution = new unsigned int[n_of_students];
  unsigned int *placed = new unsigned int[n_of_students];
  int continue_looping = 1;

  while(continue_looping){
    for(unsigned int i= 0; i < n_of_students; i++)
      placed[i] = 0;
    unsigned int restart = 0;
    unsigned int index = (int)((double)rand()/RAND_MAX * n_of_students); 
    solution[0] = index;
    placed[index] = 1;
    for(unsigned int i= 1; i < n_of_students; i++){
      while(1){
        unsigned int timeout = 0;
        unsigned int current = (unsigned int)((double)rand()/RAND_MAX * n_of_students);
        while(adjacency_matrix[index][current] == 0
          || placed[current] == 1){
          current = (current+1) % n_of_students;
          timeout+=1;
          if(timeout == n_of_students-1){
            restart = 1;
            break;
          }
        }
        if(restart){
          break;
        }
        placed[current] = 1;
        solution[i] = current;
        index = current;
        break;
      }
      if(restart){
        restart = 0;
        break;
      }
      if(i == n_of_students-1)
        continue_looping = 0;
    }
  }
  delete [] placed;
  {
    unsigned int *reverse = (unsigned int*)malloc(n_of_students*sizeof(unsigned int));
    for(unsigned int i= 0 ; i < n_of_students ; i++)
      reverse[i] = solution[n_of_students-1-i];
    unsigned int reverse_cost = cost_function(students,reverse,n_of_students);
    unsigned int cost = cost_function(students,solution,n_of_students);
    if(reverse_cost < cost){
      memcpy(solution,reverse,n_of_students*sizeof(unsigned int));
    }
    free(reverse);
  }
  time_t current_time = time(0);
  unsigned int probability = 0;
  unsigned int cost = cost_function(students,solution,n_of_students);
  while(time(0)-current_time < global_timeout){
    unsigned int* potentiel = new unsigned int[n_of_students];
    memcpy(potentiel,solution,n_of_students*sizeof(unsigned int));
    unsigned int retries = n_of_students;
    while(retries--){
      unsigned int a = (unsigned int)((double) rand()/RAND_MAX * n_of_students);
      while(a == n_of_students-1)
        a = (unsigned int)((double)rand()/RAND_MAX * n_of_students);
      unsigned int b = (unsigned int)((double)rand()/RAND_MAX * n_of_students);
      int restart = 0;
      for(unsigned int i= 0 ; i < n_of_students ; i++)
        if(//students[potentiel[b]] < students[potentiel[a]]
               potentiel[b] < potentiel[a]
              || b == 0
              || potentiel[b] == potentiel[a]){
          b = (unsigned int)((double)rand()/RAND_MAX * n_of_students);
          if(i == n_of_students-1)
            restart = 1;
        }
        else
          break;

      if(restart)
        continue;

      if(a == 0 && b == n_of_students-1){
        if( adjacency_matrix[potentiel[a]][potentiel[b-1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a+1]] == 1){
          unsigned int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
          }
      }
      else if(a == 0){
        if(adjacency_matrix[potentiel[a]][potentiel[b-1]] == 1
          && adjacency_matrix[potentiel[a]][potentiel[b+1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a+1]] == 1){
          unsigned int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
        }
      }
      else if(b == n_of_students-1){
        if(adjacency_matrix[potentiel[b]][potentiel[a-1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a+1]] == 1
          && adjacency_matrix[potentiel[a]][potentiel[b-1]] == 1){
          unsigned int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
        }
      }
      else if( a != 0 && b != n_of_students-1){
        if( adjacency_matrix[potentiel[a]][potentiel[b-1]] == 1
          && adjacency_matrix[potentiel[a]][potentiel[b+1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a-1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a+1]] == 1){
          unsigned int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
        }
      }
    }
    unsigned int *reverse = (unsigned int*)malloc(n_of_students*sizeof(unsigned int));
    for(unsigned int i= 0 ;i < n_of_students ; i++)
      reverse[i] = potentiel[n_of_students-1-i];
    unsigned int potentiel_cost = cost_function(students,potentiel,n_of_students);
    unsigned int reverse_cost = cost_function(students,reverse,n_of_students);
    if(reverse_cost < potentiel_cost){
      memcpy(potentiel,reverse,n_of_students*sizeof(unsigned int));
      potentiel_cost = reverse_cost;
    }
    free(reverse);

    cost = cost_function(students,solution,n_of_students);
    //ankouc
    if(((double)rand()/RAND_MAX)*global_timeout/(double)(potentiel_cost - cost) > probability*1.5
      || cost > potentiel_cost)
      memcpy(solution,potentiel,n_of_students*sizeof(unsigned int));

    cout << cost << ":" << potentiel_cost << endl;
    probability = time(0) - current_time ;
    delete [] potentiel;
//    for(unsigned int i= 1 ;i < n_of_students ; i++)
//      if( adjacency_matrix[solution[i]][solution[i-1]] != 1)
//        cout << "==============" << endl;
  }

  cout << "nombre d etudiant qui peuvent voir " << n_of_students-cost << endl;
//  for(unsigned int i= 0 ; i < n_of_students; i++)
//    cout << solution[i]+1 << "\t" << students[solution[i]] << endl;
  return 0;
}
