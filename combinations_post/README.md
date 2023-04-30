# How to compile?

`g++ -o test test.cpp -std=c++20 -O3 -I ${GOOGLE_BENCHMARK_INCLUDE} -L${GOOGLE_BENCHMARK_LIB} -lbenchmark -lpthread -fconstexpr-steps=80000000`

My g++ version:
```
Apple clang version 14.0.3 (clang-1403.0.22.14.1)
Target: arm64-apple-darwin22.3.0
Thread model: posix
```

MY `GOOGLE_BENCHMARK_INCLUDE` and `GOOGLE_BENCHMARK_LIB` are installed from `brew install google-benchmark`
In my `~/.zprofile`
```
export GOOGLE_BENCHMARK_INCLUDE="/opt/homebrew/Cellar/google-benchmark/1.7.1/include"
export GOOGLE_BENCHMARK_LIB="/opt/homebrew/Cellar/google-benchmark/1.7.1/lib"
```

`-lbenchmark -lpthread` allows google-benchmark to work.
`-fconstexpr-steps=80000000` clang flag is needed for compile time version.