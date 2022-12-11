#include <vector>
#include <ctime>
#include <iostream>
#include "FibonacciHeap.cpp"
#define INF INT_MAX
typedef long long ll;

using namespace std;
const int COUNT_TESTS = 30;

vector<ll> EasyDijkstra(vector<vector<pair<ll, ll>>> adjacency_lists, ll start_vertex) {
    ll tops_count = adjacency_lists.size() - 1;
    vector<ll> distance(tops_count + 1, INF);
    vector<bool> used(tops_count + 1, false);
    distance[start_vertex] = 0;
    distance[0] = 0;
    used[0] = true;

    for (ll i = 1; i <= tops_count; ++i) {
        int v = 1,  min_length = INF;
        for (int j = 1; j <= tops_count; ++j) {
            if (!used[j] && distance[j] < min_length) {
                v = j;
                min_length = distance[j];
            }
        }
        if (distance[v] == INF)
            break;
        used[v] = true;
        for (auto neigh : adjacency_lists[v]) {
            if (distance[v] + neigh.second < distance[neigh.first])
                distance[neigh.first] = distance[v] + neigh.second;
        }
    }
    for (int i = 0; i < tops_count; ++i) {
        if (distance[i] == INF)
            distance[i] = -1;
    }
    return distance;
}


vector<ll> DijkstraFibHeap(vector<vector<pair<ll, ll>>> adjacency_lists, ll x) {
    ll tops_count = adjacency_lists.size() - 1;
    vector<Node<int, int>*> vertices(tops_count + 1);
    for (int i = 1; i <= tops_count; ++i) {
        vertices[i] = (new Node<int, int>(i, INF));
    }
    for (int i = 1; i <= tops_count; ++i) {
        for (int j = 0; j < adjacency_lists[i].size(); ++j) {
            auto* edge = new Edge<int, int>(vertices[adjacency_lists[i][j].first], adjacency_lists[i][j].second);
            vertices[i]->AddNeighbours(edge);
        }
    }

    vertices[x]->state = LABELED;
    auto* heap = new FibonacciHeap<int, int>();
    vertices[x]->key = 0;
    heap->Insert(vertices[x]);

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
    vector<ll> distance = vector<ll>(tops_count + 1);
    for (int i = 1; i < vertices.size(); ++i) {
        distance[i] = vertices[i]->key;
    }
    return distance;
}


int main() {

    for (int test = 1; test <= COUNT_TESTS; ++test) {
        ofstream output("output.txt");
        clock_t start = clock();
        ll tops_count, edges_count, x;
        ifstream input("../Tests/input/in" + to_string(test) + ".txt");

        input >> tops_count >> edges_count >> x;

        vector<vector<pair<ll, ll>>> adjacency_lists(tops_count + 1, vector<pair<ll, ll>>());
        int first, second, length;
        for (int i = 0; i < edges_count; ++i) {
            input >> first >> second >> length;
            adjacency_lists[first].push_back({second, length});
        }
        input.close();

        auto vertices = EasyDijkstra(adjacency_lists, x);
        clock_t stop = clock();

        for (int i = 1; i < vertices.size(); ++i) {
            if (vertices[i] == INT_MAX) {
                output << "-1 ";
                vertices[i] =  -1;
            }
            else output << vertices[i] << " ";
        }
        string file_name = "../Tests/answer/ans" + to_string(test) + ".txt";
        ifstream ans(file_name);
        vector<int> answers(vertices.size());
        for (int i = 1; i < answers.size(); ++i) {
            ans >> answers[i];
        }
        bool is_ok = true;
        for (int i = 1; i < answers.size(); ++i) {
            if (answers[i] != vertices[i]) {
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

    return 0;
}