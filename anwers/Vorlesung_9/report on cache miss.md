# Matrix-Matrix Multiplication through Dot Products

- Perform operation C += A * B where the sizes of the three matrices are given through the integers M, N, K:
  - A: M rows by K columns
  - B: K rows by N columns
  - C: M rows by N columns
- All matrices are stored in row-major format

## Dot product implementation (mnk):

```cpp
for (std::size_t l_m = 0; l_m < i_m; l_m++ ) {
    for( std::size_t l_n = 0; l_n < i_n; l_n++ ) {
        for( std::size_t l_k = 0; l_k < i_k; l_k++ ) {
            io_c[l_m*i_n + l_n] += i_a[l_m*i_k + l_k] * i_b[l_k*i_n + l_n];
        }
    }
}

# Small Cache: Dot Product (L=1, mnk)

- Scenario:
  - Every cache line holds one value (L = 1)
  - "Loop over m, loop over n, loop over k"
  - Small cache

- Cold misses:
  - A: M * K
  - B: N * K
  - C: M * N

- Capacity misses:
  - A: (N - 1) * M * K
  - B: (M - 1) * N * K
  - C: none

- Occurrence of capacity misses:
  - A: Once a row of A does not remain in cache
  - B: Much sooner than in the A-case as remainder of B is touched until potential re-use

# Small Cache: Dot Product (L>1, mnk)

- Scenario:
  - Every cache line holds \( L \geq 1 \) values
  - "Loop over m, loop over n, loop over k"
  - Small cache

- Cold misses:
  - A: \( \frac{M \cdot K}{L} \)
  - B: \( \frac{N \cdot K}{L} \)
  - C: \( \frac{M \cdot N}{L} \)

- Capacity misses:
  - A: \( \frac{(N - 1) \cdot (M \cdot K)}{L} \)
  - B: \( \frac{M \cdot N \cdot K - (N \cdot K)}{L} \)
  - C: none

```cpp
for( std::size_t l_m = 0; l_m < i_m; l_m++ ) {
    for( std::size_t l_n = 0; l_n < i_n; l_n++ ) {
        for( std::size_t l_k = 0; l_k < i_k; l_k++ ) {
            io_c[l_m*i_n + l_n] += i_a[l_m*i_k + l_k] * i_b[l_k*i_n + l_n];
        }
    }
}

## Summary of Cache Behavior in Matrix-Matrix Multiplication

The discussed content focuses on the impact of cache size and organization on the performance of matrix-matrix multiplication, specifically when implementing the dot product with a small cache scenario where each cache line accommodates more than one value. The iteration order follows 'm', 'n', and then 'k' across the matrices. 

**Cold Misses**:
- Occur on the first access of data, resulting in compulsory cache misses for all three matrices. The number of cold misses is determined by the dimensions of the matrices and the size of the cache line \( L \).

**Capacity Misses**:
- Arise when the cache is too small to hold all the active data, leading to evictions and subsequent reloads. Matrix A experiences capacity misses when a row is no longer retained in the cache, whereas matrix B encounters them more frequently due to its particular pattern of access.


