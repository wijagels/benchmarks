cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=5 -Wundef -Wswitch-default)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=skylake -mtune=skylake -funroll-loops -ffast-math")

find_package(benchmark REQUIRED)
find_package(OpenMP REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)

pkg_check_modules(gperftools REQUIRED IMPORTED_TARGET libprofiler)

add_executable(simd_sum simd_sum.cpp)
target_link_libraries(simd_sum PUBLIC benchmark::benchmark OpenMP::OpenMP_CXX PkgConfig::gperftools Boost::boost)

add_executable(sort sort.cpp)
target_link_libraries(sort PUBLIC benchmark::benchmark OpenMP::OpenMP_CXX PkgConfig::gperftools Boost::boost)

add_executable(product product.cpp)
target_link_libraries(product PUBLIC benchmark::benchmark OpenMP::OpenMP_CXX PkgConfig::gperftools Boost::boost)
