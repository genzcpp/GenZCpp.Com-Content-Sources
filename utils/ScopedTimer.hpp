#pragma once

#include <chrono>
#include <iostream>

class ScopedTimer {
  public:
    using ClockType = std::chrono::steady_clock;
    ScopedTimer() : function_name_{""}, start_{ClockType::now()} {}
    ScopedTimer(bool isPrint)
        : function_name_{""},
          printOnDestruct(isPrint), start_{ClockType::now()} {}
    ScopedTimer(const char *func)
        : function_name_{func}, start_{ClockType::now()} {}
    ScopedTimer(const ScopedTimer &) = delete;
    ScopedTimer(ScopedTimer &&) = delete;
    auto operator=(const ScopedTimer &) -> ScopedTimer & = delete;
    auto operator=(ScopedTimer &&) -> ScopedTimer & = delete;

    bool printOnDestruct = true;

    long long elapsedMiliseconds(bool isReset = true) {
        using namespace std::chrono;
        auto stop = ClockType::now();
        auto duration = (stop - start_);
        start_ = stop;
        return duration_cast<milliseconds>(duration).count();
    }

    void reset() { start_ = ClockType::now(); }

    ~ScopedTimer() {
        using namespace std::chrono;
        auto stop = ClockType::now();
        auto duration = (stop - start_);
        auto ms = duration_cast<milliseconds>(duration).count();
        if (printOnDestruct) {
            std::cout << ms << " ms " << function_name_ << '\n';
        }
    }

  private:
    const char *function_name_{};
    ClockType::time_point start_{};
};