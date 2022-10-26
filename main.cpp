#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define INF INT_MAX

enum State {
    LABELED, UNLABELED, SCANNED
};

class Node;

class Edge {
public:
    Node *tail;
    Node *head;
    int length;

    Edge(Node *tail, Node *head, int length) {
        this->tail = tail;
        this->head = head;
        this->length = length;
    }

};

class Node {
public:
    friend class FibonacciHeap;
    vector<Edge*> incoming_edges;
    vector<Edge*> outgoing_edges;
    Node *parent;
    Node *left;
    Node *right;
    Node *child;
    State state;
public:

    bool mark;
    int value;
    int degree;
    int key;

    void AddIncomingEdge(Edge *edge) {
        incoming_edges.push_back(edge);
    }

    void AddOutgoingEdge(Edge * edge) {
        outgoing_edges.push_back(edge);
    }

};

bool iter(Edge *a, Edge *b) {
    return a->length < b->length;
}

class FibonacciHeap {
private:

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

    static Node *MakeNode(int value, int key = 0) {
        Node *new_node = new Node();
        new_node->value = value;
        new_node->key = key;
        new_node->parent = nullptr;
        new_node->child = nullptr;
        new_node->left = new_node->right = new_node;
        new_node->degree = 0;
        new_node->state = UNLABELED;
        return new_node;
    }

    Node* Merge(Node *first, Node *second) {
        if (first == nullptr)
            return second;
        if (second == nullptr)
            return first;
        if (first->key > second->key) {
            Node* loc_elem = first;
            first = second;
            second = loc_elem;
        }
        Node *A = first->left;
        Node *B = second->left;
        A->right = second;
        second->left = A;
        first->left = B;
        B->right = first;
        return first;
    }

    Node *RemoveMin(Node *elem) {
        if (elem == nullptr) {
            return elem;
        }
        UnMarkAll(elem->child);
        if (elem == elem->right) {
            elem = elem->child;
        }
        else {
            elem->right->left = elem->left;
            elem->left->right = elem->right;
            elem = Merge(elem->right, elem->child);
        }
        if (elem == nullptr) {
            return elem;
        }

        vector<Node*> trees(64, nullptr);
        while (true) {
            if (trees[elem->degree] != nullptr) {
                Node *loc_elem = trees[elem->degree];
                if (elem == loc_elem) {
                    break;
                }
                trees[elem->degree] = nullptr;
                loc_elem->left->right = loc_elem->right;
                loc_elem->right->left = loc_elem->left;
                if (elem->value < loc_elem->value) {
                    AddChild(elem, loc_elem);
                }
                else {
                    if (elem == elem->right) {
                        loc_elem->right = loc_elem->left = loc_elem;
                        AddChild(loc_elem, elem);
                        elem = loc_elem;
                    }
                    else {
                        elem->left->right = loc_elem;
                        elem->right->left = loc_elem;
                        loc_elem->right = elem->right;
                        loc_elem->left = elem->left;
                        AddChild(loc_elem, elem);
                        elem = loc_elem;
                    }
                }
                continue;
            }
            else {
                trees[elem->degree] = elem;
            }
            elem = elem->right;
//            if (elem->key < min->key) {
//                min = elem;
//            }
        }
        Node *min_elem = elem, *start = elem;
        do {
            if (elem->value < min_elem->value)
                min_elem = elem;
            elem = elem->right;
        } while (elem != start);
        return min_elem;
    }

    Node* DecreaseKey(Node *heap, Node *elem, int key) {
        if (elem->key < key) {
            return heap;
        }
        elem->key = key;
        if (elem->parent != nullptr) {
            if (elem->key < elem->parent->key) {
                heap = Cut(heap, elem);
                Node* parent = elem->parent;
                elem->parent = nullptr;
                while (parent != nullptr && parent->mark) {
                    heap = Cut(heap,parent);
                    elem = parent;
                    parent = elem->parent;
                    elem->parent = nullptr;
                }
                if (parent != nullptr && parent->parent != nullptr) {
                    parent->mark = true;
                }
            }
        }
        else if (elem->key < heap->key)
                heap = elem;

        return heap;
    }

    Node* Find(Node *heap, int key) {
        Node *loc_heap = heap;
        if (loc_heap == nullptr) {
            return nullptr;
        }
        do {
            if (loc_heap->key == key) {
                return loc_heap;
            }
            Node *result = Find(loc_heap->child, key);
            if (result != nullptr) {
                return result;
            }
            loc_heap = loc_heap->right;
        } while (loc_heap != heap);
        return nullptr;
    }

