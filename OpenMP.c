#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h>

int main() {
    double parallel=0;

    clock_t startTime,endTime;
    startTime=clock();

    int num_processes = 1;
    int array_size;

    printf("Enter the size of the array: ");
    scanf("%d", &array_size);

    double* data = (double*)malloc(array_size * sizeof(double));
    double local_sum = 0.0, total_sum = 0.0;
    double mean, squared_diff = 0.0, variance, std_deviation;
    double speedup,efficiency;

    printf("Enter the array elements:\n");
    for (int i = 0; i < array_size; i++) {
        scanf("%lf", &data[i]);
    }

    // Initialize OpenMP
    omp_set_num_threads(num_processes);
//to calculate the sum to get mean
#pragma omp parallel reduction(+:local_sum)
    {

        int rank = omp_get_thread_num();
        int chunk_size = array_size / num_processes;//1
        int start_index = rank * chunk_size;
        int end_index = start_index + chunk_size;
        printf("process%d : %d ---> %d\n",omp_get_thread_num(),start_index,end_index);
        // Calculate local sum
        for (int i = start_index; i < end_index; i++) {
            local_sum += data[i];
        }

        // Send remainder to the last process (rank n-1)
        if (rank == num_processes - 1) {
            int remainder = array_size % num_processes;
            if (remainder != 0) {
                for (int i = array_size - remainder; i < array_size; i++) {
                    local_sum += data[i];
                }
            }
        }
        total_sum+=local_sum;
    }
    printf("--------------------------------------------------------------------\n");
    mean = total_sum / array_size;
    printf("process master %d:     --->  mean %f\n", omp_get_thread_num(),mean);
    printf("--------------------------------------------------------------------\n");

#pragma omp parallel for reduction(+:squared_diff)
    for (int i = 0; i < array_size; i++) {
        printf("process %d:     ---> index %d \n", omp_get_thread_num(),i);
        double diff = data[i] - mean;
        squared_diff += (diff * diff);
    }

// Calculate variance and standard deviation
    variance = squared_diff / array_size;
    std_deviation = sqrt(variance);

    // Print results (master process only)
    printf("--------------------------------------------------------------------\n");
    printf("--------------------------------------------------------------------\n");
    printf("Mean: %.2f\n", mean);
    printf("Variance: %.2f\n", variance);
    printf("Standard Deviation: %.2f\n", std_deviation);

    endTime=clock();
    parallel=(endTime-startTime);
//    speedup=serial/parallel;
//    efficiency=speedup/num_processes;

    printf("--------------------------------------------------------------------\n");
    // printf("the serialTime = %.2f\n", serial);
    printf("the parallelTime = %.2f\n", parallel);
    // printf("the speedup = %.2f\n", speedup);
    // printf("the efficiency = %.2f\n", efficiency);
    printf("--------------------------------------------------------------------\n");
    printf("--------------------------------------------------------------------\n");
    // Clean up
    free(data);

    return 0;
}
