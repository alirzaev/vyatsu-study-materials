#include <bits\stdc++.h>

struct node
{
	char* str;
	size_t str_size;
	float* farr;
	size_t farr_size;

	node* left;
	node* right;
	node* parent;
};

struct tree {
	node root; // real root of tree is root.left
};

struct tree_iterator {
	node* cur;
	node* end;

	bool has_next() {
		return cur != end;
	}

	void next() {
		node* y = cur;
		if (y->right != nullptr) {
			y = y->right;
			while (y->left != nullptr) {
				y = y->left;
			}
			cur = y;
			return;
		}
		if (y->parent->left == y) {
			y = y->parent;
		}
		else {
			y = y->parent->parent;
		}
		cur = y;
	}
};

size_t leaks_counter = 0;

node* allocate_node(size_t str_size, size_t farr_size) {
	++leaks_counter;

	node* ret = (node*)realloc(nullptr, sizeof(node));
	ret->str = (char*)realloc(nullptr, sizeof(char) * str_size);
	ret->farr = (float*)realloc(nullptr, sizeof(float) * farr_size);

	ret->str_size = str_size;
	ret->farr_size = farr_size;

	ret->left = ret->right = ret->parent = nullptr;

	std::fill_n(ret->str, str_size, '\0');
	std::fill_n(ret->farr, farr_size, 0.0f);

	return ret;
}

node* allocate_node(node* src) {
	node* ret = allocate_node(src->str_size, src->farr_size);

	std::copy(src->str, src->str + src->str_size, ret->str);
	std::copy(src->farr, src->farr + src->farr_size, ret->farr);

	return ret;
}

void deallocate_node(node* n) {
	--leaks_counter;

	realloc(n->str, 0);
	realloc(n->farr, 0);
	realloc(n, 0);
}

bool less_node(node* first, node* second) {
	if (first->str_size == second->str_size &&
		std::equal(first->str, first->str + first->str_size, second->str)) { // if first->str == second->str
		return std::lexicographical_compare(
			first->farr, first->farr + first->farr_size,
			second->farr, second->farr + second->farr_size
			);
	}
	else {
		return std::lexicographical_compare(
			first->str, first->str + first->str_size,
			second->str, second->str + second->str_size
			);
	}
}

node* find_node(tree* tr, node* key) {
	node* root = tr->root.left;

	while (root != nullptr) {
		if (less_node(key, root)) { // key < root
			root = root->left;
		}
		else if (less_node(root, key)) { // key > root
			root = root->right;
		}
		else { // key == root, success!
			return root;
		}
	}

	return root; // key not found
}

void remove_node(tree* tr, node* key) {
	node* c = find_node(tr, key);
	if (c == nullptr) { // no such key in tree, nothing to do
		return;
	}

	node* l = c->left,
		*r = c->right,
		*p = c->parent;

	bool key_is_less = p->left == c;

	if (l == nullptr && r == nullptr) {
		if (key_is_less) { // left child
			p->left = nullptr;
		}
		else { //right child
			p->right = nullptr;
		}
	}
	else if (l == nullptr && r != nullptr) {
		if (key_is_less) {
			p->left = r;
		}
		else {
			p->right = r;
		}
	}
	else if (l != nullptr && r == nullptr) {
		if (key_is_less) {
			p->left = l;
		}
		else {
			p->right = l;
		}
	}
	else {
		node* new_p = r;
		while (r->left != nullptr) {
			new_p = r->left;
		}
		l->parent = new_p;
		new_p->left = l;
		if (key_is_less) {
			p->left = r;
		}
		else {
			p->right = r;
		}
	}
	deallocate_node(c);
}

void insert_node(tree* tr, node* key) {
	bool key_is_less = true;
	node* root = tr->root.left;
	node* parent = &(tr->root);
	
	while (root != nullptr) {
		if (less_node(key, root)) { // key < root
			key_is_less = true;
			parent = root;
			root = root->left;
		}
		else if (less_node(root, key)) { // key > root
			key_is_less = false;
			parent = root;
			root = root->right;
		}
		else { // key == root, node exists, nothing to do
			return;
		}
	}

	// insert new node
	root = allocate_node(key);
	root->parent = parent;
	if (key_is_less) {
		parent->left = root;
	}
	else {
		parent->right = root;
	}
}