    void UnMarkAll(Node* elem) {
        if (elem == nullptr) {
            return;
        }
        Node *loc_elem = elem;
        do {
            loc_elem->mark = false;
            loc_elem->parent = nullptr;
            loc_elem = loc_elem->right;
        } while (loc_elem != elem);
    }

    void AddChild(Node *parent, Node *child) {
        child->left = child->right = child;
        child->parent = parent;
        parent->degree++;
        parent->child = Merge(parent->child,child);
    }

    Node* Cut(Node *heap, Node *elem)       {
        if (elem->right == elem) {
            elem->parent->child = nullptr;
        }
        else {
            elem->right->left = elem->left;
            elem->left->right = elem->right;
            elem->parent->child = elem->right;
        }
        elem->right = elem->left = elem;
        elem->mark  = false;
        return Merge(heap, elem);
    }

public:
    Node *min;

    FibonacciHeap() {
        min = Empty();
    }

    ~FibonacciHeap()    {
        DeleteAll(min);
    }

    Node *Empty() {
        return nullptr;
    }

    bool isEmpty() {
        return min == nullptr;
    }

    Node* GetMin() {
        return min;
    }

    Node *Insert(int value, int key) {
        Node *new_elem = MakeNode(value, key);
        min = Merge(min, new_elem);
        return new_elem;
    }

    Node *Insert(Node *new_elem) {
        min = Merge(min, new_elem);
        return new_elem;
    }

    Node *Merge(FibonacciHeap other_heap) {
        return min = Merge(min, other_heap.min);
    }

    Node *RemoveMin() {
        Node* old_min = min;
        min = RemoveMin(min);
        return old_min;
    }

    Node *DecreaseKey(Node *elem, int value) {
        if (elem == nullptr)
            return elem;
        return min = DecreaseKey(min, elem, value);
    }

    void DisplayRoots() {
        Node *loc_heap = min;
        if (loc_heap == nullptr) {
            cout << "Empty" << endl;
            return;
        }
        do {
            cout << loc_heap->value;
            loc_heap = loc_heap->right;
            if (loc_heap != min)
            {
                cout<<" --> ";
            }
        }
        while (loc_heap != min);
        cout << endl;
    }

    Node* Find(int value) {
        return Find(min, value);
    }

    static void Dijkstra() {

        int COUNT_TESTS = 9;
        ifstream input, answer;
        ofstream output("Tests\\out.txt");
        ifstream output_2("Tests\\out.txt");

        for (int number_test = 1; number_test <= 9; ++number_test) {
            input.open("Tests\\input\\in" + to_string(number_test) + ".txt");
            answer.open("Tests\\answer\\ans"  + to_string(number_test) + ".txt");

            int count_vertexes, count_edge, start, tail, head, length;
            input >> count_vertexes >> count_edge >> start;

            vector<Node*> vertices(count_vertexes + 1);
            vector<Edge*> edges(count_edge);

            for (int i = 1; i <= count_vertexes; ++i) {
                vertices[i] = (MakeNode(i, INF));
            }



            vertices[start]->key = 0;
            for (int i = 0; i < count_edge; ++i) {
                input >> tail >> head >> length;
                Edge* edge = new Edge(vertices[tail], vertices[head], length);
                edge->tail->AddOutgoingEdge(edge);
                edge->head->AddIncomingEdge(edge);
                edges[i] = edge;
            }

            vertices[start]->state = LABELED;
            FibonacciHeap FibHeap =  FibonacciHeap();

            FibHeap.Insert(vertices[start]);

            for (int i = 1; i < vertices.size(); ++i) {
                sort(vertices[i]->incoming_edges.begin(), vertices[i]->incoming_edges.end(), iter);
                sort(vertices[i]->outgoing_edges.begin(), vertices[i]->outgoing_edges.end(), iter);
            }

            do {
                Node* v = FibHeap.RemoveMin();
                v->state = SCANNED;
                for (int i = 0; i < v->outgoing_edges.size(); i++) {
                    Edge* currentEdge = v->outgoing_edges[i];
                    Node* headOfCurrentEdge = currentEdge->head;

                    if (headOfCurrentEdge->state == SCANNED) {
                        continue;
                    }
                    if (headOfCurrentEdge->state == UNLABELED) {
                        headOfCurrentEdge->state = LABELED;
                        headOfCurrentEdge->key = v->key + currentEdge->length;
                        FibHeap.Insert(headOfCurrentEdge);
                    }
                    else if (headOfCurrentEdge->key > v->key + currentEdge->length) {
                        FibHeap.DecreaseKey(headOfCurrentEdge, v->key + currentEdge->length);
                    }
                    vertices[v->value] = v;
                }
            }
            while(!FibHeap.isEmpty());

            for (int i = 1; i < vertices.size(); ++i) {
                if (vertices[i]->key != 2147483647) {
                    output << vertices[i]->key << " ";
                }
                else output << "-1 ";
            }
            output << endl;

            string input_ans, corr_ans;
            getline(output_2, input_ans);
            getline(answer, corr_ans);

            if (input_ans == corr_ans) {
                system("color 02" );
                cout << "OK" << endl;
            }
            else {
                system("color 04" );
                cout << "ERROR" << endl;
            }
            input.close();
            answer.close();
        }
        output.close();


    }

};

