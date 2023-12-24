import unittest
import btreelib as btree


class BTreeTest(unittest.TestCase):
    def test_insert(self):
        keys = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 5.5, -3.3, 0]
        tree = btree.BTree()
        for key in keys:
            tree.insert(key)

    def test_exist(self):
        keys = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
        tree = btree.BTree()
        for key in keys:
            tree.insert(key)
        for key in keys:
            self.assertTrue(tree.exist(key))

        self.assertFalse(tree.exist(0))

    def test_remove(self):
        keys = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
        tree = btree.BTree()
        for key in keys:
            tree.insert(key)
        for key in keys:
            tree.remove(key)
            self.assertFalse(tree.exist(key))

    def test_height(self):
        keys = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
        tree = btree.BTree()
        for key in keys:
            tree.insert(key)
        self.assertEqual(tree.height(), 2)

    def test_size(self):
        keys = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
        tree = btree.BTree()
        for key in keys:
            tree.insert(key)
        self.assertEqual(tree.size(), len(keys))
