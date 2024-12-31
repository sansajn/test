// iterating quad tree sample
#include <array>
#include <iostream>
#include <cstddef>
using std::array;
using std::cout;

// quad tree
struct node {
	array<node *, 4> children = {0};
	int data = -1;
	bool is_leaf() const {return children[0] == nullptr;}
};

// iterate over tree leafs
void iterate(node const & root) {
	if (root.is_leaf())
		cout << root.data << ", ";
	else {
		iterate(*root.children[0]);
		iterate(*root.children[1]);
		iterate(*root.children[2]);
		iterate(*root.children[3]);
	}
}


void iterate_depth(node const & root, int depth, int max_depth) {
	if (depth == max_depth && root.is_leaf())
		cout << root.data << ", ";
	else {
		if (!root.is_leaf() && depth < max_depth) {
			iterate_depth(*root.children[0], depth+1, max_depth);
			iterate_depth(*root.children[1], depth+1, max_depth);
			iterate_depth(*root.children[2], depth+1, max_depth);
			iterate_depth(*root.children[3], depth+1, max_depth);
		}
	}
}

// iterate over tree leafs in specified depth
void iterate(node const & root, int depth) {
	iterate_depth(root, 0, depth);
}

// TODO: How to implement a range for iterating leafs?


int main(int argc, char * argv[]) {
	// build a simple tree
	array<node, 8> pool = {0};
	for (size_t i = 0; i < size(pool); ++i) // TODO: is there any better way to initialize data?
		pool[i].data = i+1;

	node root;
	root.children[0] = &pool[0];
	root.children[1] = &pool[1];  // leaf
	root.children[2] = &pool[2];  // leaf
	root.children[3] = &pool[3];  // leaf

	node & r00 = *root.children[0];
	r00.children[0] = &pool[4];  // leaf
	r00.children[1] = &pool[5];  // leaf
	r00.children[2] = &pool[6];  // leaf
	r00.children[3] = &pool[7];  // leaf

	// iterare tree
	iterate(root);  //= 5, 6, 7, 8, 2, 3, 4
	cout << '\n';

	iterate(root, 1);  //= 2, 3, 4
	cout << '\n';

	iterate(root, 2);  //= 5, 6, 7, 8
	cout << '\n';

	return 0;
}
