#include <fstream>
#include <vector>
#include <chrono>
#include <iostream>
#define INF INT_MAX

using namespace std;
using namespace chrono;

const int MAX_DEGREE = 64;

enum State {
    LABELED, UNLABELED, SCANNED
};

class Node;

class Edge {
public:
    Node* end;
    double length;

    Edge(Node* end, double length) {
        this->end = end;
        this->length = length;
    }
};

class Node {
private:
    double value;
    double key;
    int degree;
    bool mark;
    vector<Edge*> neighbours;
    Node* parent;
    Node* left;
    Node* right;
    Node* child;
    State state;
public:
    friend class FibonacciHeap;

    Node(double value, double key = 0) {
        this->value = value;
        this->key = key;
        this->mark = false;
        this->parent = nullptr;
        this->child = nullptr;
        this->left = this->right = this;
        this->degree = 0;
        this->state = UNLABELED;
    }

    void AddNeighbours(Edge* edge) {
        neighbours.push_back(edge);
    }
};

class FibonacciHeap {
private:
    Node* heap;
public:
    FibonacciHeap() {
        heap= Empty();
    }
    ~FibonacciHeap() {
        DeleteAll(heap);
    }

    Node* Insert(Node* elem) {
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

    int GetMin() {
        return heap->value;
    }

    Node* RemoveMinimum() {
        Node* old_min = heap;
        heap = RemoveMinimum(heap);
        return old_min;
    }

    void DecreaseKey(int value, Node* elem) {
        heap = DecreaseKey(heap, elem, value);
    }

    void DeleteAll(Node *heap) {
        if (heap != nullptr) {
            Node *loc_heap = heap;
            do {
                Node *n = loc_heap;
                loc_heap = loc_heap->right;
                DeleteAll(n->child);
                delete n;
            } while (loc_heap != heap);
        }
    }

    static void Dijkstra() {
        auto start = high_resolution_clock::now();
        for (int test = 1; test <= 15; ++test) {
            string file_name2 = "../Tests\\input\\in" + to_string(test) + ".txt";
            int n, m, x;
            ifstream indat(file_name2);
            ofstream fout("output.txt");
            indat >> n;
            indat >> m;
            indat >> x;
            vector<Node*> vertices(n + 1);
            for(int j=1; j <= n; j++) {
                Node* v = new Node(j, INF);
                vertices[j] = v;
            }

            for (int i = 0; i < m; ++i) {
                int tail, head;
                double  length;
                indat >> tail >> head >> length;

                Edge* edge = new Edge(vertices[head], length);
                vertices[tail]->AddNeighbours(edge);
            }
            int src = x;
            vertices[src]->state = LABELED;
            auto* heap = new FibonacciHeap();
            vertices[src]->key = 0;
            heap->Insert(vertices[src]);

            while(!heap->IsEmpty()) {
                Node* v = heap->RemoveMinimum();
                v->state = SCANNED;
                for(int i = 0; i < v->neighbours.size(); i++) {
                    Edge* currentEdge = v->neighbours[i];
                    Node* headOfCurrentEdge = currentEdge->end;

                    if(headOfCurrentEdge->state == UNLABELED) {
                        headOfCurrentEdge->state = LABELED;
                        headOfCurrentEdge->key = v->key + currentEdge->length;
                        heap->Insert(headOfCurrentEdge);
                    }
                    else if(headOfCurrentEdge->key > v->key + currentEdge->length) {
                        heap->DecreaseKey(v->key + currentEdge->length, headOfCurrentEdge);
                    }
                }
            }

            auto stop = high_resolution_clock::now();



            for (int i = 1; i < vertices.size(); ++i) {
                if (vertices[i]->key == INT_MAX) {
                    fout << "-1 ";
                    vertices[i]->key = -1;
                }
                else fout << vertices[i]->key << " ";
            }


            string file_name = "../Tests\\answer\\ans" + to_string(test) + ".txt";
            ifstream ans(file_name);
            vector<int> answers(vertices.size());
            for (int i = 1; i < answers.size(); ++i) {
                ans >> answers[i];
            }
            bool is_ok = true;
            for (int i = 1; i < answers.size(); ++i) {
                if (answers[i] != vertices[i]->key) {
                    is_ok = false;
                    break;
                }
            }

            cout << test << ") ";
            if (is_ok){
                cout << "OK" << endl;
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
            }
            else cout << "ERROR" << endl;
        }
    }

private:
    Node* Empty() {
        return nullptr;
    }

    Node* Merge(Node* first, Node* second) {
        if (first == nullptr)
            return second;
        if (second == nullptr)
            return first;
        if (first->key > second->key)
            swap(first, second);
        Node* A = first->right;
        Node* B = second->left;
        first->right = second;
        second->left = first;
        A->left = B;
        B->right = A;
        return first;
    }

    void AddChild(Node* parent, Node* child) {
        child->left= child->right=child;
        child->parent=parent;
        parent->degree++;
        parent->child= Merge(parent->child, child);
    }

    void UnMarkAll(Node* elem) {
        if (elem == nullptr)
            return;
        Node* loc_elem=elem;
        do {
            loc_elem->mark = false;
            loc_elem->parent = nullptr;
            loc_elem = loc_elem->right;
        } while (loc_elem != elem);
    }


    Node* RemoveMinimum(Node* heap) {
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
        Node* min = heap;
        Node* start = heap;
        do {
            if (heap->key < min->key)
                min = heap;
            heap = heap->right;
        } while (heap != start);
        return min;
    }

    Node* Consolidate(Node* heap) {
        vector<Node*> trees(MAX_DEGREE);
        while(true) {
            if (trees[heap->degree] != nullptr) {
                Node* t = trees[heap->degree];
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

    Node* Cut(Node* heap, Node* elem) {
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

    Node* DecreaseKey(Node* heap, Node* n, int key) {
        if (n->key < key)
            return heap;
        n->key = key;
        if (n->parent) {
            if (n->key < n->parent->key) {
                heap = Cut(heap, n);
                Node* parent = n->parent;
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


int main() {
    FibonacciHeap::Dijkstra();
    return 0;
}