/**
 * Author: Bao Tran
 * Date: 2026-11-22
 * Source: Self implementation from Tarjan's slides.
 * Description: Computes the dominator tree of a directed graph rooted at s.
 * A vertex u dominates v if every path from s to v passes through u.
 * Returns the immediate dominator of each vertex (root points to itself,
 * unreachable vertices have parent -1).
 * Time: O((N + M) \log N).
 * Status: Tested on Library checker.
 */
#pragma once

struct DominatorTree {
	int n, timer = 0;
	vector<vi> g, rg, bucket;
	vi sdom, rdom, idom, tin, rtin, pa, rt, mn, no;
	DominatorTree(int n) : n(n), g(n), rg(n), bucket(n), 
	sdom(n), rdom(n), idom(n), tin(n, -1), rtin(n), 
	pa(n), rt(n), mn(n), no(n) {}
	void addEdge(int u, int v) { g[u].push_back(v); }
	void dfs(int u) {
		rt[timer] = mn[timer] = no[timer] = timer;
		rtin[timer] = u; tin[u] = timer++;
		for (auto &v : g[u]) {
			if (tin[v] == -1) dfs(v), pa[tin[v]] = tin[u];
			rg[tin[v]].push_back(tin[u]);
		}
	}
	int getRt(int u) {
		if (rt[u] == u) return u;
		int tmp = getRt(rt[u]);
		if (mn[u] > mn[rt[u]]) {
			mn[u] = mn[rt[u]];
			no[u] = no[rt[u]];
		}
		return rt[u] = tmp;
	}
	vi build(int s) {
		dfs(s);
		for (int u = timer - 1; u > 0; --u) {
			sdom[u] = rdom[u] = u;
			for (auto &v : rg[u]) {
				getRt(v);
				sdom[u] = min(sdom[u], mn[v]);
			}
			for (auto &v : bucket[u]) {
				getRt(v);
				rdom[v] = no[v];
			}
			mn[u] = sdom[u], rt[u] = pa[u];
			if (sdom[u]) bucket[sdom[u]].push_back(u);
		}
		vi p(n, -1); p[s] = s;
		rep(u, 1, timer) {
			idom[u] = (rdom[u] == u? sdom[u] : idom[rdom[u]]);
			p[rtin[u]] = rtin[idom[u]];
		}
		return p;
	}
};