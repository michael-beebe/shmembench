#include "shmembench.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>

enum operation {
  ADD, INC, SET, FETCH, FETCH_ADD, SWAP, CSWAP, AND, OR, XOR, FETCH_NBI,
  PUT, GET, PUT_NBI, GET_NBI, P, G, IPUT, IGET, IBPUT, IBGET,
  SIGNAL, SIGNAL_NBI, SIGNAL_FETCH, SIGNAL_WAIT,
  TEST, WAIT, TEST_ALL, TEST_ANY, TEST_SOME, WAIT_ALL, WAIT_ANY, WAIT_SOME,
  TEST_ALL_VECTOR, TEST_ANY_VECTOR, TEST_SOME_VECTOR,
  WAIT_ALL_VECTOR, WAIT_ANY_VECTOR, WAIT_SOME_VECTOR
};

static const struct {
  const char *name;
  enum operation op;
  int version;
} operations[] = {
  {"shmem_atomic_add", ADD, 14}, {"shmem_atomic_inc", INC, 14},
  {"shmem_atomic_set", SET, 14}, {"shmem_atomic_fetch", FETCH, 14},
  {"shmem_atomic_fetch_add", FETCH_ADD, 14}, {"shmem_atomic_swap", SWAP, 14},
  {"shmem_atomic_compare_swap", CSWAP, 14}, {"shmem_atomic_and", AND, 14},
  {"shmem_atomic_or", OR, 14}, {"shmem_atomic_xor", XOR, 14},
  {"shmem_atomic_fetch_nbi", FETCH_NBI, 15},
  {"shmem_putmem", PUT, 14}, {"shmem_getmem", GET, 14},
  {"shmem_putmem_nbi", PUT_NBI, 14}, {"shmem_getmem_nbi", GET_NBI, 14},
  {"shmem_p", P, 14}, {"shmem_g", G, 14},
  {"shmem_iput", IPUT, 14}, {"shmem_iget", IGET, 14},
  {"shmem_ibput", IBPUT, 16}, {"shmem_ibget", IBGET, 16},
  {"shmem_putmem_signal", SIGNAL, 15},
  {"shmem_putmem_signal_nbi", SIGNAL_NBI, 15},
  {"shmem_signal_fetch", SIGNAL_FETCH, 15},
  {"shmem_signal_wait_until", SIGNAL_WAIT, 15},
  {"shmem_test", TEST, 14}, {"shmem_wait_until", WAIT, 14},
  {"shmem_test_all", TEST_ALL, 15}, {"shmem_test_any", TEST_ANY, 15},
  {"shmem_test_some", TEST_SOME, 15},
  {"shmem_wait_until_all", WAIT_ALL, 15},
  {"shmem_wait_until_any", WAIT_ANY, 15},
  {"shmem_wait_until_some", WAIT_SOME, 15},
  {"shmem_test_all_vector", TEST_ALL_VECTOR, 15},
  {"shmem_test_any_vector", TEST_ANY_VECTOR, 15},
  {"shmem_test_some_vector", TEST_SOME_VECTOR, 15},
  {"shmem_wait_until_all_vector", WAIT_ALL_VECTOR, 15},
  {"shmem_wait_until_any_vector", WAIT_ANY_VECTOR, 15},
  {"shmem_wait_until_some_vector", WAIT_SOME_VECTOR, 15}
};

static void fail(const char *message) {
  fprintf(stderr, "PE %d: %s\n", shmem_my_pe(), message);
  shmem_global_exit(EXIT_FAILURE);
}

static size_t product(size_t a, size_t b) {
  if (b && a > SIZE_MAX / b) fail("workload allocation size overflow");
  return a * b;
}

static void *symmetric(size_t bytes) {
  void *p = shmem_malloc(bytes);
  if (!p) fail("workload symmetric allocation failed");
  return p;
}

