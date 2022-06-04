#ifndef OR_ALGORITHMS_FPTAS_H
#define OR_ALGORITHMS_FPTAS_H

#include <vector>
#include <cmath>
#include "utils.h"
#include "dp.h"

Response
fptas_backpack(std::vector<Item> data, const size_t capacity, const double epsilon) {
    size_t iter = 0;
    auto c_max = data[0].price;
    for (const Item& item: data) {
        if (item.price > c_max) {
            c_max = item.price;
        }
    }
    double t = std::floor(std::log2((epsilon * static_cast<double>(c_max)) /
            ((1 + epsilon) * static_cast<double>(data.size()))));

    for (Item& item: data) {
        iter++;
        item.price = std::floor(static_cast<double>(item.price) * std::pow(2, -t));
    }

    Response result = dp_backpack(data, capacity);
    result.iter += iter;
    return result;
}

#endif //OR_ALGORITHMS_FPTAS_H
