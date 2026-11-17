/**
 * Author: Bao Tran
 * Date: 2026-11-17
 * Source: OI wiki
 * Description: Min\_25 sieve computes prefix sums of multiplicative
 * functions whose values on primes are low-degree polynomials.
 * Set $f(p^c)$ in f() and coefficients $a,b$ for $f(p)=ap+b$.
 * Uses quotient grouping and prime sieving up to $\sqrt n$.
 * All results are computed modulo mod.
 * Time: $O(n^{3/4}/\log n)$.
 * Memory: $O(\sqrt n)$.
 */

#pragma once

typedef vector<ll> vl;
const ll mod = 469762049;
struct Min25 {
  ll n, a, b;
  int lim, cnt = 0, pcnt = 0;
  vi p = {1}, le, ge;
  vl spri, Fprime, G[2], lis = {0};
  // f(p^c), with f(p) = a*p + b.
  ll f(ll prime, int c) const { return (prime + c) % mod; }
  Min25(ll n, ll a, ll b) : n(n), a(a), b(b) {
    lim = 1;
    while (1LL * (lim + 1) * (lim + 1) <= n) ++lim;
    sieve(); init(); calcFprime();
  }
  void sieve() {
    vi lp(lim + 1);
    for (int i = 2; i <= lim; ++i) {
      if (!lp[i]) lp[i] = i, p.push_back(i);
      for (int j = 1; j < sz(p) &&
					p[j] <= lp[i] && 1LL * i * p[j] <= lim; ++j)
          	lp[i * p[j]] = p[j];
    }
    pcnt = sz(p) - 1; spri.resize(pcnt + 1);
    for (int i = 1; i <= pcnt; ++i)
    	spri[i] = (spri[i - 1] + p[i]) % mod;
  }
  #define idx(x) ((x) <= lim ? le[(x)] : ge[n / (x)])
  void init() {
    le.resize(lim + 1); ge.resize(lim + 1);
    G[0] = {0}; G[1] = {0};
    for (ll l = 1, r; l <= n; l = r + 1) {
      ll x = n / l;
      r = n / x;
      lis.push_back(x);
      ++cnt;
      idx(x) = cnt;
      G[0].push_back((x - 1) % mod);
      ll u = x, v = x + 1;
      (u % 2 == 0 ? u : v) /= 2;
      G[1].push_back(((u % mod) * (v % mod) - 1 + mod) % mod);
    }
  }
  void calcFprime() {
    for (int i = 1; i <= pcnt; ++i) {
      ll prime = p[i], sqrp = prime * prime;
      for (int j = 1; j <= cnt && lis[j] >= sqrp; ++j) {
        int k = idx(lis[j] / prime);
        G[0][j] = (G[0][j] - G[0][k] + i - 1 + mod) % mod;
        G[1][j] = (G[1][j] - prime * ((G[1][k] - spri[i - 1] + mod) % mod) 
        					% mod + mod) % mod;
      }
    }
    Fprime.resize(cnt + 1);
    for (int i = 1; i <= cnt; ++i)
      Fprime[i] = (a * G[1][i] % mod
                  + b * G[0][i] % mod) % mod;
  }
  ll F(int k, ll x) {
      if (x <= 1 || (k <= pcnt && x < p[k])) return 0;
      ll pref = (a * spri[k - 1] % mod
                + b * (k - 1) % mod) % mod;
      ll ans = (Fprime[idx(x)] - pref + mod) % mod;
      for (int i = k; i <= pcnt && 1LL * p[i] * p[i] <= x; ++i) {
        ll pw = p[i];
        for (int c = 1; pw <= x / p[i]; pw *= p[i], ++c) {
          ll v = (f(p[i], c) * F(i + 1, x / pw) % mod
                 + f(p[i], c + 1)) % mod;
          ans = (ans + v) % mod;
        }
      }
      return ans;
  }
  ll solve() { return n == 0 ? 0 : (F(1, n) + 1) % mod; }
  #undef idx
};