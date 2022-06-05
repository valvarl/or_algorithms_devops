#ifndef OR_ALGORITHMS_ITERATED_LOCAL_SEARCH_H
#define OR_ALGORITHMS_ITERATED_LOCAL_SEARCH_H

#include <random>

#include "local_search.h"

std::vector<int> stochastic_k_opt(const std::vector<int> &permutation, const int k = 5) {
    const int n = static_cast<int>(permutation.size());

    std::vector<int> numbers;
    numbers.reserve(n);
    for(int i = 0; i < n; ++i) {
        numbers.push_back(i);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
    std::sort(numbers.begin(), std::next(numbers.begin(), 2 * k));

    auto new_perm = permutation;
    for(int i = 0; i < 2 * k; i += 2) {
        std::reverse(new_perm.begin() + numbers[i], new_perm.begin() + numbers[i + 1] + 1);
    }

    return new_perm;
}

LSResponse iterated_local_search(const int n, const matrix &md, const matrix &mf,
                                 const int iter = 5, const int k = 5) {
    auto permutation = initialization(n);
    auto ls_response = ls(permutation, md, mf);

    std::vector<uint64_t> cost_track;
    cost_track.insert(cost_track.end(), ls_response.cost_track.begin(), ls_response.cost_track.end());
    uint64_t best_cost = ls_response.cost_track.back();

    for (int i = 0; i < iter; ++i) {
        auto new_perm = stochastic_k_opt(permutation, k);
        ls_response = ls(new_perm, md, mf);
        if (ls_response.cost_track.back() < best_cost) {
            cost_track.insert(cost_track.end(), ls_response.cost_track.begin(), ls_response.cost_track.end());
            best_cost = ls_response.cost_track.back();
            permutation = std::move(new_perm);
        }
    }

    return {cost_track, permutation};
}

#endif //OR_ALGORITHMS_ITERATED_LOCAL_SEARCH_H
