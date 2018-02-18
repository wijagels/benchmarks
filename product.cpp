#include <benchmark/benchmark.h>
#include <boost/align/aligned_allocator.hpp>

#include <algorithm>
#include <parallel/algorithm>
#include <random>
#include <utility>

template <typename T>
static void std_transform(const T *av, const T *bv, T *outv, size_t length) {
  std::transform(av, av + length, bv, outv, std::multiplies<T>{});
}

template <typename T>
static void parallel_transform(const T *av, const T *bv, T *outv, size_t length) {
  __gnu_parallel::transform(av, av + length, bv, outv, std::multiplies<T>{});
}

template <typename T, void (*F)(const T *, const T *, T *, size_t)>
static void BM_Product(benchmark::State &state) {
  const size_t nums = state.range(0);
  state.SetComplexityN(nums);
  std::random_device rd;
  std::mt19937_64 gen{rd()};
  std::vector<T, boost::alignment::aligned_allocator<T, 64>> a;
  std::vector<T, boost::alignment::aligned_allocator<T, 64>> b;
  a.reserve(nums);
  b.reserve(nums);
  std::uniform_real_distribution<T> dis;
  for (size_t i = 0; i < nums; i++) {
    a.push_back(dis(gen));
    b.push_back(dis(gen));
  }
  std::vector<T, boost::alignment::aligned_allocator<T, 64>> out(nums);
  for (auto _ : state) {
    F(a.data(), b.data(), out.data(), nums);
    benchmark::ClobberMemory();
  }
}

BENCHMARK_TEMPLATE(BM_Product, float, std_transform<float>)
    ->RangeMultiplier(2)
    ->Range(1 << 21, 1 << 27)
    ->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE(BM_Product, float, parallel_transform<float>)
    ->RangeMultiplier(2)
    ->Range(1 << 21, 1 << 27)
    ->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE(BM_Product, long double, std_transform<long double>)
    ->RangeMultiplier(2)
    ->Range(1 << 21, 1 << 27)
    ->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE(BM_Product, long double, parallel_transform<long double>)
    ->RangeMultiplier(2)
    ->Range(1 << 21, 1 << 27)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
