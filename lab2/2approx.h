#ifndef OR_ALGORITHMS_2APPROX_H
#define OR_ALGORITHMS_2APPROX_H

#include <vector>
#include "utils.h"

Response
two_approx_backpack(const std::vector<Item> &data, const size_t capacity) {
    size_t iter = 0;
    auto index = descending_index(data);

    size_t i = 0, current_weight = 0;
    std::vector<bool> result(data.size());
    while (i < data.size() && current_weight + index[i] <= capacity) {
        result[index[i]] = true;
        current_weight += data[index[i++]].weight;
        ++iter;
    }

    if (i == data.size() || current_weight == capacity) {
        return {result, iter};
    }

    if (i > 0) {
        i--;
        result[index[i]] = false;
        current_weight -= data[index[i]].weight;
    }

    size_t remain_capacity = capacity - current_weight, max_index = index.size();
    for (; i < index.size(); ++i) {
        ++iter;
        if (data[index[i]].weight <= remain_capacity) {
            if (max_index == index.size() || (max_index != index.size() && (
                    static_cast<double>(data[index[i]].price) / static_cast<double>(data[index[i]].weight) >
                    static_cast<double>(data[index[max_index]].price) / static_cast<double>(data[index[max_index]].weight)
                    ))) {
                max_index = i;
            }
        }
    }
    result[index[max_index]] = true;
    return {result, iter};
}

#endif //OR_ALGORITHMS_2APPROX_H
