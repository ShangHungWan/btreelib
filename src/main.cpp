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
        size_t index = find_key_back(key);

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

    long long find_key_back(const T &key) const
    {
        long long index = keys.size() - 1;
        while (index >= 0 && keys[index] > key)
        {
            --index;
        }
        return index;
    }

    long long find_key_front(const T &key) const
    {
        long long index = 0;
        long long size = keys.size();
        while (index < size && key > keys[index])
        {
            ++index;
        }
        return index;
    }

    void remove(const T &key)
    {
        long long index = find_key_front(key);
        long long size = keys.size();

        if (index < size && keys[index] == key)
        {
            if (is_leaf)
            {
                keys.erase(keys.begin() + index);
            }
            else
            {
                if (children[index]->keys.size() >= ORDER)
                {
                    T pred = get_predecessor(index);
                    children[index]->remove(pred);
                    keys[index] = pred;
                }
                else if (children[index + 1]->keys.size() >= ORDER)
                {
                    T succ = get_successor(index);
                    children[index + 1]->remove(succ);
                    keys[index] = succ;
                }
                else
                {
                    merge_children(index);
                    children[index]->remove(key);
                }
            }
        }
        else
        {
            bool last_child = (index == size);

            if (children[index]->keys.size() < ORDER)
            {
                if (index > 0 && children[index - 1]->keys.size() >= ORDER)
                {
                    borrow_from_prev(index);
                }
                else if (index < size && children[index + 1]->keys.size() >= ORDER)
                {
                    borrow_from_next(index);
                }
                else
                {
                    if (index < size)
                    {
                        merge_children(index);
                    }
                    else
                    {
                        merge_children(index - 1);
                    }
                }
            }

            if (last_child && index > size)
            {
                children[index - 1]->remove(key);
            }
            else
            {
                children[index]->remove(key);
            }
        }
    }

private:
    T get_predecessor(int index)
    {
        BTreeNode<T> *current = children[index];
        while (!current->is_leaf)
        {
            current = current->children.back();
        }
        return current->keys.back();
    }

    T get_successor(int index)
    {
        BTreeNode<T> *current = children[index + 1];
        while (!current->is_leaf)
        {
            current = current->children.front();
        }
        return current->keys.front();
    }

    void borrow_from_prev(int index)
    {
        BTreeNode<T> *child = children[index];
        BTreeNode<T> *sibling = children[index - 1];

        for (int i = child->keys.size(); i > 0; --i)
        {
            child->keys[i] = child->keys[i - 1];
        }
        child->keys[0] = keys[index - 1];

        if (!child->is_leaf)
        {
            for (int i = child->children.size(); i > 0; --i)
            {
                child->children[i] = child->children[i - 1];
            }
            child->children[0] = sibling->children.back();
            sibling->children.pop_back();
        }

        keys[index - 1] = sibling->keys.back();
        sibling->keys.pop_back();
    }

    void borrow_from_next(int index)
    {
        BTreeNode<T> *child = children[index];
        BTreeNode<T> *sibling = children[index + 1];

        child->keys.push_back(keys[index]);

        if (!child->is_leaf)
        {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }

        keys[index] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
    }

    void merge_children(int index)
    {
        BTreeNode<T> *child = children[index];
        BTreeNode<T> *sibling = children[index + 1];

        child->keys.push_back(keys[index]);

        for (const T &key : sibling->keys)
        {
            child->keys.push_back(key);
        }

        if (!child->is_leaf)
        {
            for (BTreeNode<T> *c : sibling->children)
            {
                child->children.push_back(c);
            }
        }

        keys.erase(keys.begin() + index);
        children.erase(children.begin() + index + 1);

        delete sibling;
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
            size_t index = node->find_key_back(key);
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

    void remove(const T &key)
    {
        root->remove(key);
        if (root->keys.empty() && !root->is_leaf)
        {
            BTreeNode<T> *old_root = root;
            root = root->children[0];
            delete old_root;
        }
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
        .def("exist", &BTree<double>::exist)
        .def("remove", &BTree<double>::remove);
}
