#include <iostream>
#include <algorithm>
#include <cstring>
#include <omp.h>
#include <random>
#include <vector>
using namespace std;

vector<int> get_random_int_vector(int n) {
  default_random_engine re{random_device{}()};
  uniform_int_distribution<int> next_rand{INT32_MIN, INT32_MAX};
  vector<int> v(n);
  for (auto &num : v) {
    num = next_rand(re);
  }
  return v;
}

void insertion_sort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

inline void merge(const int *__restrict__ a, const int *__restrict__ b,
                  int *__restrict__ c, const int a_size, const int b_size) {
    int idx_a = 0, idx_b = 0;
    int total_size = a_size + b_size; // Calculate the total size for the loop condition

    for (int i = 0; i < total_size; ++i) {
        if (idx_a == a_size) {
            c[i] = b[idx_b++];
        } else if (idx_b == b_size) {
            c[i] = a[idx_a++];
        } else {
            c[i] = (a[idx_a] < b[idx_b]) ? a[idx_a++] : b[idx_b++];
        }
    }
}

void merge_sort_optimized(int *arr, int *aux, int n) {
    if (n <= 10) {
        insertion_sort(arr, n);
        return;
    }

    int size_a = n / 2;
    int size_b = n - size_a;

    #pragma omp task shared(arr)
    merge_sort_optimized(arr, aux, size_a);

    #pragma omp task shared(arr)
    merge_sort_optimized(arr + size_a, aux + size_a, size_b);

    #pragma omp taskwait
    merge(arr, arr + size_a, aux, size_a, size_b);
    std::copy(aux, aux + n, arr);
}

void merge_sort(int *arr, int n) {
    if (n > 1) {
        if (n < 32) { // insertion sort for small arrays
            insertion_sort(arr, n);
            return;
        }

        const int size_a = n / 2;
        const int size_b = n - size_a;

        #pragma omp task if (size_a > 10000) // parallelize for large subarrays
        merge_sort(arr, size_a);

        merge_sort(arr + size_a, size_b);

        #pragma omp taskwait

        if (n < 8192) { // stack allocation for small arrays
            int* c = (int*) alloca(n * sizeof(int));
            merge(arr, arr + size_a, c, size_a, size_b);
            memcpy(arr, c, sizeof(int) * n);
            return;
        }

        // heap allocation for larger arrays
        int *c = new int[n];
        merge(arr, arr + size_a, c, size_a, size_b);
        memcpy(arr, c, sizeof(int) * n);
        delete[] c;
    }
}

void merge_sort_run(int *arr, int n) {
    #pragma omp parallel
    #pragma omp single nowait
    merge_sort(arr, n);
}


int main(int argc, char *argv[]) {
  const int n = 100000000;
  vector<int> v = get_random_int_vector(n);
  vector<int> v_copy = v;

    double start = omp_get_wtime();
    merge_sort_run(v.data(), n); // use merge_sort_run for parallel execution
    cout << "Optimized merge sort: " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  sort(begin(v_copy), end(v_copy));
  cout << "std::sort: " << omp_get_wtime() - start << " seconds" << endl;

  if (v != v_copy) {
    cout << "Sort implementation is buggy\n";
  } else {
    cout << "Sort implementation is correct\n";
  }

  return 0;
}
