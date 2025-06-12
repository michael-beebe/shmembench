/**
  @file datatype_config.h
  @author Michael Beebe (Texas Tech University)
  @brief Data type configuration for benchmarks
 */

#ifndef DATATYPE_CONFIG_H
#define DATATYPE_CONFIG_H

/* Default data type if not specified */
#ifndef BENCHMARK_DATATYPE
#define BENCHMARK_DATATYPE long
#endif

/* Data type size macro */
#define BENCHMARK_DATATYPE_SIZE sizeof(BENCHMARK_DATATYPE)

/* Data type name string macro */
#ifndef BENCHMARK_DATATYPE_NAME
#define BENCHMARK_DATATYPE_NAME "long"
#endif

/* Macro to declare a variable of the benchmark data type */
#define BENCHMARK_TYPE(var) BENCHMARK_DATATYPE var

/* Macro to declare a pointer to the benchmark data type */
#define BENCHMARK_TYPE_PTR(var) BENCHMARK_DATATYPE *var

/* Macro to cast to the benchmark data type pointer */
#define BENCHMARK_TYPE_CAST(ptr) (BENCHMARK_DATATYPE *)(ptr)

/* Macro for memory allocation */
#define BENCHMARK_MALLOC(count) (BENCHMARK_DATATYPE *)shmem_malloc((count) * BENCHMARK_DATATYPE_SIZE)

/* Macro for memory deallocation */
#define BENCHMARK_FREE(ptr) shmem_free(ptr)

/* Macro to initialize an element */
#define BENCHMARK_INIT_ELEMENT(array, index, value) (array)[index] = (BENCHMARK_DATATYPE)(value)

/* Macro to calculate element count from size in bytes */
#define BENCHMARK_ELEM_COUNT(size) ((size) / BENCHMARK_DATATYPE_SIZE)

/* Atomic operation macros */
// clang-format off
#if defined(BENCHMARK_DATATYPE) && BENCHMARK_DATATYPE == int
    #define BENCHMARK_ATOMIC_ADD(dest, value, pe) shmem_atomic_add(dest, value, pe)
    #define BENCHMARK_ATOMIC_INC(dest, pe) shmem_atomic_inc(dest, pe)
    #define BENCHMARK_ATOMIC_FETCH(dest, pe) shmem_atomic_fetch(dest, pe)
    #define BENCHMARK_ATOMIC_SET(dest, value, pe) shmem_atomic_set(dest, value, pe)
    #define BENCHMARK_ATOMIC_SWAP(dest, value, pe) shmem_atomic_swap(dest, value, pe)
    #define BENCHMARK_ATOMIC_COMPARE_SWAP(dest, cond, value, pe) shmem_atomic_compare_swap(dest, cond, value, pe)
    #define BENCHMARK_ATOMIC_FETCH_NBI(dest, source, pe) shmem_atomic_fetch_nbi(dest, source, pe)
#elif defined(BENCHMARK_DATATYPE) && BENCHMARK_DATATYPE == long
    #define BENCHMARK_ATOMIC_ADD(dest, value, pe) shmem_atomic_add(dest, value, pe)
    #define BENCHMARK_ATOMIC_INC(dest, pe) shmem_atomic_inc(dest, pe)
    #define BENCHMARK_ATOMIC_FETCH(dest, pe) shmem_atomic_fetch(dest, pe)
    #define BENCHMARK_ATOMIC_SET(dest, value, pe) shmem_atomic_set(dest, value, pe)
    #define BENCHMARK_ATOMIC_SWAP(dest, value, pe) shmem_atomic_swap(dest, value, pe)
    #define BENCHMARK_ATOMIC_COMPARE_SWAP(dest, cond, value, pe) shmem_atomic_compare_swap(dest, cond, value, pe)
    #define BENCHMARK_ATOMIC_FETCH_NBI(dest, source, pe) shmem_atomic_fetch_nbi(dest, source, pe)
#elif defined(BENCHMARK_DATATYPE) && BENCHMARK_DATATYPE == double
    #define BENCHMARK_ATOMIC_ADD(dest, value, pe) shmem_atomic_add(dest, value, pe)
    #define BENCHMARK_ATOMIC_INC(dest, pe) shmem_atomic_inc(dest, pe)
    #define BENCHMARK_ATOMIC_FETCH(dest, pe) shmem_atomic_fetch(dest, pe)
    #define BENCHMARK_ATOMIC_SET(dest, value, pe) shmem_atomic_set(dest, value, pe)
    #define BENCHMARK_ATOMIC_SWAP(dest, value, pe) shmem_atomic_swap(dest, value, pe)
    #define BENCHMARK_ATOMIC_COMPARE_SWAP(dest, cond, value, pe) shmem_atomic_compare_swap(dest, cond, value, pe)
    #define BENCHMARK_ATOMIC_FETCH_NBI(dest, source, pe) shmem_atomic_fetch_nbi(dest, source, pe)
