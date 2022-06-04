#ifndef OR_ALGORITHMS_RABIN_KARP_H
#define OR_ALGORITHMS_RABIN_KARP_H

#include <string_view>
#include "utils.h"

Response
rabin_karp(std::string text, std::string word, int d = 256, int q = 1283) {
    int result = -1, iter = 0;

    int h = 1;

    for (size_t i = 0; i < word.size() - 1; ++i) {
        h = (h * d) % q;
    }

    int hash_word = 0, hash_text = 0;

    for (size_t i = 0; i < word.size(); ++i) {
        hash_word = (d * hash_word + word[i]) % q;
        if (hash_word < 0) {
            hash_word += q;
        }

        hash_text = (d * hash_text + text[i]) % q;
        if (hash_text < 0) {
            hash_text += q;
        }
    }

    for (size_t i = 0; i < text.size() - word.size() + 1; ++i) {
        if (hash_word == hash_text) {
            if (cmpstr(text.substr(i, word.size()), word, &iter)) {
                result = static_cast<int>(i);
                break;
            }
        }

        if (i < text.size() - word.size()) {
            hash_text =
                    (d * (hash_text - text[i] * h) + text[i + word.size()]) % q;

            if (hash_text < 0) {
                hash_text = (hash_text + q);
            }
        }
    }
    return {result, iter};
}

#endif //OR_ALGORITHMS_RABIN_KARP_H
