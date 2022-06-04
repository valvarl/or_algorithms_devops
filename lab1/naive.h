#ifndef OR_ALGORITHMS_NAIVE_H
#define OR_ALGORITHMS_NAIVE_H

#include <string_view>
#include "utils.h"

Response naive(std::string_view text, std::string_view word) {
    int result = -1, iter = 0;
    for (size_t i = 0; i < text.size() - word.size() + 1; ++i) {
        if (cmpstr(text.substr(i, word.size()), word, &iter)) {
            result = static_cast<int>(i);
            break;
        }
    }
    return {result, iter};
}

#endif //OR_ALGORITHMS_NAIVE_H
