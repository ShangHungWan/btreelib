# btreelib

About A B-Tree search library which implemented with C++ to achieve the
performance. If you don't know what's B-tree, you can refer to
[wiki](https://en.wikipedia.org/wiki/B-tree) [2] or original paper (Douglas
Comer. 1979. Ubiquitous B-Tree. ACM Comput. Surv. 11, 2 (June 1979), 121–137.
<https://doi.org/10.1145/356770.356776>).

## Requirements

You can build it from scratch by following steps below:

First install:

- Python3 (w/ pip)
- g++
- make

Then run commands below:

```shell
python3 -m pip install -r requirements.txt
```

## Usage

After install dependencies, you can use `make`:

```shell
make
```

This will generate a `btreelib.so` in the current folder, you can move it to
folder that contains your Python script and import it like:

```python
import btreelib
```

For more example, please refer to [Example](#example).

## Basic Information

This library can help users to establish an B-tree easily and do other
operations like: insert, search, and delete.

## Problem to Solve

A B-tree is a self-balancing tree data structure that maintains sorted data and
allows for efficient search, insertions, and deletions. B-trees are commonly
used in databases and file systems, where they provide fast access to stored
records, especially in scenarios where the data is too large to fit in memory.

The B-tree structure differs from binary trees by allowing each node to have
multiple keys and children, providing a balance between the height of the tree
and the number of keys in each node.

## Prospective Users

Those who want to develop an application that needs to store/search
one-dimension data like numbers.

The most important thing is that they can use both C++ and Python to include
this library. C++ can directly include the library. On the other hand, this
library also provides APIs for Python user.

## System Architecture

### Interfaces

This library will provide C++ interfaces:

- BTree<T> BTree<T>::BTree()
- void BTree<T>::insert(const T &key)
- bool BTree<T>::exist(const T &key)
- void BTree<T>::remove(const T &key)
- size_t BTree<T>::size()
- size_t BTree<T>::height()

And wrap it as Python APIs:

- BTree BTree()
- None insert(key)
- bool exist(key)
- None remove(key)
- int size()
- int height()

You can also find more examples in `tests/` folder.

### Example

```python
import btreelib

btree = btreelib.BTree()

keys = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 5.5, -3.3, 0]
for key in keys:
    btree.insert(key)

print(btree.size()) # 14
print(btree.height()) # 2

print(btree.exist(0)) # True
print(btree.exist(-1)) # False

btree.remove(1)
btree.remove(2)

print(btree.size()) # 12
```

### Testing

This library will use randomly generated dataset as the input to make sure its
performance is satisfied.

Moreover, in order to make sure its implementations are the same as our
expectation. This library contains unittest that written in Python.

## API Description

Users can include btreelib while compiling the code, for example:

```bash
g++ main.cpp -o main -lbtreelib
```

Furthermore, users who developing in Python can import it as well, for example:

```python
import btreelib
```

About interfaces, please refer to [Interfaces](#interfaces).

## Performance

In the library, we compare our performance with
[`bintrees`](https://github.com/mozman/bintrees), a library that written in
Python and Cython/C. We randomly generate `500000` elements and operate
insertion, deletion and query. Moreover, to eliminate the error, we ran 5 times
and get the average result. Here is the result:

```text
[*] Number of elements: 500000
[*] Number of rounds: 5
[*] Testing bintrees performance...
[+] Round 1
[+] Round 2
[+] Round 3
[+] Round 4
[+] Round 5
[*] Average ref insertion time: 3.166987 seconds
[*] Average ref deletion time: 3.775223 seconds
[*] Average ref query time: 0.138544 seconds
[*] Testing btreelib performance...
[+] Round 1
[+] Round 2
[+] Round 3
[+] Round 4
[+] Round 5
[*] Average btreelib insertion time: 0.123350 seconds
[*] Average btreelib deletion time: 0.106254 seconds
[*] Average btreelib query time: 0.069278 seconds
[*] btreelib insertion is 25.67 times faster than ref insertion
[*] btreelib deletion is 35.53 times faster than ref deletion
[*] btreelib query is 2.00 times faster than ref query
```

As you can see, we outperform the `bintrees` on all three operations. In
deletion, we even achieve 35x improvement.

We also encourage users to perform this test on your device by:

```shell
make performance
```

## Engineering Infrastructure

### Build System

- make
- g++
- CI by GitHub Actions

### Version control

- Git
- GitHub
- GitHub Flow

### Testing Framework

- pytest

### Documentation

- Markdown

## References

- [1] Douglas Comer. 1979. Ubiquitous B-Tree. ACM Comput. Surv. 11, 2 (June
    1979), 121–137. <https://doi.org/10.1145/356770.356776>
- [2] B-tree - Wikipedia. Retrieved from
    <https://en.wikipedia.org/wiki/B-tree>
