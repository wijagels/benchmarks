#include <benchmark/benchmark.h>
#include <boost/align/aligned_allocator.hpp>

#include <algorithm>
#include <numeric>
#include <parallel/numeric>
#include <random>
#include <utility>

int64_t std_accumulate(int64_t *input, size_t length) {
  return std::accumulate(input, input + length, int64_t{0});
}

int64_t parallel_accumulate(int64_t *input, size_t length) {
  return __gnu_parallel::accumulate(input, input + length, int64_t{0});
}

template <int64_t (*F)(int64_t *, size_t)>
static void BM_Sum(benchmark::State &state) {
  const size_t nums = state.range(0);
  state.SetComplexityN(nums);
  std::random_device rd;
  std::mt19937_64 gen{rd()};
  std::vector<int64_t, boost::alignment::aligned_allocator<int64_t, 64>> vec;
  vec.reserve(nums);
  std::uniform_int_distribution<int64_t> dis;
  for (size_t i = 0; i < nums; i++) {
    vec.push_back(dis(gen));
  }
  for (auto _ : state) {
    benchmark::DoNotOptimize(F(vec.data(), vec.size()));
  }
}

BENCHMARK_TEMPLATE(BM_Sum, std_accumulate)
    ->RangeMultiplier(2)
    ->Range(1 << 20, 1 << 26)
    ->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE(BM_Sum, parallel_accumulate)
    ->RangeMultiplier(2)
    ->Range(1 << 20, 1 << 26)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
