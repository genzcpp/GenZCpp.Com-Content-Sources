#include <string>
#include <iostream>

using namespace std;
using ll = long long;

constexpr ll MOD = 1e9+7;
constexpr ll mod_add(ll a, ll b) {
    return (a % MOD + b % MOD) % MOD;
}
    
constexpr ll mod_mul(ll a, ll b) {
    return ((a % MOD) * (b % MOD)) % MOD;
}

template<int N>
class CompileTimeComb {
public:
    constexpr CompileTimeComb() {
        std::fill_n(begin(facts), size(facts), -1);
        std::fill_n(begin(inv_fact), size(inv_fact), -1);
        buildfact();
        for (int k = 0; k < N; k++) {
            buildInvFact(k);
        }
    }
    ll fact(ll x) {
        if (x <= 1) {
            return 1;
        }
        if (x >= N) {
            throw std::runtime_error("out of bound: " + to_string(x));
        }
        return facts[x];
    }

    ll comb(ll n, ll k) const {
        assert(n < N);
        return mod_mul(facts[n], mod_mul(inv_fact[k], inv_fact[n-k]));
    }
private:
    ll facts[N] = {0};
    ll inv_fact[N] = {0};

    constexpr void buildfact() {
        for (int i = 0; i < N; i++) {
            facts[i] = i<=1 ? 1 : mod_mul(i, facts[i-1]);
        }
    }

    constexpr ll inv(ll i) {
        if (i > MOD) {
            i = i % MOD;
        }
        if (i <= 1) {
            return i;
        }
        if (i < N && inv_fact[i] != -1) {
            return inv_fact[i];
        }
        auto res = MOD - mod_mul((long long)(MOD / i), inv(MOD % i));
        if (i < N) {
            inv_fact[i] = res;
        }
        return res;
    }

    constexpr ll buildInvFact(ll k) {
        inv_fact[k] = inv(facts[k]);
        return inv_fact[k];
    }
};


class RuntimeComb {
public:
    static ll fact(ll x) {
        if (x <= 1) {
            return 1;
        }
        return x * fact(x - 1);
    }

    static ll inv(ll i) {
        if (i > MOD) {
            i = i % MOD;
        }
        if (i <= 1) {
            return i;
        }
        return MOD - mod_mul((long long)(MOD / i), inv(MOD % i));
    }

    static ll comb(ll n, ll k) {
        return mod_mul(fact(n), mod_mul(inv(k), inv(n-k)));
    }
};

class RuntimeInitCacheComb {
public:
    size_t N;

    RuntimeInitCacheComb() = default;
    RuntimeInitCacheComb(size_t n) : N(n){
        facts.resize(N);
        inv_fact.resize(N);
        std::fill_n(begin(facts), size(facts), -1);
        std::fill_n(begin(inv_fact), size(inv_fact), -1);
        buildfact();
        for (int k = 0; k < N; k++) {
            buildInvFact(k);
        }
    }
    ll fact(ll x) {
        if (x <= 1) {
            return 1;
        }
        if (x >= N) {
            throw std::runtime_error("out of bound: " + to_string(x));
        }
        return facts[x];
    }

    ll comb(ll n, ll k) const {
        if (n >= N) {
            cout << "n: " << n << " out of bound";
            assert(false);
        }
        return mod_mul(facts[n], mod_mul(inv_fact[k], inv_fact[n-k]));
    }
private:
    vector<ll> facts;
    vector<ll> inv_fact;

    constexpr void buildfact() {
        for (int i = 0; i < N; i++) {
            facts[i] = i<=1 ? 1 : mod_mul(i, facts[i-1]);
        }
    }

    constexpr ll inv(ll i) {
        if (i > MOD) {
            i = i % MOD;
        }
        if (i <= 1) {
            return i;
        }
        return MOD - mod_mul((long long)(MOD / i), inv(MOD % i));
    }

    constexpr ll buildInvFact(ll k) {
        inv_fact[k] = inv(facts[k]);
        return inv_fact[k];
    }
};