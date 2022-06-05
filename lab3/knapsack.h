#ifndef OR_ALGORITHMS_KNAPSACK_H
#define OR_ALGORITHMS_KNAPSACK_H

#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <vector>
#include <fstream>
#include <climits>

#include "utils.h"

namespace rnd {
    std::random_device rd;
    std::mt19937 mt{rd()};
    std::uniform_real_distribution<double> dist{0, 1};

    double rand() {
        return dist(mt);
    }
}

struct Chromo {
    explicit Chromo(int dimc) { items = new bool[dimc]; }

    ~Chromo() { items = nullptr; }

    void mutate(const int dimc, const int count) const {
        int mi;
        for (int i = 0; i < count; i++) {
            mi = static_cast<int>(round(rnd::rand() * (dimc - 1)));
            items[mi] = !items[mi];
        }
    }

    bool *items;
    int fv = 0, fw = 0;
};

std::pair<int, int>
fitness(bool *&x, const int dimc, const std::vector<Item> &data, const size_t capacity) {
    uint64_t fit = 0, wsum = 0;
    for (int i = 0; i < dimc; i++) {
        wsum += x[i] * data[i].weight;
        fit += x[i] * data[i].price;
    }
    if (wsum > capacity) {
        fit = 0;
    }
    return {static_cast<int>(fit), static_cast<int>(wsum)};
}

void crossover1p(const Chromo &c1, const Chromo &c2, const Chromo &c3, const int dimc, const int cp) {
    for (int i = 0; i < dimc; i++) {
        c3.items[i] = (i < cp ? c1.items[i] : c2.items[i]);
    }
}

bool cfit(const Chromo &c1, const Chromo &c2) {
    return c1.fv > c2.fv;
}

bool cmpfun(const std::pair<int, double> &r1, const std::pair<int, double> &r2) {
    return r1.second > r2.second;
}

int coin(const double crp) {  // a cointoss
    return (rnd::rand() < crp ? 1 : 0);  // crossover or mutation
}

// initializes the chromosomes with the results of a greedy algorithm
void initpopg(bool **&c, const std::vector<Item> &data,
              const int dimw, const size_t capacity, const int pop) {
    std::vector<std::pair<int, double>> rvals(dimw);
    std::vector<int> index(dimw, 0);
    for (int i = 0; i < dimw; i++) {
        rvals.push_back(std::pair<int, double>(
                std::make_pair(i, static_cast<double>(data[i].price) / static_cast<double>(data[i].weight))
                ));
    }
    std::sort(rvals.begin(), rvals.end(), cmpfun);
    size_t cur_weight = 0, k;
    for (int i = 0; i < dimw; i++) {
        k = rvals[i].first;
        if (cur_weight + data[k].weight <= capacity) { // greedy fill
            cur_weight += data[k].weight;
            index[k] = 1;
        }
    }
    for (int i = 0; i < pop; i++) {
        for (int j = 0; j < dimw; j++) {
            c[i][j] = index[j];
        }
    }
}

struct KnapsackResponse {
    std::vector<int> value_track, weight_track;
    std::vector<bool> result;
    double time;
};

const int pop = 50; // chromosome population size
const int gens = 10; // maximum number of generations

KnapsackResponse knapsack(const std::vector<Item> &data, const size_t capacity) {
    const int disc = static_cast<int>(ceil(pop * 0.8)); // chromosomes discarded via elitism
    const int dimw = static_cast<int>(data.size());
    int best = 0, ind = 0, ind2 = 0; // a few helpers for the main()
    int parc = 0; // parent index for crossover
    double crp = 0.35; // crossover probability
    std::vector<Chromo> ch(pop, Chromo(dimw));
    bool **c = new bool *[pop];
    for (int i = 0; i < pop; i++) {
        c[i] = new bool[dimw];
    }

    clock_t start = clock();
    initpopg(c, data, dimw, capacity, pop);  // greedy initializing

    std::pair<int, int> fitness_vw;
    for (int i = 0; i < pop; i++) {
        ch[i].items = c[i];
        fitness_vw = fitness(ch[i].items, dimw, data, capacity);
        ch[i].fv = fitness_vw.first;
        ch[i].fw = fitness_vw.second;
    }

    std::vector<int> value_track, weight_track;
    for (int p = 0; p < gens; p++) {
        best = parc = 0;
        std::sort(ch.begin(), ch.end(), cfit);
        for (int i = 0; i < pop; i++) {
            if (i > pop - disc) { // elitism - only processes the discarded chromosomes
                if (coin(crp) == 1) { // crossover section
                    // choosing parents for crossover
                    ind = parc + static_cast<int>(round(10 * rnd::rand()));
                    ind2 = parc + 1 + static_cast<int>(round(10 * rnd::rand()));
                    crossover1p(ch[ind % pop], ch[ind2 % pop], ch[i], dimw,
                                static_cast<int>(round(rnd::rand() * (dimw - 1))));
                    fitness_vw = fitness(ch[i].items, dimw, data, capacity);
                    ch[i].fv = fitness_vw.first;
                    ch[i].fw = fitness_vw.second;
                    parc += 1;
                } else { // mutation section
                    ch[i].mutate(dimw, 1);
                    fitness_vw = fitness(ch[i].items, dimw, data, capacity);
                    ch[i].fv = fitness_vw.first;
                    ch[i].fw = fitness_vw.second;
                }
            }
            if (ch[i].fv > ch[best].fv) {
                best = i;
            }
        }
        value_track.push_back(ch[best].fv);
        weight_track.push_back(ch[best].fw);
    }

    std::vector<bool> result;
    result.reserve(dimw);
    for (int i = 0; i < dimw; ++i) {
        result.push_back(ch[best].items[i]);
    }
    clock_t end = clock();
    return {value_track, weight_track, result, static_cast<double>(end - start) / CLOCKS_PER_SEC};
}

#endif //OR_ALGORITHMS_KNAPSACK_H