int InputInt() {
    string s;
    int answer;
    while (true) {
        cin >> s;
        try {
            answer = stoi(s);
            break;
        }
        catch (exception &e) {
            cout << "Incorrect input." << endl;
            cout << "Try again:" << endl;
        }
    }
    return answer;
}

void Menu(FibonacciHeap FibHeap) {
    int choice, key, value;
    Node *node;
    while (true) {
        cout << endl;
        cout << "Operations on Fibonacci heap" << endl;
        cout << "1) Insert element in the heap" << endl;
        cout << "2) Insert a set of elements" << endl;
        cout << "3) Remove minimum" << endl;
        cout << "4) Get minimum" << endl;
        cout << "5) Display Heap" << endl;
        cout << "6) Decrease key" << endl;
        cout << "7) Exit" << endl;
        cout << "Enter Your Choice: ";

        choice = InputInt();
        switch (choice) {
            case 1:
                cout << "Enter element's value and key:";
                value = InputInt();
                key = InputInt();
                FibHeap.Insert(value, key);
                break;

            case 2:
                int count;
                cout << "Enter the count of element: ";
                count = InputInt();
                cout << "Enter elements: ";
                for (int i = 0; i < count; ++i) {
                    cin >> value >> key;
                    FibHeap.Insert(value, key);
                }
                break;
            case 3:
                node = FibHeap.RemoveMin();
                cout << (node != nullptr ? to_string(node->value) :  "Heap is empty");
                break;

            case 4:
                node = FibHeap.GetMin();
                if (node != nullptr) {
                    cout << "Minimum: " << FibHeap.GetMin()->value << endl;
                }
                else {
                    cout << "Heap is empty" << endl;
                }
                break;

            case 5:
                cout << "The Heap is: " << endl;
                FibHeap.DisplayRoots();
                break;

            case 6:
                cout << "Enter the element: ";
                key = InputInt();
                cout << "Enter new value: ";
                value = InputInt();
                FibHeap.DecreaseKey(FibHeap.Find(key), value);
                break;

            case 7:
                return;

            default:
                cout << "Wrong Choice" << endl;
        }
    }
}


void RuCode() {
    FibonacciHeap Fib = FibonacciHeap();
    string b;
    int a;
    while (cin >> b) {
        if (b ==  "ADD") {
            cin >> a;
            Fib.Insert(a, a);
        }
        else if (b == "EXTRACT") {
            Node* min = Fib.RemoveMin();
            if (min != nullptr) {
                cout << min->value << endl;
            }
            else cout << "CANNOT\n";
        }
        else if (b == "CLEAR") {
            Fib = FibonacciHeap();
        }
    }
    return;
}


int main() {
    //FibonacciHeap::Dijkstra();
//    ofstream cout("graph.txt");
//    int n, m, a;
//    cin >> n >> m >> a;
//    cout << n << " " << m << " " << a << endl;
//    for (int i = 0; i < m; ++i) {
//        int f = 0, s = 0;
//        while (f == s) {
//            f = rand() % n + 1;
//            s = rand() % n + 1;
//        }
//        cout << f << " " << s << " " << rand() % 100000 + 1 << endl;
//    }

    //FibonacciHeap::Dijkstra();
    Menu(FibonacciHeap());
    return 0;
}
