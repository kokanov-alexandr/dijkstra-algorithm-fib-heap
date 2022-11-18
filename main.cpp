#include <vector>
#include <chrono>
#include <iostream>
#include "FibonacciHeap.cpp"

#define INF INT_MAX

using namespace std;

const int COUNT_TESTS = 30;

   
pair<vector<Node<int, int>*>, int> ScanGraphDijkstra(string inFileName) {
    int n, m, x;
    ifstream input(inFileName);
    input >> n >> m >> x;
    vector<Node<int, int>*> vertices(n + 1);
    for (int j = 1; j <= n; j++) {
        auto* v = new Node<int, int>(j, INF);
        vertices[j] = v;
    }
    for (int i = 0; i < m; ++i) {
        int tail, head, length;
        input >> tail >> head >> length;
        auto* edge = new Edge<int, int>(vertices[head], length);
        vertices[tail]->AddNeighbours(edge);
    }
    input.close();
    return {vertices, x};
}


void Dijkstra() {
    for (int test = 1; test <= COUNT_TESTS; ++test) {
        clock_t start = clock();
        auto Graph = ScanGraphDijkstra("../Tests\\input\\in" + to_string(test) + ".txt");
        ofstream output("output.txt");
        int start_vertex = Graph.second;
        vector<Node<int, int>*> vertices = Graph.first;
        vertices[start_vertex]->state = LABELED;
        auto* heap = new FibonacciHeap<int, int>();
        vertices[start_vertex]->key = 0;
        heap->Insert(vertices[start_vertex]);

        while (!heap->IsEmpty()) {
            auto* v = heap->RemoveMinimum();
            v->state = SCANNED;
            for(int i = 0; i < v->neighbours.size(); i++) {
                auto* edge = v->neighbours[i];
                auto* u = edge->end;
                int NewKey = v->key + edge->length;
                if (u->state == UNLABELED) {
                    u->state = LABELED;
                    u->key = v->key + edge->length;
                    heap->Insert(u);
                }
                else if (u->key > NewKey) {
                    heap->DecreaseKey(NewKey, u);
                }
            }
        }
        clock_t stop = clock();

        for (int i = 1; i < vertices.size(); ++i) {
            if (vertices[i]->key == INT_MAX) {
                output << "-1 ";
                vertices[i]->key = -1;
            }
            else output << vertices[i]->key << " ";
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

        cout << "Test " << test << ": ";
        if (is_ok) {
            cout << "OK ";
            cout << "Time: "<< (double)(stop - start) / CLOCKS_PER_SEC << " seconds" << endl;
        }
        else cout << "ERROR" << endl;
    }
}


int main() {
    Dijkstra();
    return 0;
}