#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void kernel_eliminate_8(double *A_pivot, double *A_target, int jj,
                        double factor, const int SIZE) {
  register __m256d factor_vec;
  register __m256d pivot_vec_1, pivot_vec_2;
  register __m256d target_vec_1, target_vec_2;

  factor_vec = _mm256_set1_pd(factor);

  pivot_vec_1 = _mm256_loadu_pd(&A_pivot[jj]);
  pivot_vec_2 = _mm256_loadu_pd(&A_pivot[jj + 4]);

  target_vec_1 = _mm256_loadu_pd(&A_target[jj]);
  target_vec_2 = _mm256_loadu_pd(&A_target[jj + 4]);

  target_vec_1 = _mm256_fnmadd_pd(factor_vec, pivot_vec_1, target_vec_1);
  target_vec_2 = _mm256_fnmadd_pd(factor_vec, pivot_vec_2, target_vec_2);

  _mm256_storeu_pd(&A_target[jj], target_vec_1);
  _mm256_storeu_pd(&A_target[jj + 4], target_vec_2);
}

int gaussian_elimination(double **A, double *b, double *x, const int SIZE) {
  double factor;
  int i, j, k, jj;

  // Forward elimination
  for (k = 0; k < SIZE - 1; k++) {
    for (i = k + 1; i < SIZE; i++) {
      factor = A[i][k] / A[k][k];
      for (jj = k; jj <= SIZE - 8; jj += 8) {
        kernel_eliminate_8(A[k], A[i], jj, factor, SIZE);
      }
      for (j = jj; j < SIZE; j++) A[i][j] -= factor * A[k][j];
      b[i] -= factor * b[k];
    }
  }

  // Back substitution
  for (int i = SIZE - 1; i >= 0; i--) {
    x[i] = b[i];
    for (int j = i + 1; j < SIZE; j++) {
      x[i] -= A[i][j] * x[j];
    }
    x[i] /= A[i][i];
  }

  return 0;  // Success
}

/* Adapted from the bl2_clock() routine in the BLIS library */
static double gtod_ref_time_sec = 0.0;

double dclock() {
  double the_time, norm_sec;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (gtod_ref_time_sec == 0.0) gtod_ref_time_sec = (double)tv.tv_sec;
  norm_sec = (double)tv.tv_sec - gtod_ref_time_sec;
  the_time = norm_sec + tv.tv_usec * 1.0e-6;
  return the_time;
}

int verify_solution(double *x, double *true_x, const int SIZE) {
  double tolerance = 1e-6;

  for (int i = 0; i < SIZE; i++) {
    if (fabs(x[i] - true_x[i]) > tolerance) return 1;  // Verification failed
  }
  return 0;  // Verification successful
}

void generate_solvable_system(double **A, double *b, double *true_x,
                              const int SIZE) {
  for (int i = 0; i < SIZE; i++) {
    true_x[i] = ((double)rand() / RAND_MAX) * 20.0 - 10.0;
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      A[i][j] = ((double)rand() / RAND_MAX) * 20.0 - 10.0;
    }
  }

  for (int i = 0; i < SIZE; i++) {
    b[i] = 0.0;
    for (int j = 0; j < SIZE; j++) {
      b[i] += A[i][j] * true_x[j];
    }
  }
}

void print_matrix(double **A, const int SIZE) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%10.4f ", A[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, const char *argv[]) {
  srand(time(NULL));
  int iret;
  double dtime;

  int SIZE = atoi(argv[1]);
  if (SIZE <= 0) {
    fprintf(stderr, "Invalid matrix size: %d\n", SIZE);
    return EXIT_FAILURE;
  }

  double *A_ = (double *)malloc(SIZE * SIZE * sizeof(double));
  double **A = (double **)malloc(SIZE * sizeof(double *));
  for (int i = 0; i < SIZE; i++) A[i] = A_ + i * SIZE;
  double *b = (double *)malloc(SIZE * sizeof(double));
  double *x = (double *)malloc(SIZE * sizeof(double));
  double *true_x = malloc(SIZE * sizeof(double));

  generate_solvable_system(A, b, true_x, SIZE);

  dtime = dclock();
  iret = gaussian_elimination(A, b, x, SIZE);
  dtime = dclock() - dtime;
  printf("Time (seconds): %le \n", dtime);
  if (iret != 0) {
    fprintf(stderr, "Gaussian elimination failed.\n");
  } else if (verify_solution(x, true_x, SIZE) != 0) {
    fprintf(stderr, "Solution verification failed.\n");
    iret = EXIT_FAILURE;
  }

  fflush(stdout);

  // cleanup
  free(A_);
  free(A);
  free(b);
  free(x);
  free(true_x);

  return iret;
}
