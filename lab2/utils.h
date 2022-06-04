#ifndef OR_ALGORITHMS_UTILS_H
#define OR_ALGORITHMS_UTILS_H

#include <vector>

struct Item {
    size_t weight = 0;
    size_t price = 0;
    size_t amount = 0;
};

struct Response {
    std::vector<bool> result;
    size_t iter;
};

std::vector<size_t> descending_index(const std::vector<Item>& data) {
    std::vector<size_t> index;
    index.reserve(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        index.push_back(i);
    }

    std::sort(index.begin(), index.end(), [&data](size_t lhs, size_t rhs) {
        return static_cast<double>(data[lhs].price) / static_cast<double>(data[lhs].weight) >
        static_cast<double>(data[rhs].price) / static_cast<double>(data[rhs].weight);
    });

    return index;
}

#endif //OR_ALGORITHMS_UTILS_H
