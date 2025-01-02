// Quad tree Depth-First Search (DFS) and Breadth-First Search (BFS) traverse implementation.
#include <array>
#include <stack>
#include <queue>
#include <iostream>
#include <cstddef>
using std::array, std::stack, std::queue, std::cout;

struct node {
	array<node *, 4> children = {0};
	int data = -1;
	bool is_leaf() const {return children[0] == nullptr;}
};

void dfs_leaf_traverse(node const * root) {
	if (!root) return;

	stack<node const *> stack;
	stack.push(root);

	while (!stack.empty()) {
		const node *current = stack.top();
		stack.pop();

		if (current->is_leaf())
			cout << current->data << ", ";
		else {
			for (node const * child : current->children) {
				if (child)
					stack.push(child);
			}
		}
	}
}

void bfs_leaf_traverse(const node *root) {
	if (!root) return;

	queue<const node *> q;
	q.push(root);

	while (!q.empty()) {
		const node *current = q.front();
		q.pop();

		if (current->is_leaf())
			cout << current->data << ", ";
		else {
			for (node const * child : current->children) {
				if (child) {
					q.push(child);
				}
			}
		}
	}
}

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

	cout << "DFS traverse order: ";
	dfs_leaf_traverse(&root);  //= 4, 3, 2, 8, 7, 6, 5,
	cout << '\n';

	cout << "BFS traverse order: ";
	bfs_leaf_traverse(&root);  //= 2, 3, 4, 5, 6, 7, 8,
	cout << '\n';

	return 0;
}
