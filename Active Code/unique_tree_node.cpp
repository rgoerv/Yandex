#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right))
    {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) noexcept
{
    if (!node) {
        return true;
    }

    if ((min && (node->value <= *min)) || (max && (node->value >= *max))) {
        return false;
    }

    return CheckTreeProperty<T>(node->left.get(), min, &node->value)
        && CheckTreeProperty<T>(node->right.get(), &node->value, max);
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept
{
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept
{
    if (!node || (node && !node->left)) {
        return node;
    }
    return begin<T>(node->left.get());
}

template <class T>
TreeNode<T>* top(TreeNode<T>* node)
{
    if ((node && node->parent) && (node == node->parent->right.get())) {
        return top<T>(node->parent);
    } else if ((node && node->parent) && (node == node->parent->left.get())) {
        return node->parent;
    }
    return nullptr;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept
{
    if (node && node->right.get()) {
        return begin(node->right.get());
    }

    if (node && !node->right.get()) {
        return top<T>(node);
    }

    return nullptr;
}

// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {})
{
    TreeNodePtr<int> res = std::make_unique<TreeNode<int>>(TreeNode<int> { val, std::move(left), std::move(right) });
    if (res.get()->left) {
        res->left->parent = res.get();
    }
    if (res.get()->right) {
        res->right->parent = res.get();
    }

    return res;
}

int main()
{
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    system("pause");
    // Функция DeleteTree не нужна. Узлы дерева будут рекурсивно удалены
    // благодаря деструкторам unique_ptr
}