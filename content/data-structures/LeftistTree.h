/**
 * Author: Bao Tran
 * Date: 2026-11-12
 * License: CC0
 * Source: kactl
 * Description: Persistent Dynamic Leftist Tree for Min Heap managing and merging.
 * Addition and Deletion can be turned into merges.
 * Time: $O(\log N)$ for merges.
 * Status: Tested on QOJ.
 */
#pragma once

struct Node {
	Node *l = 0, *r = 0;
	int val, c = 1;
	Node(int val) : val(val) {}
	void recalc();
};
int cnt(Node* n) { return n ? n->c : 0; }
void Node::recalc() { c = cnt(l) + cnt(r) + 1; }
Node* merge(Node *x, Node *y) {
	if (!x) return y;
	if (!y) return x;
	if (x->val > y->val) swap(x, y);
	x = new Node(*x);
	x->r = merge(x->r, y);
	if (cnt(x->l) < cnt(x->r)) swap(x->l, x->r);
	x->recalc();
	return x;
}
