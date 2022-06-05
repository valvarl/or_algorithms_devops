#ifndef OR_ALGORITHMS_LOCAL_SEARCH_H
#define OR_ALGORITHMS_LOCAL_SEARCH_H

#include <algorithm>
#include <random>
#include <chrono>
#include <climits>

using matrix = std::vector<std::vector<int>>;

uint64_t fitness(const std::vector<int> &permutation, const matrix &md, const matrix &mf) {
    uint64_t total_cost = 0;
    for (size_t i = 0; i < permutation.size(); ++i) {
        for (size_t j = 0; j < permutation.size(); ++j) {
            total_cost += mf[permutation[i]][permutation[j]] * md[i][j];
        }
    }
    return total_cost;
}

struct LSResponse {
    std::vector<uint64_t> cost_track;
    std::vector<int> solution;
};

std::vector<int> initialization(const int n) {
    std::vector<int> permutation(n);  // permutation[i] facility on i-th location
    for (int i = 0; i < n; ++i) {  // initialization
        permutation[i] = i;
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(permutation.begin(), permutation.end(), e);

    return permutation;
}

LSResponse ls(std::vector<int> &permutation, const matrix &md, const matrix &mf) {
    const int n = static_cast<int>(permutation.size());
    std::vector<uint64_t> cost_track;

    int first, second;
    uint64_t cur_fitness = std::numeric_limits<uint64_t>::max();
    while (true) {
        first = second = -1;
        for (int i = 0; i < n; ++i) {  // find the best neighboring permutation
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    std::swap(permutation[i], permutation[j]);
                    auto fit = fitness(permutation, md, mf);
                    if (fit < cur_fitness) {
                        cur_fitness = fit;
                        first = i;
                        second = j;
                    }
                    std::swap(permutation[i], permutation[j]);
                }
            }
        }

        if (first == -1) {  // no best neighboring permutation found
            break;
        }

        std::swap(permutation[first], permutation[second]);  // best swap
        cost_track.push_back(cur_fitness);  // save cost to track
    }

    return {cost_track, permutation};
}

LSResponse local_search(const int n, const matrix &md, const matrix &mf) {
    auto permutation = initialization(n);
    return ls(permutation, md, mf);
}

#endif //OR_ALGORITHMS_LOCAL_SEARCH_H
