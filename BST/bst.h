#pragma once
#include <iostream>
#include <cassert>
#include <functional>
#include <stack>

using namespace std;

//struct BinaryNode;

template<class T>
struct BinaryNode
{
	BinaryNode* left, * right;
	T data;

	BinaryNode() : data(T()), left(nullptr), right(nullptr) {}
	BinaryNode(const T& val) : data(val), left(nullptr), right(nullptr) {}
};

template<class T>
class BST
{
	 typedef BinaryNode<T> Node;

public:
    class Iterator : public std::iterator<std::input_iterator_tag, const T>
    {
//        typename BST<T>::Iterator Iter;
        stack<Node*> stack_;
        Node* current;

        public:
        Iterator() = default;
        Iterator(Node* node);
        Iterator(const Iterator& source);
        bool operator==(const Iterator &it) const { return (current == it.current); }
        bool operator!=(const Iterator &it) const { return !((current == it.current) && (stack_ == it.stack_)); }
        Iterator& operator++();
        Iterator operator++(int); // post
        const T& operator*() const { if(current != nullptr) return current->data; else throw std::invalid_argument("nullptr"); }
        const T* operator->() const { if( current != nullptr) return &current->data; else return nullptr;}
        void next();
        bool isEmpty() { return stack_.empty(); }
        friend class BST<T>;
    };


	T* insert(const T& x);
	bool remove(const T& x);
	void removeMin();
    void clearTree() { clearTree(root); cur_size = 0; }

	const T& findMin() const;
	const T& findMax() const;
    Node* findMax(Node* ptr) const;
	T* find(const T& x);
	const T* find(const T& indata) const ;

	Node* getRoot() const { return root; }
	bool isEmpty() const { return cur_size == 0; }
	int size() const { return cur_size; }
	void inorder(std::function<void(const T&)> f) const;

	// Wypisywanie
	void preOrder(ostream& os) const;
	void inOrder(ostream& os) const;
	void postOrder(ostream& os) const;
	void inOrder() const;

	// Konstruktory i destruktor
	BST() : root(nullptr), cur_size(0) {}
	BST(std::initializer_list<T> list);
	BST(const BST& tree);
	BST(BST&& tree);
	virtual ~BST() { clearTree(root); }

	// Operatory
	BST& operator=(const BST&);
	BST& operator=(BST&&) ;

    // Do iteratora
    Iterator begin() const;
    Iterator end() const;

protected:

	Node* root;
	int cur_size;

	T* insert(const T& x, Node*& ptr);
	bool remove(const T& x, Node*& ptr);
	void removeMin(Node*& ptr);
	Node* findMin(Node* ptr) const;
	Node* find(const T& x, Node* ptr) const;
	const Node* find(Node* ptr, const T& x ) const;
	Node* clone(Node* node) const;
	void clearTree(Node*& ptr);

	void preOrder(ostream& os, Node* ptr) const;
	void inOrder(ostream& os, Node* ptr) const;
	void postOrder(ostream& os, Node* ptr) const;
	void inOrder(Node* node) const;
	void inorder(Node* node, std::function<void(const T&)> f) const;

};


//KONSTRUKTORY
template <class T>
BST<T>::BST(std::initializer_list<T> list) :BST() {
	for(const T& e : list) {
		insert(e);
	}
}

template<class T>
BST<T>::BST(const BST& tree) : BST()
{
	root = clone(tree.root);
	cur_size = tree.cur_size;
}

template<class T>
BST<T>::BST(BST &&tree) : BST()
{
	root = tree.root;
	tree.root = nullptr;
	cur_size = tree.cur_size;
	tree.cur_size = 0;
}

//OPERATORY
template<class T>
BST<T> &BST<T>::operator=(const BST & other)
{
	if(this != &other)
	{
		clearTree();
		root = clone(other.root);
		cur_size = other.cur_size;
	}
	return *this;
}

template <typename T>
BST<T>& BST<T>::operator=(BST&& bst) {
	if (this != &bst) {
		root = bst.root;
		cur_size = bst.cur_size;
		bst.root = nullptr;
		bst.cur_size = 0;
	}
    return *this;
}


//INSERT
template<class T>
T* BST<T>::insert(const T& x)
{
	return insert(x, root);
}

template<class T>
T* BST<T>::insert(const T& x, Node*& ptr)
{
    if (ptr == nullptr) {
        ptr = new Node(x);
        cur_size++;
        return &ptr->data;
    }
    else {
        if (x < ptr->data)
            return insert(x, ptr->left);
        else
            return insert(x, ptr->right);
    }
}

//REMOVE
template<class T>
bool BST<T>::remove(const T& x)
{
	return remove(x, root);
}

template<class T>
bool BST<T>::remove(const T& x, Node*& ptr)
{
    if (ptr) {
        if (x < ptr->data)
            remove(x, ptr->left);
        else if (x > ptr->data)
            remove(x, ptr->right);
        else {
            //Dwoje dzieci
            if (ptr->left && ptr->right) {
                Node* min = findMin(ptr->right);
                ptr->data = min->data;
                removeMin(ptr->right);
            }
            else { /*Zero lub jedno dziecko*/
                Node* temp = ptr;
                ptr = ptr->left ? ptr->left : ptr->right;
                delete temp;
                cur_size--;
            }
            return true;
        }
    }
    return false;
}

template<class T>
void BST<T>::removeMin()
{
	removeMin(root);
}

template<class T>
void BST<T>::removeMin(Node*& ptr)
{
    if (ptr ) {
        if (ptr->left)
            removeMin(ptr->left);
        else {
            Node* min = ptr;
            ptr = ptr->right;
            delete min;
            cur_size--;
        }
    }
}


