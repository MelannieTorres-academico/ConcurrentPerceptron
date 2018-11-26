/*
*  To run:
*  g++ sequential.cpp  -std=c++11
*  ./a.out < ./tests/breast_cancer.txt
*  ./a.out < ./tests/ruben_or.txt -true
*  ./a.out < ./tests/ruben_linearly_separable.txt -true
*  ./a.out < ./tests/breast_cancer.txt
*  ./a.out < ./tests/ruben_no_solution.txt
*  ./a.out < ./tests/ruben_no_solution_2.txt
*/
#include <iostream>
#include <random>
#include "cppheader.h"

using namespace std;

    void test(int d, long double *x_test_set, long double *y_test_set, int n_size, double *weights, bool show){
      int i, j;
      double acum, y_hat;

        for (i = 0; i < n_size; i++) {
          acum = 0;
          for (j = 0; j < (d+1); j++) {
            acum += x_test_set[i*(d+1) + j]*weights[j];
          }

          y_test_set[i] = (acum >= 0) ? 1 : 0;
          if(show){
            cout<< y_test_set[i]<<endl;
          }
        }
    }


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
      double l_rate = 0.01, ms = 0;

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
        bool flag = (argc > 1);

        for (int i = 0; i < 10; i++) {
          t.start();
          test(d, x_test_set, y_test_set,  n_size, weights, flag);
          ms += t.stop();
        }

       cout << "Sequential C++ avg time = " << (ms/10) << " ms\n" << endl;

      }

      for (int i = 0; i < m_size; i++) {
        free(x_training_set[i]);
      }
      free(x_training_set);
      free(y_training_set);

      free(x_test_set);
      free(y_test_set);

    return 0;
  }
