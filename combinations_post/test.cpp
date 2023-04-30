#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <type_traits>

#include "CombHelper.hpp"

using namespace std;

namespace {
using CombFunctionPtr = std::add_pointer<ll(ll, ll)>::type;
};

class Solution {
private:
    int cnt[26] = {0};
public:
    int countGoodSubsequences(string s, CombFunctionPtr comb) {
        int mf = 0;
        for (char c : s) {
            int f = ++cnt[c-'a'];
            mf = max(mf, f);
        }

        ll res = 0;
        for (int i = 1; i <= mf; i++) {
            ll cr = 1;
            for (int ci = 0; ci < 26; ci++) {
                if (cnt[ci] < i) {
                    continue;
                }
                cr = mod_mul(cr, (*comb)(cnt[ci], i)+1);
            }
            res = mod_add(res, cr - 1);
        }
        return res;
    }
};


constexpr CompileTimeComb<100001> f1{};
static RuntimeInitCacheComb comb2(100001);
constexpr const char* input = "vjegrusjmearasjqegqesrevdduqaqvsduguqgqvqdgmeeeuqgeuaumrjsejqmgrqgjudegagmjgqrrsresuurusrduuae";

static void TestCompileTimeComb(benchmark::State& state) {
  auto s = Solution();
  CombFunctionPtr combl = +[](ll n, ll k) -> ll {return f1.comb(n, k);};
  for (auto _ : state){
    benchmark::DoNotOptimize(s.countGoodSubsequences(input, std::move(combl)));
  }
}

static void TestRuntimeComb(benchmark::State& state) {
  auto s = Solution();
  CombFunctionPtr combl = +[](ll n, ll k) -> ll {return RuntimeComb::comb(n, k);};
  for (auto _ : state){
    benchmark::DoNotOptimize(s.countGoodSubsequences(input, std::move(combl)));
  }
}

static void TestRuntimeInitWithCacheComb(benchmark::State& state) {
  auto s = Solution();
  CombFunctionPtr combl = +[](ll n, ll k) -> ll {return comb2.comb(n, k);};
  for (auto _ : state){
    benchmark::DoNotOptimize(s.countGoodSubsequences(input, std::move(combl)));
  }
}

//Register benchmarks
BENCHMARK(TestCompileTimeComb)->Unit(benchmark::kMillisecond);
BENCHMARK(TestRuntimeComb)->Unit(benchmark::kMillisecond);
BENCHMARK(TestRuntimeInitWithCacheComb)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();