static int target(const options *opts, int origin, int slot, int npes) {
  if (!strcmp(opts->target, "self")) return origin;
  if (!strcmp(opts->target, "peer")) return (origin + 1) % npes;
  /* Stable across runtimes and independent of libc rand(). Includes self. */
  uint32_t x = (uint32_t)origin * UINT32_C(747796405) + (uint32_t)slot + 1;
  x = (x ^ (x >> 16)) * UINT32_C(2246822519);
  return (int)((x ^ (x >> 13)) % (uint32_t)npes);
}

static unsigned long atomic_value(enum operation op, uint64_t rounds) {
  if (op == ADD || op == INC || op == FETCH_ADD) return (unsigned long)rounds;
  if (op == FETCH || op == FETCH_NBI) return 7;
  if (op == AND) return rounds ? 3 : 7;
  if (op == XOR) return rounds % 2 ? 7 : 0;
  return rounds ? 7 : 0;
}

/* The legacy atomic entry points use this same checked, reduction-free path. */
bool run_workload(options *opts) {
  bool mode = !strcmp(opts->benchtype, "issue") ||
              !strcmp(opts->benchtype, "quiet") ||
              !strcmp(opts->benchtype, "window");
  bool atomic = !strncmp(opts->bench, "shmem_atomic_", 13);
  if (!mode && !(atomic && !strcmp(opts->benchtype, "latency"))) return false;
  size_t selected;
  for (selected = 0; selected < sizeof(operations) / sizeof(operations[0]); ++selected)
    if (!strcmp(opts->bench, operations[selected].name)) break;
  if (selected == sizeof(operations) / sizeof(operations[0]))
    fail("unsupported checked workload");
  int version = 14;
#ifdef USE_15
  version = 15;
#endif
#ifdef USE_16
  version = 16;
#endif
  if (operations[selected].version > version)
    fail("workload unsupported by selected USE_SHMEM_VERSION");
  enum operation op = operations[selected].op;
  int me = shmem_my_pe(), npes = shmem_n_pes();
  bool observation = op >= SIGNAL_FETCH;
  bool strided = op >= IPUT && op <= IBGET;
  bool scalar = atomic || op == P || op == G ||
                op == SIGNAL_FETCH || op == SIGNAL_WAIT || op == TEST || op == WAIT;
  bool get = op == GET || op == GET_NBI || op == G || op == IGET || op == IBGET;
  bool fetched = op == FETCH || op == FETCH_ADD || op == SWAP || op == CSWAP ||
                 op == FETCH_NBI || op == G || op == SIGNAL_FETCH || op == SIGNAL_WAIT;
  bool waiting = op == WAIT || op == SIGNAL_WAIT || op == WAIT_ALL ||
                 op == WAIT_ANY || op == WAIT_SOME || op >= WAIT_ALL_VECTOR;
  bool any = op == TEST_ANY || op == WAIT_ANY ||
             op == TEST_ANY_VECTOR || op == WAIT_ANY_VECTOR;
  bool some = op == TEST_SOME || op == WAIT_SOME ||
              op == TEST_SOME_VECTOR || op == WAIT_SOME_VECTOR;
  bool issue = !strcmp(opts->benchtype, "issue");
  bool quiet_each = !strcmp(opts->benchtype, "quiet") ||
                    !strcmp(opts->benchtype, "latency");
  if (!observation && strcmp(opts->target, "self") && npes < 2)
    fail("peer/random workloads require at least two PEs");
  if (waiting && !opts->ready) fail("wait workloads require --ready 1");
  if (strided && opts->stride < opts->block_size)
    fail("stride must be at least block-size (nonoverlapping blocks)");
  if ((op == IPUT || op == IGET) && opts->block_size != 1)
    fail("iput/iget require --block-size 1");
  if (opts->mask && op < TEST_ALL) fail("--mask requires an array observation");
  if (!opts->ready && op < TEST) fail("--ready requires a test workload");
  uint64_t rounds = (uint64_t)opts->warmups + (uint64_t)opts->ntimes;
  if (rounds > ULONG_MAX) fail("atomic operand would overflow");

  for (size_t bytes = scalar ? sizeof(unsigned long) : (size_t)opts->min_msg_size;
       bytes <= (scalar ? sizeof(unsigned long) : (size_t)opts->max_msg_size);) {
    size_t elements = bytes / sizeof(unsigned long);
    bool typed = strided || op >= TEST_ALL;
    if (typed && (!elements || bytes % sizeof(unsigned long)))
      fail("typed workloads require byte sizes divisible by sizeof(unsigned long)");
    if (strided && elements % (size_t)opts->block_size)
      fail("payload element count must be divisible by block-size");
    size_t blocks = strided ? elements / (size_t)opts->block_size : 0;
    size_t span = strided ? product((blocks - 1), (size_t)opts->stride) +
                           (size_t)opts->block_size : (bytes + sizeof(unsigned long) - 1) /
                                                               sizeof(unsigned long);
    size_t slots = product((size_t)npes, (size_t)opts->window);
    size_t words = product(slots, span);
    size_t allocation = product(words, sizeof(unsigned long));
    unsigned long *source = symmetric(allocation), *dest = symmetric(allocation);
    unsigned long *returns = symmetric(product((size_t)opts->window, sizeof(*returns)));
    uint64_t *signals = symmetric(product(slots, sizeof(*signals)));
    double *elapsed = symmetric(sizeof(*elapsed));
    size_t *indices = malloc(product(product((size_t)opts->window, span), sizeof(*indices)));
    size_t *observed = malloc(product((size_t)opts->window, sizeof(*observed)));
    int *peers = malloc(product((size_t)opts->window, sizeof(*peers)));
    int *status = calloc(span, sizeof(*status));
    unsigned long *values = malloc(product(span, sizeof(*values)));
    if (!indices || !observed || !peers || !status || !values)
      fail("workload local allocation failed");
    size_t excluded = op >= TEST_ALL ? span * (size_t)opts->mask / 100 : 0;
    size_t eligible = span - excluded;
    if (waiting && (any || some) && !eligible)
      fail("wait any/some requires a nonempty eligible set");
    for (size_t j = 0; j < words; ++j) {
      source[j] = 7;
      dest[j] = atomic ? atomic_value(op, 0) : 0;
    }
    if (!typed && !scalar) memset(source, 0x5a, allocation);
    for (size_t j = 0; j < slots; ++j) signals[j] = observation ? 7 : 0;
    for (size_t j = 0; j < span; ++j) {
      status[j] = j < excluded;
      values[j] = opts->ready ? 7 : 8;
    }
    for (int j = 0; j < opts->window; ++j) peers[j] = target(opts, me, j, npes);
    *elapsed = 0;
    shmem_barrier_all();
    for (uint64_t round = 0; round < rounds; ++round) {
      if (round == (uint64_t)opts->warmups) shmem_barrier_all();
      double start = mysecond();
      for (int j = 0; j < opts->window; ++j) {
        size_t slot = (size_t)me * (size_t)opts->window + (size_t)j;
        unsigned long *s = source + slot * span, *d = dest + slot * span;
        unsigned long *r = returns + j;
#ifdef USE_15
        size_t *ix = indices + (size_t)j * span;
#endif
        int pe = peers[j];
        unsigned long compare = opts->ready ? 7 : 8;
        switch (op) {
          case ADD: shmem_ulong_atomic_add(d, 1, pe); break;
          case INC: shmem_ulong_atomic_inc(d, pe); break;
          case SET: shmem_ulong_atomic_set(d, 7, pe); break;
          case FETCH: *r = shmem_ulong_atomic_fetch(d, pe); break;
          case FETCH_ADD: *r = shmem_ulong_atomic_fetch_add(d, 1, pe); break;
          case SWAP: *r = shmem_ulong_atomic_swap(d, 7, pe); break;
          case CSWAP: *r = shmem_ulong_atomic_compare_swap(d, 0, 7, pe); break;
          case AND: shmem_ulong_atomic_and(d, 3, pe); break;
          case OR: shmem_ulong_atomic_or(d, 7, pe); break;
          case XOR: shmem_ulong_atomic_xor(d, 7, pe); break;
          case PUT: shmem_putmem(d, s, bytes, pe); break;
          case GET: shmem_getmem(d, s, bytes, pe); break;
          case PUT_NBI: shmem_putmem_nbi(d, s, bytes, pe); break;
          case GET_NBI: shmem_getmem_nbi(d, s, bytes, pe); break;
          case P: shmem_ulong_p(d, 7, pe); break;
          case G: *r = shmem_ulong_g(s, pe); break;
          case IPUT: shmem_ulong_iput(d, s, opts->stride, opts->stride, elements, pe); break;
          case IGET: shmem_ulong_iget(d, s, opts->stride, opts->stride, elements, pe); break;
          case TEST: observed[j] = shmem_ulong_test(s, SHMEM_CMP_EQ, compare); break;
          case WAIT: shmem_ulong_wait_until(s, SHMEM_CMP_EQ, compare); break;
#ifdef USE_15
          case FETCH_NBI: shmem_ulong_atomic_fetch_nbi(r, d, pe); break;
          case SIGNAL: shmem_putmem_signal(d, s, bytes, signals + slot, 1, SHMEM_SIGNAL_ADD, pe); break;
          case SIGNAL_NBI: shmem_putmem_signal_nbi(d, s, bytes, signals + slot, 1, SHMEM_SIGNAL_ADD, pe); break;
          case SIGNAL_FETCH: *r = shmem_signal_fetch(signals + slot); break;
          case SIGNAL_WAIT: *r = shmem_signal_wait_until(signals + slot, SHMEM_CMP_EQ, 7); break;
          case TEST_ALL: observed[j] = shmem_ulong_test_all(s, elements, status, SHMEM_CMP_EQ, compare); break;
          case TEST_ANY: observed[j] = shmem_ulong_test_any(s, elements, status, SHMEM_CMP_EQ, compare); break;
          case TEST_SOME: observed[j] = shmem_ulong_test_some(s, elements, ix, status, SHMEM_CMP_EQ, compare); break;
          case WAIT_ALL: shmem_ulong_wait_until_all(s, elements, status, SHMEM_CMP_EQ, compare); break;
          case WAIT_ANY: observed[j] = shmem_ulong_wait_until_any(s, elements, status, SHMEM_CMP_EQ, compare); break;
          case WAIT_SOME: observed[j] = shmem_ulong_wait_until_some(s, elements, ix, status, SHMEM_CMP_EQ, compare); break;
          case TEST_ALL_VECTOR: observed[j] = shmem_ulong_test_all_vector(s, elements, status, SHMEM_CMP_EQ, values); break;
          case TEST_ANY_VECTOR: observed[j] = shmem_ulong_test_any_vector(s, elements, status, SHMEM_CMP_EQ, values); break;
          case TEST_SOME_VECTOR: observed[j] = shmem_ulong_test_some_vector(s, elements, ix, status, SHMEM_CMP_EQ, values); break;
          case WAIT_ALL_VECTOR: shmem_ulong_wait_until_all_vector(s, elements, status, SHMEM_CMP_EQ, values); break;
          case WAIT_ANY_VECTOR: observed[j] = shmem_ulong_wait_until_any_vector(s, elements, status, SHMEM_CMP_EQ, values); break;
          case WAIT_SOME_VECTOR: observed[j] = shmem_ulong_wait_until_some_vector(s, elements, ix, status, SHMEM_CMP_EQ, values); break;
#endif
#ifdef USE_16
          case IBPUT: shmem_ulong_ibput(d, s, opts->stride, opts->stride, opts->block_size, blocks, pe); break;
          case IBGET: shmem_ulong_ibget(d, s, opts->stride, opts->stride, opts->block_size, blocks, pe); break;
#endif
          default: fail("workload not compiled");
        }
        if (quiet_each) shmem_quiet();
      }
      if (!issue && !quiet_each) shmem_quiet();
      double duration = mysecond() - start;
      if (issue) shmem_quiet();
      if (round >= (uint64_t)opts->warmups) *elapsed += duration;
      for (int j = 0; j < opts->window; ++j) {
        if (fetched && returns[j] != (atomic ? atomic_value(op, round) : 7))
          fail("fetched operand check failed");
        if (op == TEST && observed[j] != (size_t)opts->ready)
          fail("scalar test result check failed");
        if (op == TEST_ALL || op == TEST_ALL_VECTOR) {
          if (observed[j] != (size_t)(opts->ready || !eligible))
            fail("test_all result check failed");
        }
        if (any && ((opts->ready && eligible) ?
                    (observed[j] < excluded || observed[j] >= span) :
                    observed[j] != SIZE_MAX))
          fail("any result check failed");
        if (some) {
          size_t count = observed[j];
          if (count > eligible || ((!opts->ready || !eligible) && count) ||
              (opts->ready && eligible && !count))
            fail("some count check failed");
          for (size_t k = 0; k < count; ++k) {
            size_t index = indices[(size_t)j * span + k];
            if (index < excluded || index >= span) fail("some index check failed");
            if (status[index]) fail("duplicate some index");
            status[index] = 1;
          }
          memset(status + excluded, 0, eligible * sizeof(*status));
        }
      }
    }
    shmem_barrier_all();
    for (int origin = 0; origin < npes; ++origin) {
      for (int j = 0; j < opts->window; ++j) {
        size_t slot = (size_t)origin * (size_t)opts->window + (size_t)j;
        bool touched = target(opts, origin, j, npes) == me;
        if (atomic) {
          if (dest[slot * span] != atomic_value(op, touched ? rounds : 0))
            fail("final atomic operand check failed");
        } else if (!observation && op != G) {
          bool updated = get ? origin == me : touched;
          if (strided || op == P) {
            for (size_t k = 0; k < span; ++k) {
              bool payload = !strided || k % (size_t)opts->stride < (size_t)opts->block_size;
              if (dest[slot * span + k] != (updated && payload ? 7UL : 0UL))
                fail("strided/scalar payload check failed");
            }
          } else {
            unsigned char *d = (unsigned char *)(dest + slot * span);
            for (size_t k = 0; k < bytes; ++k)
              if (d[k] != (updated ? 0x5a : 0)) fail("RMA payload check failed");
          }
#ifdef USE_15
          if ((op == SIGNAL || op == SIGNAL_NBI) &&
              shmem_signal_fetch(signals + slot) != (touched ? rounds : 0))
            fail("signal count check failed");
#endif
        }
      }
    }
    shmem_barrier_all();
    if (!me) {
      for (int pe = 0; pe < npes; ++pe) {
        double seconds;
        shmem_getmem(&seconds, elapsed, sizeof(seconds), pe);
        if (!isfinite(seconds) || seconds <= 0) fail("invalid or zero elapsed time");
        double count = (double)opts->ntimes * opts->window;
        printf("RESULT bench=%s bytes=%zu pe=%d target=%s mode=%s window=%d "
               "ntimes=%d warmup=%d stride=%d block=%d ready=%d mask=%d "
               "us=%.9g ops_per_s=%.9g MiB_per_s=%.9g check=pass\n",
               opts->bench, bytes, pe, observation ? "local" : opts->target,
               !strcmp(opts->benchtype, "latency") ? "quiet" : opts->benchtype,
               opts->window, opts->ntimes, opts->warmups, opts->stride,
               opts->block_size, opts->ready, opts->mask, seconds * 1e6 / count,
               count / seconds, count * bytes / seconds / 1048576.0);
      }
    }
    shmem_barrier_all();
    shmem_free(elapsed);
    shmem_free(signals);
    shmem_free(returns);
    shmem_free(dest);
    shmem_free(source);
    free(values);
    free(status);
    free(peers);
    free(observed);
    free(indices);
    if (scalar || bytes > (size_t)opts->max_msg_size / 2) break;
    bytes *= 2;
  }
  return true;
}
