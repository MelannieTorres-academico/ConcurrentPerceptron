/*
* To run:  g++ tbb.cpp -ltbb -std=c++11
*
*  ./a.out < ./tests/ruben_or.txt  -true
*  ./a.out < ./tests/ruben_linearly_separable.txt -true
*  ./a.out < ./tests/breast_cancer.txt
*  ./a.out < ./tests/ruben_no_solution.txt
*  ./a.out < ./tests/ruben_no_solution_2.txt
*/
#include <iostream>
#include <random>
#include "cppheader.h"
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

const int GRAIN = 10000;

using namespace std;
using namespace tbb;

class test {
private:
  int d, n_size;
	long double *x_test_set, *y_test_set;
  double *weights;
  bool show;

public:
	test(int dim, long double *array_x_test_set, long double *array_y_test_set, int n, double *array_weights, bool flag) :d(dim), x_test_set(array_x_test_set), y_test_set(array_y_test_set), weights(array_weights), show(flag) {}

	void operator() (const blocked_range<int> &r) const {
    for (int i = r.begin(); i < r.end(); i++) {
      double acum = 0;
      for (int j = 0; j < (d+1); j++) {
        acum += x_test_set[i*(d+1) + j]*weights[j];
      }

      y_test_set[i] =  (acum >= 0) ? 1 : 0;
      if(show){
        cout<<"i: "<<i<<" result:" << y_test_set[i]<<endl;
      }
    }
	}
};


    void initialize_weights(double *weights, int d){
      for (int i = 0; i < (d+1); i++) {
        weights[i] = (double)rand() / RAND_MAX;
      }
    }

    int ann_training(int d, double l_rate, long double **x_training_set, long double *y_training_set, double* weights, int m_size, int n_size){
      int error, num_iterations;
      double delta_w, acum, y_hat, y_difference;
      double ms = 0;

      initialize_weights(weights, d);
      error = 1;
      num_iterations = 0;


      while (error > 0 and num_iterations < 100000){
        error = 0;
        num_iterations ++;
        for (int i = 0; i < (m_size); i++) {
            acum = 0;
            for (int j = 0; j < (d+1); j++) {
                acum += x_training_set[i][j] * weights[j];
            }
            y_hat = (acum >= 0)? 1 : 0;
            y_difference = y_training_set[i] - y_hat;

            if (y_difference != 0){
              error++;
            }

            for (int k = 0; k < (d+1); k++) {
                delta_w = (y_training_set[i]-y_hat) * l_rate * x_training_set[i][k];
                weights[k] += delta_w;
              }
            }
          }

          return error;
        }


  int main (int argc, char* argv[]) {
      Timer t;

      int d, m_size, n_size, pos, errors, j;
      string aux;
      double l_rate = 0.01;
      double ms;
      long double **x_training_set;
      long double *y_training_set;
      long double *x_test_set;
      long double *y_test_set;
      double *weights;

      srand (time(NULL));

      //parse
      cin >> d;
      cin >> m_size;
      cin >> n_size;

      x_training_set = (long double**) malloc (m_size*sizeof(long double*));
      for (int i = 0; i<m_size; i++) {
        x_training_set[i] = (long double*) malloc ((d+1)*sizeof(long double));
      }

      y_training_set = (long double*) malloc (m_size*sizeof(long double));

      x_test_set = (long double*) malloc (n_size*(d+1)*sizeof(long double));


      y_test_set = (long double*) malloc (n_size*sizeof(long double));
      weights = (double*) malloc ((d+1)*sizeof(double));

      for (int i = 0; i < m_size; i++) {
        for (j = 0; j < d; j++) {
          getline(cin, aux, ',');
          x_training_set[i][j] = stod(aux);
        }
        x_training_set[i][d]=1.0; //bias
        getline(cin, aux);
        y_training_set[i] = stod(aux);
      }

      for (int i = 0; i < n_size; i++) {
        for (j = 0; j < (d-1); j++) {
          getline(cin, aux, ',');
          x_test_set[i*(d+1) + j]= stod(aux);
        }
        getline(cin, aux);
        x_test_set[i*(d+1) + j] = stod(aux);
        x_test_set[i*(d+1)+d] = 1.0; //bias
      }

      //train
      errors = ann_training(d, l_rate, x_training_set, y_training_set, weights, m_size, n_size);

      //test
      if (errors != 0){
        cout<<"no solution found"<<endl;
      } else{
        ms = 0;
        bool flag = (argc > 1);
        for (int i = 0; i < 10; i++) {
          t.start();
          parallel_for( blocked_range<int>(0, n_size, GRAIN), test(d, x_test_set, y_test_set,  n_size, weights, flag) );
          ms += t.stop();
        }
        cout << "avg time = " << (ms / 10) << " ms\n";
      }

      //free memory
      for (int i = 0; i < m_size; i++) {
        free(x_training_set[i]);
      }
      free(x_training_set);
      free(y_training_set);

      free(x_test_set);
      free(y_test_set);

    return 0;
  }
