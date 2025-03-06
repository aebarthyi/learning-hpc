#include<omp.h>
#include<stdio.h>

static long steps = 1000000000;
double step;

void main(){
    int i,j;
    double x;
    double pi_seq, pi_par, sum = 0.0;
    double delta_sequential, delta_parallel;

    step = 1.0/(double) steps;

    //------------------SEQUENTIAL SECTION---------------------
    sum = 0.0;
    double start_sequential = omp_get_wtime();

    for (i=0; i < steps; i++) {
        x = (i+0.5)*step;
        sum += 4.0 / (1.0+x*x);
    }
    pi_seq = step * sum;

    delta_sequential = omp_get_wtime() - start_sequential;
    //---------------------------------------------------------


    //------------------PARALLEL SECTION-----------------------
    omp_set_num_threads(omp_get_max_threads()); //get maximum number of threads from system
    sum = 0.0;
    double start_parallel = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum) private(x)
    for (i=0; i < steps; i++) {
        x = (i+0.5)*step;
        sum += 4.0 / (1.0+x*x);
    }
    pi_par = step * sum;

    delta_parallel = omp_get_wtime() - start_parallel;
    //---------------------------------------------------------

    printf("PI = %.16g computed on one thread in %.4g seconds\n", pi_seq, delta_sequential);
    printf("PI = %.16g computed on %d threads in %.4g seconds\n", pi_par, omp_get_max_threads(), delta_parallel);
    printf("Speedup = %.3gx\n", delta_sequential/delta_parallel);
}