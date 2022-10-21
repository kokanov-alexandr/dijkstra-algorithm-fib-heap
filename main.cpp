#include <fstream>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

enum State {
    LABELED, UNLABELED, SCANNED
};

class Node;

class Edge
{
public:
    Node *tail;
    Node *head;
    double length;

    Edge(Node *tail, Node *head, double length)
    {
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

int MaxSize = 1000;
class FibonacciHeap {
private:

    vector<Node*> rootListByRank = vector<Node*>(MaxSize);

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
        if (first == nullptr) {
            return second;
        }
        if (second == nullptr) {
            return first;
        }
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
                        loc_elem->left = elem->left;
                        loc_elem->right = elem->right;
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
            if (elem->key < min->key) {
                min = elem;
            }
        }
        return elem;
    }

    Node* DecreaseKey(Node *heap, Node *elem, int key) {
        if (elem->key < key) {
            return heap;
        }
        elem->key = key;
        if (elem->parent) {
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
        else {
            if (elem->key < heap->key) {
                heap = elem;
            }
        }
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
        if (elem != nullptr) {
            return min = DecreaseKey(min, elem, value);
        }
        return elem;
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
        vector<Node*> vertices;
        vector<Edge*> edges;
        ifstream input("C:\\Users\\HP\\CLionProjects\\dijkstra-algorithm-fibonacci-heap\\Graph.txt");
        if (input) {
            int n;
            input >> n;
            for (int i = 0; i < n; ++i) {
                vertices.push_back(MakeNode(i, 0));
            }

            while (!input.eof()) {
                int tail, head, length;
                input >> tail >> head >> length;
                Edge* edge = new Edge(vertices[tail], vertices[head], length);
                edge->head->AddIncomingEdge(edge);
                edge->tail->AddOutgoingEdge(edge);
                edges.push_back(edge);
            }
        }

        else {
            cout << "File will not Find!\n";
        }

        int start, finish;
        cout << "\nEnter the first vertex: ";
        cin >> start;
        cout << "\nEnter the second vertex: ";
        cin >> finish;

        vertices.push_back(MakeNode(start, 0));
        vertices[finish]->state = LABELED;
        FibonacciHeap FibHeap =  FibonacciHeap();

        FibHeap.Insert(vertices[finish]);

        do
        {
            FibHeap.DisplayRoots();
            Node* v = FibHeap.RemoveMin();
            v->state = SCANNED;

            for(int i = 0; i < v->incoming_edges.size(); i++)
            {
                Edge* currentEdge = v->incoming_edges[i];
                Node* headOfCurrentEdge = currentEdge->tail;

                if(headOfCurrentEdge->state != SCANNED)
                {
                    if(headOfCurrentEdge->state == UNLABELED)
                    {
                        headOfCurrentEdge->state = LABELED;
                        headOfCurrentEdge->right = v;
                        headOfCurrentEdge->key = v->key + currentEdge->length;
                        FibHeap.Insert(headOfCurrentEdge);
                    }
                    else if(headOfCurrentEdge->key > v->key + currentEdge->length )
                    {
                        headOfCurrentEdge->right = v;
                        FibHeap.DecreaseKey(headOfCurrentEdge, v->key + currentEdge->length);
                    }
                }
            }
        }
        while(!FibHeap.isEmpty());

        Node* temp = vertices[start];


        if(!temp->right)
        {
            printf("There exist no s-t paths\n");
            return;
        }

        int vertexCount = 0;

        printf("\nDistance: %d\n", (int)vertices[start]->key);

        while(temp)
        {
            cout << temp->value;
            temp = temp->right;
            if(temp)
                printf(" -> ");

            vertexCount++;
        }
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

//fstream fin ("in.txt", ios::in);
//fstream fout ("out.txt", ios::out);

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
                cout << FibHeap.RemoveMin()->value;
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

int main() {
    FibonacciHeap Fib = FibonacciHeap();
    Menu(Fib);
    return 0;
}
