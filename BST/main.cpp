#include <iostream>
#include "bstTest.h"

using namespace std;

int main() {



    BST<int> bst;

    bst.insert(15);
    bst.insert(35);
    bst.insert(12);
    bst.insert(2);
    bst.insert(6);
    bst.insert(14);
    bst.insert(34);
    bst.insert(15);
    bst.insert(678);
    bst.insert(23);
    bst.insert(1);
    bst.insert(9486);
    bst.insert(579);

//    bst.inOrder();

    auto it = bst.begin();
    auto it2 = bst.begin();
    ++it;
    ++it2;
    bool a = (it == it2);
    cout << "ROZNY : " << a << endl;

    for(auto it = bst.begin(); it != bst.end(); ++it)
        cout << *it << ", ";

    cout << endl;
}


