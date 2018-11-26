class SequentialTest{
  private double[] weights;
  private int[] results;
  private double[] x_test_set;
  private int dim;
  private int n_size;


  public SequentialTest (double weights[], double[] x_test_set, int dim, int n_size, int[] results) {
     this.weights = weights;
     this.x_test_set = x_test_set;
     this.dim = dim;
     this.n_size = n_size;
     this.results = results;
  }


  public void test() {
    int i, j;
    double acum = 0;

    for (i = 0; i < this.n_size; i++) {
      acum = 0;
      for (j = 0; j < dim+1; j++) {
        acum += x_test_set[i*(dim+1) + j]*weights[j];
      }
      results[i] = (acum >= 0)? 1 : 0;
    }
   }

   public int[] getResults(){
     return results;
   }

}
