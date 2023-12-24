from bintrees import RBTree
import btreelib
import random
import time

class BTree:
    def __init__(self):
        self.tree = RBTree()

    def insert(self, key):
        self.tree.insert(key, None)

    def delete(self, key):
        del self.tree[key]

    def search(self, key):
        return key in self.tree

def test_btree_performance(num_elements):
    btree = BTree()

    # Insertion
    start_time = time.time()
    for i in range(num_elements):
        btree.insert(i)
    insertion_time = time.time() - start_time

    # Deletion
    start_time = time.time()
    for i in range(num_elements):
        btree.delete(i)
    deletion_time = time.time() - start_time

    # Querying
    start_time = time.time()
    for i in range(num_elements):
        btree.search(i)
    query_time = time.time() - start_time

    return insertion_time, deletion_time, query_time

def test_btreelib_performance(num_elements):
    btree = btreelib.BTree()

    # Insertion
    start_time = time.time()
    for i in range(num_elements):
        btree.insert(i)
    insertion_time = time.time() - start_time

    # Deletion
    start_time = time.time()
    for i in range(num_elements):
        btree.remove(i)
    deletion_time = time.time() - start_time

    # Querying
    start_time = time.time()
    for i in range(num_elements):
        btree.exist(i)
    query_time = time.time() - start_time

    return insertion_time, deletion_time, query_time

if __name__ == "__main__":
    num_elements = 500000
    rounds = 5

    print("[*] Number of elements: %d" % num_elements)
    print("[*] Number of rounds: %d" % rounds)

    print("[*] Testing bintrees performance...")

    ref_insertion_time, ref_deletion_time, ref_query_time = 0, 0, 0
    for i in range(rounds):
        print("[+] Round %d" % (i + 1))
        insertion_time, deletion_time, query_time = test_btree_performance(num_elements)
        ref_insertion_time += insertion_time
        ref_deletion_time += deletion_time
        ref_query_time += query_time

    print("[*] Average ref insertion time: %.6f seconds" % (ref_insertion_time / rounds))
    print("[*] Average ref deletion time: %.6f seconds" % (ref_deletion_time / rounds))
    print("[*] Average ref query time: %.6f seconds" % (ref_query_time / rounds))

    print("[*] Testing btreelib performance...")

    btreelib_insertion_time, btreelib_deletion_time, btreelib_query_time = 0, 0, 0
    for i in range(rounds):
        print("[+] Round %d" % (i + 1))
        insertion_time, deletion_time, query_time = test_btreelib_performance(num_elements)
        btreelib_insertion_time += insertion_time
        btreelib_deletion_time += deletion_time
        btreelib_query_time += query_time
    
    print("[*] Average btreelib insertion time: %.6f seconds" % (btreelib_insertion_time / rounds))
    print("[*] Average btreelib deletion time: %.6f seconds" % (btreelib_deletion_time / rounds))
    print("[*] Average btreelib query time: %.6f seconds" % (btreelib_query_time / rounds))

    print("[*] btreelib insertion is %.2f times faster than ref insertion" % (ref_insertion_time / btreelib_insertion_time))
    print("[*] btreelib deletion is %.2f times faster than ref deletion" % (ref_deletion_time / btreelib_deletion_time))
    print("[*] btreelib query is %.2f times faster than ref query" % (ref_query_time / btreelib_query_time))