void deallocate_tree(node* root) {
	if (root == nullptr) {
		return;
	}

	deallocate_tree(root->left);
	deallocate_tree(root->right);

	root->left = root->right = nullptr;

	deallocate_node(root);
}

void allocate_tree(tree* tr) {
	tr->root.left = tr->root.right = tr->root.parent = nullptr;
}

tree_iterator get_tree_iterator(tree* tr) {
	tree_iterator it;
	it.cur = &tr->root;
	it.end = &tr->root;

	while (it.cur->left != nullptr) {
		it.cur = it.cur->left;
	}

	return it;
}

void print_node(node* n) {
	for (size_t i = 0; i < n->str_size - 1; ++i) {
		std::cout << n->str[i];
	}
	std::cout << std::endl;
	for (size_t i = 0; i < n->farr_size; ++i) {
		std::cout << n->farr[i] << ' ';
	}
}

void solve(tree* tr) {
	char key_str[] = { 'k', 'b', 'z', 'u', 'l' };
	int n = 5;

	node* key = allocate_node(2, 2);
	
	// test insert
	for (int i = 0; i < n; ++i) {
		key->str[0] = key_str[i];
		insert_node(tr, key);
	}
	
	//test find
	for (int i = n - 1; i >= 0; --i) {
		key->str[0] = key_str[i];
		node* res = find_node(tr, key);
		if (res != nullptr) {
			std::cout << "success: " << key->str[0] << " " << key_str[i] << std::endl;
		}
		else {
			std::cout << "fail: " << key_str[i] << std::endl;
		}
	}

	for (tree_iterator it = get_tree_iterator(tr); it.has_next(); it.next()) {
		print_node(it.cur);
		std::cout << std::endl;
	}

	deallocate_node(key);
}

void loop(tree* tr) {
	while (true) {
		std::cout << "1. Insert\n2. Find\n3. Remove\n0. Exit\n";
		int code; std::cin >> code;
		if (code == 1) {
			std::string s; int n1, n2;
			std::cin >> n1 >> s >> n2;
			s.resize(n1);
			node* key = allocate_node(n1 + 1, n2);
			for (int i = 0; i < n1; ++i) {
				key->str[i] = s[i];
			}
			for (int i = 0; i < n2; ++i) {
				std::cin >> key->farr[i];
			}
			insert_node(tr, key);
			deallocate_node(key);
		}
		else if (code == 2) {
			std::string s; int n1, n2;
			std::cin >> n1 >> s >> n2;
			s.resize(n1);
			node* key = allocate_node(n1 + 1, n2);
			for (int i = 0; i < n1; ++i) {
				key->str[i] = s[i];
			}
			for (int i = 0; i < n2; ++i) {
				std::cin >> key->farr[i];
			}
			if (find_node(tr, key) != nullptr) {
				std::cout << "key exists\n";
			}
			else {
				std::cout << "key doesn't exist\n";
			}
			deallocate_node(key);
		}
		else if (code == 3) {
			std::string s; int n1, n2;
			std::cin >> n1 >> s >> n2;
			s.resize(n1);
			node* key = allocate_node(n1 + 1, n2);
			for (int i = 0; i < n1; ++i) {
				key->str[i] = s[i];
			}
			for (int i = 0; i < n2; ++i) {
				std::cin >> key->farr[i];
			}
			remove_node(tr, key);
			deallocate_node(key);
		}
		else if (code == 0) {
			break;
		}
	}
}

int main(void) {
	tree tr = { nullptr };
	//solve(&tr);
	loop(&tr);
	deallocate_tree(tr.root.left);
	std::cout << "leaks: " << leaks_counter << std::endl;
	return 0;
}