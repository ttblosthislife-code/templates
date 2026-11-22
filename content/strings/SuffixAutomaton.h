/**
 * Author: Bao Tran
 * Date: 2026-11-20
 * License: CC0
 * Source: cp-algo, std-abs codebook
 * Description: Suffix automaton for lowercase strings.
 * Each state represents substrings with the same endpos set.
 * ch stores transitions, link stores suffix links.
 * len stores maximum length of represented substrings.
 * pos stores first ending position (0-indexed).
 * cnt stores occurrence count after buildCnt().
 * Supports substring matching and occurrence queries.
 * Time: O(n) build, O(n log n) buildCnt().
 * Memory: O(nC)
 * Status: Tested on library checker
 */
#pragma once

struct SAM {
	enum { N = (int)5e5 + 5, C = 26 };
	int ch[2 * N][C], len[2 * N], link[2 * N], pos[2 * N], cnt[2 * N];
	bool is_clone[2 * N], is_terminal[2 * N];
	int size, last;
	SAM() { reset(); }
	int newNode() {
		fill_n(ch[size], C, 0);
		len[size] = link[size] = pos[size] = cnt[size] = 0;
		is_clone[size] = is_terminal[size] = 0;
		return size++;
	}
	void extend(char x) {
		int c = x - 'a';
		int cur = newNode();
		len[cur] = len[last] + 1;
		pos[cur] = pos[last] + 1;
		cnt[cur] = 1;
		int p = last;
		while (p != -1 && !ch[p][c]) {
			ch[p][c] = cur;
			p = link[p];
		}
		if (p != -1) {
			int q = ch[p][c];
			if (len[p] + 1 == len[q]) link[cur] = q;
			else {
				int clone = newNode();
				rep(j, 0, C) ch[clone][j] = ch[q][j];
				len[clone] = len[p] + 1;
				link[clone] = link[q];
				pos[clone] = pos[q];
				is_clone[clone] = 1;
				while (p != -1 && ch[p][c] == q) {
					ch[p][c] = clone;
					p = link[p];
				}
				link[q] = link[cur] = clone;
			}
		}
		last = cur;
	}
	void build(const string &s) {
		for (auto &c : s) extend(c);
		int p = last;
		while (p != -1) {
			is_terminal[p] = 1;
			p = link[p];
		}
		buildCnt();
	}
	void buildCnt() {
		vi p(size);
		iota(all(p), 0);
		sort(all(p), [&](int i, int j) {
			return len[i] > len[j]; });
		rep(i, 0, size - 1) cnt[link[p[i]]] += cnt[p[i]];
	}
	void reset() {
		size = last = 0;
		newNode();
		link[0] = pos[0] = -1;
		cnt[0] = 1;
	}
} sam;