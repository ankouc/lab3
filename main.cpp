#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <cstdlib>
#include <time.h>

#include <random>

#define MIN(x,y) ((x)<(y)?(x):(y))

using namespace std;


int global_timeout = 60*3;//60*60;
int cost_function(int* students,int* solution,int size){
  int cost = 1;
  int current = students[solution[0]];
  for(int i= 1 ; i < size ; i++){
    if(students[solution[i]] > current)
      current = students[solution[i]];
    else
      cost++;
  }
  return cost;
}

int main(int argc, char** argv){
  if(argc < 3)
    return 1;
  if(strcmp(argv[1],"-f") != 0)
    return 1;
  srand(time(0));
  time_t current_time = time(0);
  
  ifstream f;
  f.open(argv[2]);
  int n_of_students;
  f >> n_of_students; 

  int n_of_safe_pairs;
  f >> n_of_safe_pairs;

  int* students = new int[n_of_students];
  int** adjacency_matrix = new int*[n_of_students];
  int max_heigth = 0;
  for(int i= 0; i < n_of_students; i++){
    f >> students[i];
    if(max_heigth < students[i])
      max_heigth = students[i];
    adjacency_matrix[i] = new int[n_of_students];
    for(int j = 0 ; j < n_of_students; j++)
      adjacency_matrix[i][j] = 0;
  }

  for(int i= 0; i < n_of_safe_pairs; i++){
    int a, b;
    f >> a >> b;
    adjacency_matrix[a-1][b-1] = 1;
    adjacency_matrix[b-1][a-1] = 1;
  }

  int *solution = new int[n_of_students];
  int *placed = new int[n_of_students];
  int continue_looping = 1;

  while(continue_looping){
    for(int i= 0; i < n_of_students; i++)
      placed[i] = 0;
    int restart = 0;
    int index = (int)((double)rand()/RAND_MAX * n_of_students); 
    solution[0] = index;
    placed[index] = 1;
    for(int i= 1; i < n_of_students; i++){
      while(1){
        int timeout = 0;
        int current = (int)((double)rand()/RAND_MAX * n_of_students);
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
    int *reverse = new int[n_of_students];
    for(int i= 0 ; i < n_of_students ; i++)
      reverse[i] = solution[n_of_students-1-i];
    int reverse_cost = cost_function(students,reverse,n_of_students);
    int cost = cost_function(students,solution,n_of_students);
    if(reverse_cost < cost){
      memcpy(solution,reverse,n_of_students*sizeof(int));
    }
    free(reverse);
  }
  int probability = 0;
  int cost = cost_function(students,solution,n_of_students);
  while(time(0)-current_time < global_timeout){
    int* potentiel = new int[n_of_students];
    memcpy(potentiel,solution,n_of_students*sizeof(int));
    int retries = n_of_students;
    while(retries--){
      int a = (int)((double) rand()/RAND_MAX * n_of_students);
      while(a >= n_of_students-2)
        a = (int)((double)rand()/RAND_MAX * n_of_students);
      int b = (int)((double)rand()/RAND_MAX * n_of_students);
      int restart = 0;
      for(int i= 0 ; i < n_of_students ; i++)
        if(//students[potentiel[b]] < students[potentiel[a]]
                potentiel[b] < potentiel[a]
              || b == 0){
          b = (int)((double)rand()/RAND_MAX * n_of_students);
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
          int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
          }
      }
      else if(a == 0){
        if(adjacency_matrix[potentiel[a]][potentiel[b-1]] == 1
          && adjacency_matrix[potentiel[a]][potentiel[b+1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a+1]] == 1){
          int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
        }
      }
      else if(b == n_of_students-1){
        if(adjacency_matrix[potentiel[b]][potentiel[a-1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a+1]] == 1
          && adjacency_matrix[potentiel[a]][potentiel[b-1]] == 1){
          int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
        }
      }
      else if( a > 0 && b < n_of_students-1){
        if( adjacency_matrix[potentiel[a]][potentiel[b-1]] == 1
          && adjacency_matrix[potentiel[a]][potentiel[b+1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a-1]] == 1
          && adjacency_matrix[potentiel[b]][potentiel[a+1]] == 1){
          int temp = potentiel[a];
          potentiel[a] = potentiel[b];
          potentiel[b] = temp;
          break;
        }
      }
    }
    int *reverse = new int[n_of_students];
    for(int i= 0 ;i < n_of_students ; i++)
      reverse[i] = potentiel[n_of_students-1-i];
    int potentiel_cost = cost_function(students,potentiel,n_of_students);
    int reverse_cost = cost_function(students,reverse,n_of_students);
    if(reverse_cost < potentiel_cost){
      memcpy(potentiel,reverse,n_of_students*sizeof(int));
      potentiel_cost = reverse_cost;
    }
    delete [] reverse;

    cost = cost_function(students,solution,n_of_students);
    if(((double)rand()/RAND_MAX)*global_timeout/(double)(potentiel_cost - cost) > probability*1
      || cost >= potentiel_cost)
      memcpy(solution,potentiel,n_of_students*sizeof(int));

    probability = time(0) - current_time ;
    delete [] potentiel;
    //for(int i= 1 ;i < n_of_students ; i++)
    //  if( adjacency_matrix[solution[i]][solution[i-1]] != 1)
    //    cout << "==============" << endl;
  }

  if(argc > 3)
    if(strcmp(argv[3],"-p") == 0){
      cout << "nombre d etudiant qui peuvent voir " << n_of_students-cost << endl;
      cout << "solution : " << endl;
      for(int i= 0 ; i < n_of_students; i++)
        cout << solution[i]+1 << "\t" << students[solution[i]] << endl;
    }
  return 0;
}
