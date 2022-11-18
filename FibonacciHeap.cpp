#include <fstream>
#include <vector>
#include <iostream>

using namespace std;
const int MAX_DEGREE = 64;

template <class V, class P> class FibonacciHeap;
template <class V, class P> class Node;

enum State {
    LABELED, UNLABELED, SCANNED
};

template <class V, class P>
class Edge {
public:
    Node<V, P>* end;
    int length;

    Edge(Node<V, P>* end, double length) {
        this->end = end;
        this->length = length;
    }
};

template <class V, class P>
class Node {
public:
    V value;
    P key;
    int degree;
    bool mark;
    Node<V, P>* parent;
    Node<V, P>* left;
    Node<V, P>* right;
    Node<V, P>* child;
    State state;
    vector<Edge<V, P>*> neighbours;
    void AddNeighbours(Edge<V, P>* edge) {
        neighbours.push_back(edge);
    }
    Node(V value, P key = 0) {
        this->value = value;
        this->key = key;
        this->mark = false;
        this->parent = nullptr;
        this->child = nullptr;
        this->left = this->right = this;
        this->degree = 0;
        this->state = UNLABELED;
    }
};

template <class V, class P> class FibonacciHeap {
private:
    Node<V, P>* heap;
public:
    FibonacciHeap() {
        heap = Empty();
    }

    ~FibonacciHeap() {
        DeleteAll(heap);
    }

    Node<V, P>* Insert(Node<V, P>* elem) {
        heap = Merge(heap, elem);
        return elem;
    }

    void Merge(FibonacciHeap& other) {
        heap = Merge(heap, other.heap);
        other.heap = Empty();
    }

    bool IsEmpty() {
        return heap == nullptr;
    }

    V GetMin() {
        return heap->value;
    }

    Node<V, P>* RemoveMinimum() {
        Node<V, P>* old_min = heap;
        heap = RemoveMinimum(heap);
        return old_min;
    }

    void DecreaseKey(V value, Node<V, P>* elem) {
        heap = DecreaseKey(heap, elem, value);
    }

    void DeleteAll(Node<V, P>* heap) {
        if (heap != nullptr) {
            Node<V, P>* loc_heap = heap;
            do {
                Node<V, P>* n = loc_heap;
                loc_heap = loc_heap->right;
                DeleteAll(n->child);
                delete n;
            } while (loc_heap != heap);
        }
    }

private:
    Node<V, P>* Empty() {
        return nullptr;
    }

    Node<V, P>* Merge(Node<V, P>* first, Node<V, P>* second) {
        if (first == nullptr)
            return second;
        if (second == nullptr)
            return first;
        if (first->key > second->key)
            swap(first, second);
        Node<V, P>* A = first->right;
        Node<V, P>* B = second->left;
        first->right = second;
        second->left = first;
        A->left = B;
        B->right = A;
        return first;
    }

    void AddChild(Node<V, P>* parent, Node<V, P>* child) {
        child->left = child->right = child;
        child->parent = parent;
        parent->degree++;
        parent->child = Merge(parent->child, child);
    }

    void UnMarkAll(Node<V, P>* elem) {
        if (elem == nullptr)
            return;
        Node<V, P>* loc_elem = elem;
        do {
            loc_elem->mark = false;
            loc_elem->parent = nullptr;
            loc_elem = loc_elem->right;
        } while (loc_elem != elem);
    }


    Node<V, P>* RemoveMinimum(Node<V, P>* heap) {
        UnMarkAll(heap->child);
        if (heap->right == heap)
            heap = heap->child;
        else {
            heap->right->left = heap->left;
            heap->left->right = heap->right;
            heap = Merge(heap->right, heap->child);
        }
        if (heap == nullptr)
            return heap;

        heap = Consolidate(heap);
        Node<V, P>* min = heap;
        Node<V, P>* start = heap;
        do {
            if (heap->key < min->key)
                min = heap;
            heap = heap->right;
        } while (heap != start);
        return min;
    }

    Node<V, P>* Consolidate(Node<V, P>* heap) {
        vector<Node<V, P>*> trees(MAX_DEGREE);
        while (true) {
            if (trees[heap->degree] != nullptr) {
                Node<V, P>* t = trees[heap->degree];
                if (t == heap)
                    break;
                trees[heap->degree] = nullptr;
                if (heap->key < t->key) {
                    t->left->right = t->right;
                    t->right->left = t->left;
                    AddChild(heap, t);
                }
                else {
                    t->left->right = t->right;
                    t->right->left = t->left;
                    if (heap->right == heap) {
                        t->right = t->left = t;
                        AddChild(t, heap);
                        heap = t;
                    }
                    else {
                        heap->left->right = t;
                        heap->right->left = t;
                        t->right = heap->right;
                        t->left = heap->left;
                        AddChild(t, heap);
                        heap = t;
                    }
                }
                continue;
            }
            else
                trees[heap->degree] = heap;
            heap = heap->right;
        }
        return heap;
    }

    Node<V, P>* Cut(Node<V, P>* heap, Node<V, P>* elem) {
        if (elem->right == elem)
            elem->parent->child = nullptr;
        else {
            elem->right->left = elem->left;
            elem->left->right = elem->right;
            elem->parent->child = elem->right;
        }
        elem->right = elem->left = elem;
        elem->mark  = false;
        return Merge(heap, elem);
    }

    Node<V, P>* DecreaseKey(Node<V, P>* heap, Node<V, P>* n, int key) {
        n->key = key;
        if (n->parent) {
            if (n->key < n->parent->key) {
                heap = Cut(heap, n);
                auto* parent = n->parent;
                n->parent = nullptr;
                while (parent != nullptr && parent->mark) {
                    heap = Cut(heap, parent);
                    n = parent;
                    parent = n->parent;
                    n->parent = nullptr;
                }
                if (parent != nullptr && parent->parent != nullptr)
                    parent->mark = true;
            }
        }
        else if (n->key < heap->key)
            heap = n;
        return heap;
    }
};
