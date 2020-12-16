#pragma once
#include <functional>
#include <iostream>
#include <cassert>
#include <stack>


using namespace std;

template<typename T>
struct BSTNode{
    T key;
    BSTNode *left, *right;

    BSTNode() : key(T()), left(nullptr), right(nullptr) {}
    explicit BSTNode(const T& val) : key(val), left(nullptr), right(nullptr) {}
};

template<typename T>
class BST{
    typedef BSTNode<T> Node;

public:

    class Iterator : public std::iterator<std::input_iterator_tag, const T>{
        stack<Node*> stack_;
        Node* current;

    public:
        Iterator() : current(nullptr) {}
        Iterator(Node* ptr);
        Iterator(const Iterator& source);
        Iterator(Node *node, Node * root);

        bool operator==(const Iterator &it) const { return (current == it.current); }
        bool operator!=(const Iterator &it) const { return !((current == it.current) && (stack_ == it.stack_)); }

        Iterator& operator++();
        Iterator operator++(int);

        const T& operator*() const { if(current != nullptr) return current->key; else throw invalid_argument("nullptr"); }
        const T* operator->() const { if( current != nullptr) return &current->key; else return nullptr;}
        explicit operator bool() const { return current != nullptr; }

        void next();
        bool isEmpty() { return stack_.empty(); }
        friend class BST<T>;
    };

    //konstruktory
    BST() : root_(nullptr), size_(0) {};
    virtual ~BST() { clearTree(root_);}
    BST(const BST<T>& bst);
    BST(BST<T>&& bst);
    BST(std::initializer_list<T> list);

    BST<T>& operator=(const BST<T>&);
    BST<T>& operator=(BST<T>&&);

    const T& findMin() const;
    Node* minimum(Node* ptr) const;
    Node* maximum(Node* ptr) const;
    T* insert(const T& x, Node*& ptr);
    void preOrder(Node* ptr) const;
    void postOrder(Node* ptr) const;
    Node* getRoot() const { return root_; }
    bool isEmpty() const;
    size_t size() const;
    Node* clone(Node* ptr) const;
    void inOrder(Node* ptr, std::function<void(const T&)> f) const;
    void inOrder(std::function<void (const T &val)> f) const;
    const Node* find(Node* ptr, const T& val ) const;
    Node* find(const T& val, Node* ptr) const;
    bool remove(const T& val, Node*& ptr);
    void removeMin(Node*& ptr);
    Node* findMin(Node* ptr) const;
    Node* find_It_Helper(const T& x ) const;
    void clearTree(Node*& ptr);

    Node* minimum() const;
    Node* maximum() const;
    bool remove(const T& x);
    void clearTree();
    void preOrder() const;
    T* insert(const T& ptr);
    void postOrder() const;
    const T* find(const T& val) const ;
    Iterator begin(Node * ptr) const;
    Iterator end() const;
    T* find(const T& val);

private:
    Node* root_;
    int size_;
};


template<typename T>
BST<T>& BST<T>::operator=(const BST<T>& bst){
    if (this != &bst){
        clearTree(root_);
        root_ = clone(bst.root_);
        size_ = bst.size_;
    }
    return *this;
}

template<typename T>
BST<T>& BST<T>::operator=(BST<T>&& bst){
    if (this != &bst) {
        root_ = bst.root_;
        size_ = bst.size_;
        bst.size_ = 0;
        bst.root_ = nullptr;
    }
    return *this;
}

template<typename T>
BST<T>::BST(const BST<T>& bst) : BST(){
    root_ = clone(bst.root_);
    size_ = bst.size_;
}

template<typename T>
BST<T>::BST(BST<T>&& bst) : BST(){
    root_ = bst.root_;
    size_ = bst.size_;
    bst.root_ = nullptr;
    bst.size_ = 0;
}

template<typename T>
BST<T>::BST(initializer_list<T> list) {
    for(const T& e : list)
        insert(e);
}

template<typename T>
BSTNode<T>* BST<T>::minimum(Node* ptr) const{
    while (root_->left != nullptr) { ptr = ptr->left;}
    return root_;
}

template<typename T>
BSTNode<T>* BST<T>::minimum() const{
    return minimum(root_);
}

template<typename T>
BSTNode<T>* BST<T>::maximum(Node* ptr) const{
    while (ptr->right != nullptr) {ptr = ptr->right;}
    return ptr;
}

template<typename T>
BSTNode<T>* BST<T>::maximum() const{
    return maximum(root_);
}



template<typename T>
T* BST<T>::insert(const T& x, BSTNode<T>*& ptr){
    if (ptr == nullptr){
        ptr = new Node(x);
        size_++;
        return &ptr->key;
    }else{
        if (x < ptr->key)
            return insert(x, ptr->left);
        else
            return insert(x, ptr->right);
    }
}

template<typename T>
T* BST<T>::insert(const T& ptr){
    return insert(ptr, root_);
}

template<typename T>
void BST<T>::inOrder(Node* ptr, std::function<void (const T &)> f) const{
    if(ptr == nullptr) return;
    inOrder(ptr->left, f);
    f(ptr->key);
    inOrder(ptr->right, f);
}

