import java.util.Scanner;
import java.lang.String;
import java.util.*;
import java.util.concurrent.ForkJoinPool;

/* To run:
*  javac *.java
*  java MainJava < ./tests/ruben_or.txt -true
*  java MainJava < ./tests/ruben_linearly_separable.txt -true
*  java MainJava < ./tests/breast_cancer.txt
*  java MainJava < ./tests/ruben_no_solution.txt
*  java MainJava < ./tests/ruben_no_solution_2.txt
*/

class MainJava {
  private static final int MAXTHREADS =  Runtime.getRuntime().availableProcessors();

  public static void main(String[] args) {
    ForkJoinPool pool;
    Perceptron perceptron = new Perceptron();
    double weights[];
    double learning_rate = 0.01;
    double acum = 0;
    double[] x_test_set;
    int result_fork_join[];
    int result_sequential[];
    int result_threads[];
    int limit = 100000;
    int iterations = 10;
    int n_size, dim, i;
    boolean hasSolution;
    long startTime, stopTime;
    Threads threads[];
    int block;


    perceptron.parse();

    if (perceptron.train(learning_rate, limit)){

      weights = perceptron.getWeights();
      x_test_set = perceptron.getXTestSet();
      n_size = perceptron.getNSize();
      dim = perceptron.getDim();
      result_fork_join = new int[n_size];
      result_sequential = new int[n_size];
      result_threads = new int[n_size];

      //sequential

      SequentialTest sequential = new SequentialTest(weights, x_test_set, dim, n_size, result_sequential);

      for (i = 0; i < iterations; i++) {
        startTime = System.currentTimeMillis();
        sequential.test();
        stopTime = System.currentTimeMillis();
        acum +=  (stopTime - startTime);
      }
      System.out.println("Sequential:\n  avg time: "+(acum/iterations));

      if (args.length>0 && args[0].equals("-true")) {
        result_sequential = sequential.getResults();
        System.out.print("  ");
        for (i = 0; i < n_size; i++) {
          System.out.print(result_sequential[i]+" ");
        }
        System.out.println();
      }
       //fork join

      pool = new ForkJoinPool(MAXTHREADS);

      ForkJoinTest forkjoin = new ForkJoinTest(weights, x_test_set, dim, 0, n_size, result_fork_join);
      acum = 0;
      for (i = 0; i < iterations; i++){
        startTime = System.currentTimeMillis();
        pool.invoke(forkjoin);
        stopTime = System.currentTimeMillis();
  			acum +=  (stopTime - startTime);
      }

      System.out.println("\nFork Join:\n  avg time: "+(acum/iterations));
      if (args.length>0 && args[0].equals("-true")) {
        result_sequential = sequential.getResults();
        System.out.print("  ");
        for(i = 0; i < n_size ;i++){
          System.out.print(result_sequential[i]+" ");
        }
      }


      //threads


  		block = n_size / MAXTHREADS;
  		threads = new Threads[MAXTHREADS];

  		acum = 0;
  		for (int j = 0; j < iterations; j++) {
  			for (i = 0; i < threads.length; i++) {
  				if (i != threads.length - 1) {
  					threads[i] = new Threads(weights, x_test_set, dim, (i * block), ((i + 1) * block), result_threads);
  				} else {
  					threads[i] = new Threads(weights, x_test_set, dim, (i * block), n_size, result_threads);
  				}
  			}

  			startTime = System.currentTimeMillis();
  			for (i = 0; i < MAXTHREADS; i++) {
  				threads[i].start();
  			}
  			for (i = 0; i < MAXTHREADS; i++) {
  				try {
  					threads[i].join();
  				} catch (InterruptedException e) {
  					e.printStackTrace();
  				}
  			}
  			stopTime = System.currentTimeMillis();
  			acum +=  (stopTime - startTime);
  		}

  		System.out.println("\nThreads:\n  avg time: "+ (acum / iterations));
      if (args.length>0 && args[0].equals("-true")) {
        result_sequential = sequential.getResults();
        System.out.print("  ");
        for(i = 0; i < n_size; i++){
          System.out.print(result_threads[i]+" ");
        }
      }

    } else {
      System.out.println("There's no solution");
    }
  }
}
