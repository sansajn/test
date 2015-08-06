// binary search tree implementation (recursive) from 'c++ data structures'
#include <iostream>

struct TreeNode;

typedef char ItemType;

enum OrderType {PRE_ORDER, IN_ORDER, POST_ORDER};

class TreeType
{
public:
	TreeType();
	TreeType(TreeType const & t);
	~TreeType();
	void operator=(TreeType const & t);
	void MakeEmpty();
	bool IsEmpty() const;
	bool IsFull() const;
	int LengthIs() const;
	void RetrieveItem(ItemType & item, bool & found) const;
	void InsertItem(ItemType item);
	void DeleteItem(ItemType item);
	void ResetTree(OrderType order);
	void GetNextItem(ItemType & item, OrderType order, bool & finished);
	void Print(ofstrem & outFile) const;

private:
	TreeNode * root;
};  // TreeType

struct TreeNode
{
	ItemType info;
	TreeNode * left;
	TreeNode * right;
};  // TreeNode

bool TreeType::IsFull() const
{
	TreeNode * location;
	try 
	{
		location = new TreeNode;
		delete location;
		return false;
	} 
	catch (std::bad_alloc e) {return true;}
}

bool TreeType::IsEmpty() const
{
	return root == nullptr;
}

int CountNodes(TreeNode * tree);

if TreeType::LengthIs() const
{
	return CountNodes(root);
}

int CountNodes(TreeNode * t)
{
	if (!t)
		return 0;
	else
		return CountNodes(tree->left) + CountNodes(tree->right) + 1;
}

void Retrieve(TreeNode * t, Item & item, bool & found);

void TreeType::RetrieveItem(ItemType & item, bool & found) const
{
	Retrieve(root, item, found);
}

void Retrieve(TreeNode * t, Item & item, bool & found)
{
	if (!t)
		found = false;
	else if (item < tree->info)
		Retrieve(tree->left, item, found);
	else if (item > tree->info)
		Retrieve(tree->right, item, found);
	else
	{
		item = tree->info;
		found = true;
	}
}

void Insert(TreeNode *& t, ItemType item);

void TreeType::InsertItem(ItemType item)
{
	Insert(root, item);
}

void Insert(TreeNode *& t, ItemType item)
{
	if (!t)
	{  // insertion place found
		t = new TreeNode;
		t->right = nullptr;
		t->left = nullptr;
		t->info = item;
	}
	else if (item < t->info)
		Insert(t->left, item);
	else
		Insert(t->right, item);
}

void DeleteNode(TreeNode *& t);

void Delete(TreeNode *& t, ItemType item);

void TreeType::DeleteItem(ItemType item)
{
	Delete(root, item);
}

void Delete(TreeNode *& t, ItemType item)
{
	if (item < t->info)
		Delete(t->left, item);
	else if (item > t->info)
		Delete(t->right, item);
	else
		Delete(t);
}

void GetPredecessor(TreeNode * t, ItemType & data);

void DeleteNode(TreeNode *& t)
{
	ItemType data;
	TreeNode * tempPtr = t;
	if (!t->left)
	{
		t = t->right;
		delete tempPtr;
	}
	else if (!t->right)
	{
		t = t->left;
		delete tempPtr;
	}
	else
	{
		GetPredecessor(t->left, data);  // logical predecessor
		t->info = data;
		Delete(t->left, data);
	}
}

void GetPredecessor(TreeNode * t, ItemType & data)
{
	while (t->right)
		t = t->right;
	data = t->info;
}

void PrintTree(TreeNode * t, std::ofstream & out)  // inorder traversal
{
	if (t)
	{
		PrintTree(t->left, out);
		out << t->info;
		PrintTree(t->right, out);
	}
}

void TreeType::Print(std::ofstream & out) const
{
	PrintTree(root, out);
}

TreeType::TreeType()	: root(nullptr)
{}

void Destroy(TreeNode *& t);

TreeType::~TreeType()
{
	Destroy(root);
}

void Destroy(TreeNode *& t)
{
	if (t)
	{
		Destroy(t->left);
		Destroy(t->right);
		delete t;
	}
}

void CopyTree(TreeNode *& cpy, TreeNode const * t);

TreeType::TreeType(TreeType const & t)
{
	CopyTree(root, t.root);
}

void TreeType::operator=(TreeType const & t)
{
	if (&t == this)
		return;
	Destroy(root);
	CopyTree(root, t.root);
}

void CopyTree(TreeNode *& cpy, TreeNode const * t)
{
	if (!t)
		cpy = nullptr;
	else
	{
		cpy = new TreeNode;
		cpy->info = t->info;
		CopyTree(cpy->left, t->left);
		CopyTree(cpy->right, t->right);
	}
}

