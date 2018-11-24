#include <algorithm>
#include <chrono>
#include <cstdio>
#include <utility>
#include <vector>

#include "DynamicGraph/DynamicGraph.hpp"
#include "NaiveDynamicGraph/NaiveDynamicGraph.hpp"

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::max;
using std::vector;
using std::pair;
using usp::DynamicGraph;
using usp::NaiveDynamicGraph;

const int kIterations = 10000;

const bool print_step = false;
const bool print_total = true;

int query_count = 0;
int insert_count = 0;
int remove_count = 0;

microseconds naive_query_time_in_us;
microseconds hdt_query_time_in_us;

microseconds naive_insert_time_in_us;
microseconds hdt_insert_time_in_us;

microseconds naive_remove_time_in_us;
microseconds hdt_remove_time_in_us;

void do_query(NaiveDynamicGraph &naive, DynamicGraph &dg) {
    query_count++;
    int u = rand() % naive.size();
    int v = rand() % naive.size();

    auto start = high_resolution_clock::now();
    dg.is_connected(u, v);
    auto end = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(end - start);
    if (print_step) printf("hdt,query,%lld\n", dur.count());
    hdt_query_time_in_us += dur;

    start = high_resolution_clock::now();
    // naive.is_connected(u, v);
    end = high_resolution_clock::now();
    dur = duration_cast<microseconds>(end - start);
    if (print_step) printf("naive,query,%lld\n", dur.count());
    naive_query_time_in_us += dur;
}

void do_insert(int u, int v, NaiveDynamicGraph &naive, DynamicGraph &dg) {
    insert_count++;

    auto start = high_resolution_clock::now();
    dg.insert(u, v);
    auto end = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(end - start);
    if (print_step) printf("hdt,insert,%lld\n", dur.count());
    hdt_insert_time_in_us += dur;

    start = high_resolution_clock::now();
    naive.insert(u, v);
    end = high_resolution_clock::now();
    dur = duration_cast<microseconds>(end - start);
    if (print_step) printf("naive,insert,%lld\n", dur.count());
    naive_insert_time_in_us += dur;
}

void do_remove(int u, int v, NaiveDynamicGraph &naive, DynamicGraph &dg) {
    remove_count++;

    auto start = high_resolution_clock::now();
    dg.remove(u, v);
    auto end = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(end - start);
    if (print_step) printf("hdt,remove,%lld\n", dur.count());
    hdt_remove_time_in_us += dur;

    start = high_resolution_clock::now();
    naive.remove(u, v);
    end = high_resolution_clock::now();
    dur = duration_cast<microseconds>(end - start);
    if (print_step) printf("naive,remove,%lld\n", dur.count());
    naive_remove_time_in_us += dur;
}

int main() {
    srand(7);
    for (int n = 10; n*n < 2e6; n += 10) {
        insert_count = remove_count = query_count = 0;
        hdt_insert_time_in_us = microseconds::zero();
        hdt_remove_time_in_us = microseconds::zero();
        hdt_query_time_in_us = microseconds::zero();

        NaiveDynamicGraph original = NaiveDynamicGraph::grid(n, n);
        NaiveDynamicGraph naive(n * n);
        DynamicGraph dg(n * n);

        vector<pair<int, int>> edges = original.edges();
        for (const auto &edge : edges) {
            bool should_insert = rand() % 2;
            if (should_insert) {
                naive.insert(edge.first, edge.second);
                dg.insert(edge.first, edge.second);
            }
        }

        for (int iter = 1; iter <= kIterations; iter++) {
            int idx = rand() % edges.size();
            int u = edges[idx].first;
            int v = edges[idx].second;
            if (naive.has_edge(u, v)) {
                do_remove(u, v, naive, dg);
            } else {
                do_insert(u, v, naive, dg);
            }
            bool query = rand() % 2;
            if (query) {
                do_query(naive, dg);
            }
        }

        if (print_total) {
            // n, insert tempo, insert qt, remove tempo, remove qt, query tempo, query qt
            printf("%d", n*n);
            printf(",%lld,%d", hdt_insert_time_in_us.count(), insert_count);
            // printf("naive insert %lld\n", naive_insert_time_in_us.count());
            printf(",%lld,%d", hdt_remove_time_in_us.count(), remove_count);
            // printf("naive remove %lld\n", naive_remove_time_in_us.count());
            printf(",%lld,%d", hdt_query_time_in_us.count(), query_count);
            // printf("naive query %lld\n", naive_query_time_in_us.count());
            puts("");
            // printf("avg query %lf\n", (double) hdt_query_time_in_us.count() / query_count);
        }
    }
}
