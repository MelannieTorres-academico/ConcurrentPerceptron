/*
This code was made by @Sharatgm
 */
import java.lang.String;
import java.util.*;

class Perceptron {
  private double[] weights;
  private double[][] x_train_set;
  private double[] y_train_set;
  private double[] x_test_set;
  private int m_size;
  private int n_size;
  private int dim;

  // Constructor
  public Perceptron() {}

  // Getters
  public double[] getWeights() {
    return weights;
  }

  public double[] getXTestSet() {
    return x_test_set;
  }

  public int getNSize(){
    return n_size;
  }
  public int getDim(){
    return dim;
  }


  // Parse input
  public void parse(){
    Scanner input = new Scanner(System.in);
    Random r = new Random();

    // Dimension
    dim = input.nextInt();
    input.nextLine(); //clear buffer
    weights = new double[dim+1];

    // Train set size
    m_size = input.nextInt();
    x_train_set = new double[m_size][dim+1];
    y_train_set = new double[m_size];
    input.nextLine(); //clear buffer

    // Test set size
    n_size = input.nextInt();
    x_test_set = new double[n_size * (dim+1)];
    input.nextLine(); //clear buffer

    // Fill in x and y train sets
    String line;
    for(int i=0; i<m_size; i++){
      line = input.nextLine();
      String[] aux_line = line.split(",");
      for(int j=0; j<dim+1; j++){
        if(j == dim) x_train_set[i][j] = (double)1.0;
        else x_train_set[i][j] = Double.parseDouble(aux_line[j]);
      }
      y_train_set[i] = Double.parseDouble(aux_line[dim]);
    }

    // Fill in x test set
    for(int i=0; i<n_size; i++){
      line = input.nextLine();
      String[] aux_line = line.split(",");

      for(int j=0; j<dim; j++){
        x_test_set[i*(dim+1) + j] = Double.parseDouble(aux_line[j]); //i*dim + j
      }
      x_test_set[i*(dim+1)+dim]=1.0;
    }

    double random;
    // Fill weights
    for(int i = 0; i < dim+1; i++) {
      random = 0 + r.nextDouble() * (1 - 0);
      weights[i] = random;
    }
  }

 public boolean train(double learning_rate, int limit) {


    int error = 1;
    int iterations = 0;
    double acum, difference, delta_w;
    int y_hat;

    //this.printWeights();

    while(error > 0 && iterations < limit){
      error = 0;
      iterations++;

      for(int i = 0; i < m_size; i++){
        acum = 0;
        for(int j = 0; j < dim+1; j++){
          acum += x_train_set[i][j] * weights[j];
        }
        if(acum >= 0) y_hat = 1;
        else y_hat = 0;

        difference = y_train_set[i] - y_hat;
        //System.out.println("Difference = "+ difference);
        if(difference != 0) error +=1;

        for(int j = 0; j < dim + 1; j++) {
          delta_w = (y_train_set[i] - y_hat) * learning_rate * x_train_set[i][j];
          //System.out.println("DeltaW = "+ delta_w);
          weights[j] += delta_w;
        }
      }
    }

    if(error != 0) {
      return false;
    } else {
      return true;
    }
  }

}