//FIND
template<class T>
const T& BST<T>::findMin() const
{
	Node* ptr = findMin(root);
	assert(ptr);
	return ptr->data;
}

template<class T>
BinaryNode<T> * BST<T>::findMin(Node* ptr) const
{
    if (ptr){
        while (ptr->left)
            ptr = ptr->left;
    }
    return ptr;
}

template<class T>
const T& BST<T>::findMax() const
{
	Node* ptr = findMax(root);
	assert(ptr);
    return ptr->data;
}

template<class T>
BinaryNode<T> * BST<T>::findMax(Node* ptr) const
{
    if (ptr){
        while (ptr->right)
            ptr = ptr->right;
    }
    return ptr;
}

template<class T>
T* BST<T>::find(const T& x)
{
	Node* ptr = find(x, root);
	if (ptr)
		return &ptr->data;
	else
		return (T*)nullptr;
}

template<class T>
BinaryNode<T> * BST<T>::find(const T& x, Node* ptr) const
{
    while (ptr) {
        if (x < ptr->data)
            ptr = ptr->left;
        else if (ptr->data < x)
            ptr = ptr->right;
        else
            break;
    }
    return ptr;
}

template<class T>
const T *BST<T>::find(const T &indata) const
{
    const BinaryNode<T> * ptr = find(root, indata);

    if (ptr)
        return &ptr->data;
    else
        return (T*)nullptr;
}

template<class T>
const BinaryNode<T> *BST<T>::find(BinaryNode<T> *ptr, const T &x) const
{
    while (ptr){
        if (x < ptr->data)
            ptr = ptr->left;
        else if (ptr->data < x)
            ptr = ptr->right;
        else
            break;
    }
    return ptr;
}

//CLONE
template<typename T>
BinaryNode<T>* BST<T>::clone(BST::Node* node) const {

    if(node) {
        Node* newNode = new BinaryNode<T>(node->data);
        newNode->left = clone(node->left);
        newNode->right = clone(node->right);
        return newNode;
    }
    return nullptr;
}

//CLEAR
template<class T>
void BST<T>::clearTree(Node*& ptr)
{
    if (ptr){
        clearTree(ptr->left);
        clearTree(ptr->right);
        delete ptr;
        ptr = nullptr;
    }
}

//PRZECHODZENIE - FUNKCJA
template<class T>
void BST<T>::inorder(std::function<void (const T &x)> f) const
{
	inorder(root, f);
}

template<class T>
void BST<T>::inorder(BST::Node *node, std::function<void (const T &)> f) const
{
    if(node == nullptr) return;

    inorder(node->left, f);
    f(node->data);
    inorder(node->right, f);
}


//PRZECHODZENIE
template<class T>
void BST<T>::preOrder(ostream& os) const
{
    preOrder(os, root);
}

template<class T>
void BST<T>::preOrder(ostream& os, Node* ptr) const
{
    if (ptr) {
        os << ptr->data << ' ';
        preOrder(os, ptr->left);
        preOrder(os, ptr->right);
    }
}

template<class T>
void BST<T>::inOrder(ostream& os) const
{
    inOrder(os, root);
}

template<class T>
void BST<T>::inOrder(ostream& os, Node* ptr) const
{
    if (ptr) {
        inOrder(os, ptr->left);
        os << ptr->data << ' ';
        inOrder(os, ptr->right);
    }
}

template<class T>
void BST<T>::postOrder(ostream& os) const
{
    postOrder(os, root);
}

template<class T>
void BST<T>::postOrder(ostream& os, Node* ptr) const
{
    if (ptr) {
        postOrder(os, ptr->left);
        postOrder(os, ptr->right);
        os << ptr->data << ' ';
    }
}

template<class T>
void BST<T>::inOrder() const
{
    if(root == nullptr) return;

    BST<T>::inOrder(root->left);
    std::cout << root->data << std::endl;
    BST<T>::inOrder(root->right);
}

template<class T>
void BST<T>::inOrder(BST<T>::Node* node) const
{
    if(node == nullptr) return;

    BST<T>::inOrder(node->left);
    std::cout << node->data << std::endl;
    BST<T>::inOrder(node->right);
}

//DLA ITERATORA
template<class T>
typename BST<T>::Iterator BST<T>::begin() const
{
    BST<T>::Iterator it(root);
    return it;
}

template<class T>
typename BST<T>::Iterator BST<T>::end() const
{
    BST<T>::Iterator it(findMax(root));
    ++it;
    return it;
}


//================= KLASA ITERATOR ================
template<class T>
BST<T>::Iterator::Iterator(BST<T>::Node *node) : current(nullptr)
{
    Node* tmp = node;
    while ( tmp != nullptr) {
        stack_.push(tmp);
        tmp = tmp->left;
    }
    current = stack_.top();
    ++(*this);
}

template<class T>
BST<T>::Iterator::Iterator(const BST<T>::Iterator &source) : current(source.current), stack_(source.stack_)
{}

template<class T>
typename BST<T>::Iterator &BST<T>::Iterator::operator++()
{
    next();
    return (*this);
}

template<class T>
typename BST<T>::Iterator BST<T>::Iterator::operator++(int)
{
    BST<T>::Iterator it(*this);
    next();
    return it;
}

template<class T>
void BST<T>::Iterator::next()
{
    if( stack_.empty() ) {
        current = nullptr;
        return;
    }
    this->current = stack_.top();
    Node* tmp = current;
    tmp = tmp->right;
    stack_.pop();

    while ( tmp != nullptr) {
        stack_.push(tmp);
        tmp = tmp->left;
    }
}