#elif defined(BENCHMARK_DATATYPE) && BENCHMARK_DATATYPE == float
    #define BENCHMARK_ATOMIC_ADD(dest, value, pe) shmem_atomic_add(dest, value, pe)
    #define BENCHMARK_ATOMIC_INC(dest, pe) shmem_atomic_inc(dest, pe)
    #define BENCHMARK_ATOMIC_FETCH(dest, pe) shmem_atomic_fetch(dest, pe)
    #define BENCHMARK_ATOMIC_SET(dest, value, pe) shmem_atomic_set(dest, value, pe)
    #define BENCHMARK_ATOMIC_SWAP(dest, value, pe) shmem_atomic_swap(dest, value, pe)
    #define BENCHMARK_ATOMIC_COMPARE_SWAP(dest, cond, value, pe) shmem_atomic_compare_swap(dest, cond, value, pe)
    #define BENCHMARK_ATOMIC_FETCH_NBI(dest, source, pe) shmem_atomic_fetch_nbi(dest, source, pe)
#elif defined(BENCHMARK_DATATYPE) && BENCHMARK_DATATYPE == char
    #define BENCHMARK_ATOMIC_ADD(dest, value, pe) shmem_atomic_add(dest, value, pe)
    #define BENCHMARK_ATOMIC_INC(dest, pe) shmem_atomic_inc(dest, pe)
    #define BENCHMARK_ATOMIC_FETCH(dest, pe) shmem_atomic_fetch(dest, pe)
    #define BENCHMARK_ATOMIC_SET(dest, value, pe) shmem_atomic_set(dest, value, pe)
    #define BENCHMARK_ATOMIC_SWAP(dest, value, pe) shmem_atomic_swap(dest, value, pe)
    #define BENCHMARK_ATOMIC_COMPARE_SWAP(dest, cond, value, pe) shmem_atomic_compare_swap(dest, cond, value, pe)
    #define BENCHMARK_ATOMIC_FETCH_NBI(dest, source, pe) shmem_atomic_fetch_nbi(dest, source, pe)
#elif defined(BENCHMARK_DATATYPE) && BENCHMARK_DATATYPE == short
    #define BENCHMARK_ATOMIC_ADD(dest, value, pe) shmem_atomic_add(dest, value, pe)
    #define BENCHMARK_ATOMIC_INC(dest, pe) shmem_atomic_inc(dest, pe)
    #define BENCHMARK_ATOMIC_FETCH(dest, pe) shmem_atomic_fetch(dest, pe)
    #define BENCHMARK_ATOMIC_SET(dest, value, pe) shmem_atomic_set(dest, value, pe)
    #define BENCHMARK_ATOMIC_SWAP(dest, value, pe) shmem_atomic_swap(dest, value, pe)
    #define BENCHMARK_ATOMIC_COMPARE_SWAP(dest, cond, value, pe) shmem_atomic_compare_swap(dest, cond, value, pe)
    #define BENCHMARK_ATOMIC_FETCH_NBI(dest, source, pe) shmem_atomic_fetch_nbi(dest, source, pe)
#else
    /* Default to long for unsupported types */
    #define BENCHMARK_ATOMIC_ADD(dest, value, pe) shmem_atomic_add(dest, value, pe)
    #define BENCHMARK_ATOMIC_INC(dest, pe) shmem_atomic_inc(dest, pe)
    #define BENCHMARK_ATOMIC_FETCH(dest, pe) shmem_atomic_fetch(dest, pe)
    #define BENCHMARK_ATOMIC_SET(dest, value, pe) shmem_atomic_set(dest, value, pe)
    #define BENCHMARK_ATOMIC_SWAP(dest, value, pe) shmem_atomic_swap(dest, value, pe)
    #define BENCHMARK_ATOMIC_COMPARE_SWAP(dest, cond, value, pe) shmem_atomic_compare_swap(dest, cond, value, pe)
    #define BENCHMARK_ATOMIC_FETCH_NBI(dest, source, pe) shmem_atomic_fetch_nbi(dest, source, pe)
#endif
// clang-format on

#endif /* DATATYPE_CONFIG_H */