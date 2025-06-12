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

/* Macro to allocate memory for the benchmark data type */
#define BENCHMARK_MALLOC(size)                                                 \
  BENCHMARK_TYPE_CAST(shmem_malloc((size) * BENCHMARK_DATATYPE_SIZE))

/* Macro to free memory */
#define BENCHMARK_FREE(ptr) shmem_free(ptr)

/* Macro to initialize array elements */
#define BENCHMARK_INIT_ELEMENT(array, index, value)                            \
  ((array)[index] = (BENCHMARK_DATATYPE)(value))

/* Macro to get the number of elements for a given byte size */
#define BENCHMARK_ELEM_COUNT(byte_size) ((byte_size) / BENCHMARK_DATATYPE_SIZE)

/* Macro to get the actual byte size for a given number of elements */
#define BENCHMARK_BYTE_SIZE(elem_count) ((elem_count) * BENCHMARK_DATATYPE_SIZE)

#endif /* DATATYPE_CONFIG_H */