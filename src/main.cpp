#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <iostream>
#include <vector>

const int ORDER = 3;

template <typename T>
class BTreeNode
{
public:
    bool is_leaf;
    std::vector<T> keys;
    std::vector<BTreeNode<T> *> children;

    BTreeNode(bool is_leaf = true) : is_leaf(is_leaf) {}

    bool is_full()
    {
        return keys.size() == 2 * ORDER - 1;
    }

    void insert_non_full(const T &key)
    {
        size_t index = find_key(key);

        if (is_leaf)
        {
            keys.insert(keys.begin() + index + 1, key);
        }
        else
        {
            if (children[index + 1]->is_full())
            {
                split_child(index + 1);
                if (key > keys[index + 1])
                {
                    ++index;
                }
            }

            children[index + 1]->insert_non_full(key);
        }
    }

    void split_child(int child_index)
    {
        BTreeNode<T> *child = children[child_index];
        keys.insert(keys.begin() + child_index, child->keys[ORDER - 1]);

        BTreeNode<T> *new_child = new BTreeNode<T>(child->is_leaf);
        new_child->keys.assign(child->keys.begin() + ORDER, child->keys.end());

        children.insert(children.begin() + child_index + 1, new_child);

        child->keys.resize(ORDER - 1);

        if (!child->is_leaf)
        {
            new_child->children.assign(child->children.begin() + ORDER, child->children.end());
            child->children.resize(ORDER);
        }
    }

    size_t find_key(const T &key)
    {
        int index = keys.size() - 1;
        while (index >= 0 && keys[index] > key)
        {
            --index;
        }
        return index;
    }
};

template <typename T>
class BTree
{
public:
    BTreeNode<T> *root;

    BTree() : root(nullptr)
    {
        root = new BTreeNode<T>();
    }

    void insert(const T &key)
    {
        if (root->is_full())
        {
            split_root();
        }
        root->insert_non_full(key);
    }

    bool exist(const T &key)
    {
        BTreeNode<T> *node = root;
        while (node != nullptr)
        {
            size_t index = node->find_key(key);
            if (index < node->keys.size() && node->keys[index] == key)
            {
                return true;
            }
            else if (node->is_leaf)
            {
                return false;
            }
            else
            {
                node = node->children[index + 1];
            }
        }
        return false;
    }

private:
    void split_root()
    {
        BTreeNode<T> *new_root = new BTreeNode<T>(false);
        new_root->children.push_back(root);
        root = new_root;
        root->split_child(0);
    }
};

PYBIND11_MODULE(btree, m)
{
    m.doc() = "A B-Tree library which implemented with C++ to achieve the performance. Furthermore, it wrapped with Python to achieve useability at the same time.";

    pybind11::class_<BTree<double>>(m, "BTree")
        .def(pybind11::init<>())
        .def("insert", &BTree<double>::insert)
        .def("exist", &BTree<double>::exist);
}