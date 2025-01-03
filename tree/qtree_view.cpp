// qtree leaf view implementation
#include <array>
#include <ranges>
#include <stack>
#include <iostream>
using std::array, std::cout;

// quad tree
struct node {
	using value_type = int;

	array<node *, 4> children = {0};
	value_type data = -1;
	bool is_leaf() const {return children[0] == nullptr;}
};

// Leaf traversal view depth-first search (DFS) implementation.
template <typename Node>
struct leaf_view : std::ranges::view_base {
	explicit leaf_view(Node & root) : _root(&root) {}

	struct iterator {
		using value_type = Node::value_type;
		using reference = std::conditional_t<std::is_const_v<Node>, value_type const &, value_type &>;
		using pointer = std::conditional_t<std::is_const_v<Node>, value_type const *, value_type *>;
		using iterator_category = std::input_iterator_tag;

		std::stack<Node *> nodes;

		explicit iterator(Node * root) {
			if (root) {
				nodes.push(root);
				advance_to_leaf();
			}
		}

		reference operator*() const { 
			return nodes.top()->data;  // Dereference the leaf node's data
		}

		pointer operator->() const { 
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
				Node * current = nodes.top();
				nodes.pop();
				for (Node * child : current->children)
					nodes.push(child);
			}
		}
	};

	iterator begin() { return iterator(_root); }
	iterator end() { return iterator(nullptr); }

private:
	Node * _root;
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

	// view for const tree
	node const & croot = root;
	leaf_view cleaves(croot);
	for (int data : cleaves)  //= 4, 3, 2, 8, 7, 6, 5,
		std::cout << data << ", ";
	cout << '\n';


	return 0;
}