#include <cassert>
#include <iostream>

using namespace std;

template <typename T>
struct TreeNode {
    T value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

template <typename T>
void DeleteTree(TreeNode<T>* node) {
    if (!node) {
        return;
    }
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

// Возвращает true, если узел node и его дочерние элементы находятся в диапазоне [min, max]
template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) {
    if (!node) {
        return true;
    }

    if ((min && (node->value <= *min)) || (max && (node->value >= *max))) {
        return false;
    }

    return CheckTreeProperty<T>(node->left, min, &node->value) 
        && CheckTreeProperty<T>(node->right, &node->value, max);
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

int main() {
    using T = TreeNode<int>;
    T* root1 = new T{ 6,
        new T{4, new T{3}, new T{5}}, new T{7} };
    assert(CheckTreeProperty(root1));

    T* root2 = new T{ 6,
        new T{4, new T{3}, new T{5}}, new T{7, new T{8}, nullptr} };
    assert(!CheckTreeProperty(root2));

    T* root3 = new T{ 6 };
    assert(CheckTreeProperty(root3));

    T* root4 = nullptr;
    assert(CheckTreeProperty(root4));

    T* root5 = new T{ 6, new T{4}, new T{7} };
    assert(CheckTreeProperty(root5));

    // ok
    T* root6 = new T{ 7, new T{4, new T{1, nullptr, new T{3, new T{2}, nullptr}}, new T{5, nullptr, new T{6}}},
                        new T{10, new T{9}, new T{11}} };

    assert(CheckTreeProperty(root6));

    // not search three
    T* root7 = new T{ 7, new T{4, new T{1, nullptr, new T{3, new T{2}, nullptr}}, new T{5, nullptr, new T{8}}},
                    new T{10, new T{9}, new T{11}} };

    assert(!CheckTreeProperty(root7));

    DeleteTree(root1);
    DeleteTree(root2);
    DeleteTree(root3);
    DeleteTree(root4);
    DeleteTree(root5);
    DeleteTree(root6);
    DeleteTree(root7);
}