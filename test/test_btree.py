import unittest
import btree


class BTreeTest(unittest.TestCase):
    def test_insert(self):
        keys = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
        tree = btree.BTree()
        for key in keys:
            tree.insert(key)
