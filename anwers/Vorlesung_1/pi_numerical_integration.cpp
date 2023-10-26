#include <iomanip>
#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    int num_steps = 100000000;
    double width = 1.0 / double(num_steps);
    double sum = 0.0;

    double start_time = omp_get_wtime();  // start timing here

    #pragma omp parallel 
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        
        for (int i = thread_id; i < num_steps; i += num_threads) {
            double x = (i + 0.5) * width;
            sum += (1.0 / (1.0 + x * x));
        }

      
    }

    double pi = sum * 4 * width;
    double run_time = omp_get_wtime() - start_time;

    cout << "pi with " << num_steps << " steps is " << setprecision(17)
         << pi << " in " << setprecision(6) << run_time << " seconds\n";
}
