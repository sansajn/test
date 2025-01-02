// qtree leaf view implementation
#include <array>
#include <ranges>
#include <stack>
#include <iostream>
using std::array, std::cout;

// quad tree
struct node {
	array<node *, 4> children = {0};
	int data = -1;
	bool is_leaf() const {return children[0] == nullptr;}
};

// Leaf traversal view depth-first search (DFS) implementation.
struct leaf_view : std::ranges::view_base {
	explicit leaf_view(node & root) : _root(&root) {}

	struct iterator {
		std::stack<node *> nodes;

		explicit iterator(node * root) {
			if (root) nodes.push(root);
			advance_to_leaf();
		}

		int & operator*() const { 
			return nodes.top()->data; // Dereference the leaf node's data
		}

		int * operator->() const { 
			return &nodes.top()->data; // Access the data of the leaf node
		}

		iterator & operator++() {
			nodes.pop();
			advance_to_leaf();
			return * this;
		}

		bool operator!=([[maybe_unused]] iterator const & other) const {
			return !nodes.empty();
		}

	private:
		void advance_to_leaf() {
			while (!nodes.empty() && !nodes.top()->is_leaf()) {
				node * current = nodes.top();
				nodes.pop();
				for (node * child : current->children)
					nodes.push(child);
			}
		}
	};

	iterator begin() { return iterator(_root); }
	iterator end() { return iterator(nullptr); }

private:
	node * _root;
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
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
	
	leaf_view leaves(root);
	for (int data : leaves)  //= 4, 3, 2, 8, 7, 6, 5,
		std::cout << data << ", ";
	cout << '\n';

	return 0;
}