template<typename T>
void BST<T>::inOrder(std::function<void (const T &x)> f) const{
    inOrder(root_, f);
}

template<typename T>
void BST<T>::preOrder(Node* ptr) const{
    if (ptr != nullptr) {
        std::cout << ptr->key << " ";
        preOrder(ptr->left);
        preOrder(ptr->right);
    }
}

template<typename T>
void BST<T>::preOrder() const{
    preOrder(root_);
}

template<typename T>
void BST<T>::postOrder(Node* ptr) const{
    if (ptr != nullptr) {
        postOrder(ptr->left);
        postOrder(ptr->right);
        std::cout << ptr->key << " ";
    }
}

template<typename T>
void BST<T>::postOrder() const{
    postOrder(root_);
}

template<typename T>
bool BST<T>::isEmpty() const{
    return root_ == nullptr || false;
}

template<typename T>
void BST<T>::clearTree(Node*& ptr){
    if(ptr){
        clearTree(ptr->left);
        clearTree(ptr->right);
        delete ptr;
        ptr = nullptr;
    }
}

template<typename T>
void BST<T>::clearTree(){
    clearTree(root_);
}

template<typename T>
size_t BST<T>::size() const{
    return size_;
}

template<typename T>
BSTNode<T>* BST<T>::clone(Node* ptr) const{
    if (ptr) {
        Node* newNode = new Node(ptr->key);
        newNode->left = clone(ptr->left);
        newNode->right = clone(ptr->right);
        return newNode;
    }
    return nullptr;
}



template<typename T>
bool BST<T>::remove(const T& val, Node*& ptr){
    if (ptr){
        if (val < ptr->key)
            return remove(val, ptr->left);
        else if (val > ptr->key)
            return remove(val, ptr->right);
        else {
            if (ptr->left && ptr->right) {
                Node* min = findMin(ptr->right);
                ptr->key = min->key;
                removeMin(ptr->right);
            }
            else {
                Node* temp = ptr;
                ptr = ptr->left ? ptr->left : ptr->right;
                delete temp;
                size_--;
            }
            return true;
        }
    }
    return false;
}

template<typename T>
bool BST<T>::remove(const T& x){
    return remove(x, root_);
}

template<typename T>
void BST<T>::removeMin(Node*& ptr){
    if (ptr) {
        if (ptr->left)
            removeMin(ptr->left);
        else{
            Node* min = ptr;
            ptr = ptr->right;
            delete min;
            size_--;
        }
    }
}

template<typename T>
BSTNode<T> * BST<T>::findMin(Node* ptr) const{
    if (ptr){
        while (ptr->left)
            ptr = ptr->left;
    }
    return ptr;
}

template<typename T>
BSTNode<T> * BST<T>::find(const T& val, Node* ptr) const{
    while (ptr) {
        if (val < ptr->key)
            ptr = ptr->left;
        else if (ptr->key < val)
            ptr = ptr->right;
        else
            break;
    }
    return ptr;
}

template<typename T>
T* BST<T>::find(const T& val){
    Node* ptr = find(val, root_);
    if (ptr)
        return &ptr->key;
    else
        return (T*)nullptr;
}

template <typename T>
const T *BST<T>::find(const T &val) const{
    const Node* ptr = find(root_, val);
    if (ptr)
        return &ptr->key;
    else
        return (T*)nullptr;
}

template <typename T>
const BSTNode<T>* BST<T>::find(Node* ptr, const T& val ) const{
    while (ptr){
        if (val < ptr->data)
            ptr = ptr->left;
        else if (ptr->data < val)
            ptr = ptr->right;
        else
            break;
    }
    return ptr;
}

template<typename T>
BSTNode<T>* BST<T>::find_It_Helper(const T &val) const{
    return find(val, root_);
}

template<class T>
typename BST<T>::Iterator BST<T>::begin(Node * ptr) const{
    BST<T>::Iterator it(ptr, root_);
    return it;
}

template<typename T>
typename BST<T>::Iterator BST<T>::end() const{

    return Iterator();
}

template<typename T>
BST<T>::Iterator::Iterator(Node *ptr) : current(nullptr){
    Node* tmp = ptr;
    while ( tmp != nullptr) {
        stack_.push(tmp);
        tmp = tmp->left;
    }
    current = stack_.top();
    ++(*this);
}

template<class T>
BST<T>::Iterator::Iterator(Node *node, Node * root) : current(nullptr) {
    if (node == root) {
        Node *tmp = root;
        while (tmp != nullptr) {
            stack_.push(tmp);
            tmp = tmp->left;
        }
        current = stack_.top();
        ++(*this);
    } else {
        current = root;
        while (current != node) {
            if(node->key < current->key) {
                stack_.push(current);
                current = current->left;
            } else {
                current = current->right;
            }
        }
    }

}

template<typename T>
BST<T>::Iterator::Iterator(const BST<T>::Iterator &source) : current(source.current), stack_(source.stack_){}

template<typename T>
typename BST<T>::Iterator &BST<T>::Iterator::operator++(){
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