#ifndef OR_ALGORITHMS_DP_H
#define OR_ALGORITHMS_DP_H

#include <vector>
#include <algorithm>
#include "utils.h"

Response dp_backpack(const std::vector<Item> &data, const size_t capacity) {
    size_t iter = 0;
    std::vector<std::vector<size_t>> K(data.size() + 1, std::vector<size_t>(capacity + 1));
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 1; j <= capacity; ++j) {
            if (j >= data[i].weight) {
                K[i + 1][j] = std::max(K[i][j], K[i][j - data[i].weight] + data[i].price);
            } else {
                K[i + 1][j] = K[i][j];
            }
            ++iter;
        }
    }

    std::vector<bool> result(data.size());
    auto res = K[data.size()][capacity], w = capacity;
    for (size_t i = data.size(); i > 0 && res > 0; i--) {
        if (res != K[i - 1][w]) {
            result[i - 1] = true;
            res -= data[i - 1].price;
            w -=  data[i - 1].weight;
        }
    }
    return {result, iter};
}

#endif //OR_ALGORITHMS_DP_H
