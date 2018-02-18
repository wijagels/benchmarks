#include <benchmark/benchmark.h>
#include <boost/align/aligned_allocator.hpp>

#include <numeric>
#include <parallel/algorithm>
#include <random>
#include <utility>

static void BM_StdSort(benchmark::State &state) {
  const size_t nums = state.range(0);
  state.SetComplexityN(nums);
  std::random_device rd;
  std::mt19937_64 gen{rd()};
  std::vector<size_t, boost::alignment::aligned_allocator<size_t, 64>> vec;
  vec.reserve(nums);
  std::uniform_int_distribution<size_t> dis;
  for (size_t i = 0; i < nums; i++) {
    vec.push_back(dis(gen));
  }
  for (auto _ : state) {
    std::sort(vec.begin(), vec.end());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_StdSort)->RangeMultiplier(2)->Range(1 << 10, 1 << 20)->Complexity();

static void BM_ParallelSort(benchmark::State &state) {
  const size_t nums = state.range(0);
  state.SetComplexityN(nums);
  std::random_device rd;
  std::mt19937_64 gen{rd()};
  std::vector<size_t, boost::alignment::aligned_allocator<size_t, 64>> vec;
  vec.reserve(nums);
  std::uniform_int_distribution<size_t> dis;
  for (size_t i = 0; i < nums; i++) {
    vec.push_back(dis(gen));
  }
  for (auto _ : state) {
    __gnu_parallel::sort(vec.begin(), vec.end());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_ParallelSort)->RangeMultiplier(2)->Range(1 << 10, 1 << 20)->Complexity();

BENCHMARK_MAIN();
