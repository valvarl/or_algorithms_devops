#ifndef LAB1_NAIVE_H_
#define LAB1_NAIVE_H_

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

#endif  // LAB1_NAIVE_H_
