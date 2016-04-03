#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <cstdlib>
#include <time.h>

#include <random>


using namespace std;

// returns the number of students that can not see
unsigned int cost_function(int* solution, int size){
  unsigned int cost = 0;
  unsigned int current = solution[0];
  for(int i = 1 ; i < size ; i++){
    if(solution[i] > current)
      current = solution[i];
    else
      cost++;
  }
  return cost;
}

int main(int argc, char** argv){
  srand(time(0));
  default_random_engine generator;
  generator.seed(time(0));
  int verbose = 0;
  if(argc > 3)
    verbose = 1;

  ifstream f;
  f.open(argv[2]);
  unsigned int n_of_students;
  f >> n_of_students; 
//  printf("[n_of_students=%d]\n",n_of_students);

  unsigned int n_of_safe_pairs;
  f >> n_of_safe_pairs;
// printf("[n_of_safe_pairs=%d]\n",n_of_safe_pairs);

  unsigned int* students = new unsigned int[n_of_students];
  unsigned int** adjacency_matrix = new unsigned int*[n_of_students];
  unsigned int max_heigth = 0;
  for(unsigned int i = 0; i < n_of_students; i++){
    f >> students[i];
    if(max_heigth < students[i])
      max_heigth = students[i];
    adjacency_matrix[i] = new unsigned int[n_of_students];
    for(unsigned int j = 0 ; j < n_of_students; j++)
      adjacency_matrix[i][j] = 0;
  }

  for(unsigned int i = 0; i < n_of_safe_pairs; i++){
    int a, b;
    f >> a >> b;
    adjacency_matrix[a-1][b-1] = 1;
    adjacency_matrix[b-1][a-1] = 1;
  }

  if(verbose){
    for(unsigned int i = 0; i < n_of_students; i++)
      cout << setw(2) << i ;
    cout << endl;
    for(unsigned int i = 0; i < n_of_students; i++){
      for(unsigned int j = 0; j < n_of_students; j++)
        cout << setw(2) << adjacency_matrix[i][j] ;
      cout << "--" << i << endl;
    }
  }

  unsigned int *solution = new unsigned int[n_of_students];
  unsigned int *placed = new unsigned int[n_of_students];
  int hello = 1;

  while(hello){
    for(unsigned int i = 0; i < n_of_students; i++)
      placed[i] = 0;
    unsigned int restart = 0;
    unsigned int index = rand() % n_of_students; 
    solution[0] = index;
    for(unsigned int i = 1; i < n_of_students; i++){
      while(1){
        unsigned int timeout = 0;
        unsigned int current = rand() % n_of_students;
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
        hello = 0;
    }
  }
  time_t current_time = time(0);
  while(time(0)-current_time < 60){
    unsigned int* potentiel = new unsigned int[n_of_students];
    memcpy(potentiel,solution,n_of_students*sizeof(unsigned int));
    unsigned int retries = n_of_students;
    while(retries--){
      unsigned int a = rand() % n_of_students;
      while(a == 0 || a == n_of_students-1)
        a = rand() % n_of_students;
      unsigned int b = rand() % n_of_students;
      int restart = 0;
      for(int i = 0 ; i < n_of_students ; i++)
        if(students[potentiel[b]] > students[potentiel[a]]
              || potentiel[b] < potentiel[a]
              || b == n_of_students-1
              || b == 0
              || potentiel[b] == potentiel[a]){
          b = rand() % n_of_students;
          if(i == n_of_students-1)
            restart = 1;
        }
        else
          break;

      if(restart)
        continue;

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
    unsigned int cost = cost_function(solution,n_of_students);
    unsigned int potentiel_cost = cost_function(potentiel,n_of_students);
    if(cost > potentiel_cost){
      memcpy(solution,potentiel,n_of_students*sizeof(unsigned int));
      cout << cost << ":" << potentiel_cost << endl;
    }
  }
  return 0;
}
