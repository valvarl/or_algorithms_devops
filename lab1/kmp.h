#ifndef OR_ALGORITHMS_KMP_H
#define OR_ALGORITHMS_KMP_H

#include <vector>
#include <string_view>
#include "utils.h"

std::vector<size_t> complete_prefix_function(std::string_view pattern) {
    auto pattern_length = pattern.size();
    std::vector<size_t> prefixes(pattern_length, 0);
    size_t kk = 0;
    for (size_t q = 1; q < pattern_length; ++q) {
        while (kk > 0 && pattern[kk] != pattern[q]) {
            kk = prefixes[kk - 1];
        }
        if (pattern[kk] == pattern[q]) {
            kk++;
        }
        prefixes[q] = kk;
    }
    return prefixes;
}

Response kmp_matcher(std::string_view text, std::string_view word) {
    int result = -1, iter = 0;
    auto prefix = complete_prefix_function(word);
    size_t matched_pos = 0;
    for (size_t current = 0; current < text.length(); ++current) {
        while (matched_pos > 0 && inc(&iter) &&
               word[matched_pos] != text[current]) {
            matched_pos = prefix[matched_pos - 1];
        }

        if (inc(&iter) && word[matched_pos] == text[current]) {
            matched_pos++;
        }

        if (matched_pos == (word.length())) {
            result = static_cast<int>(current - (word.length() - 1));
            break;
            // matched_pos = prefix[matched_pos - 1];
        }
    }
    return {result, iter};
}

#endif //OR_ALGORITHMS_KMP_H
