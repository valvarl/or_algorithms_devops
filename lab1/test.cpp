
#include <iostream>
#include <utility>
#include <vector>
#include <ctime>
#include <fstream>
#include <gtest/gtest.h>

#include "utils.h"
#include "naive.h"
#include "rabin_karp.h"
#include "kmp.h"

std::pair<std::string, std::string> get_name(const std::string &text_path, const std::string &word_path) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream inf_text("lab1/benchmarks/" + text_path);
    std::string text((std::istreambuf_iterator<char>(inf_text)),
                    std::istreambuf_iterator<char>());

    std::ifstream inf_word("lab1/benchmarks/" + word_path);
    std::string word((std::istreambuf_iterator<char>(inf_word)),
                     std::istreambuf_iterator<char>());

    return std::make_pair(text, word);
}

std::vector<std::string> fp{"bad", "good"};

std::pair<std::vector<std::string>, std::vector<std::string>> load_benches() {
    std::vector<std::string> texts, words;
    for (const std::string &s: fp) {
        for (int i = 1; i <= 4; ++i) {
            auto text_path = s + "_t_" + std::to_string(i) + ".txt";
            auto word_path = s + "_w_" + std::to_string(i) + ".txt";
            auto pair = get_name(text_path, word_path);
            texts.push_back(pair.first);
            words.push_back(pair.second);
        }
    }
    return std::make_pair(texts, words);
}

auto benches = load_benches();
const auto texts = benches.first;
const auto words = benches.second;

std::vector<int> compare_results{8, 90, 900, 4000, 1057, 971, 2941, 17069};

const int kRepeats = 1000;

TEST(ExactPatternMatching, Naive) {
    std::ofstream f("lab1/results.txt", std::ios::binary|std::ios::app);
    for (size_t i = 0; i < texts.size(); ++i) {
        Response response;
        double duration;
        {
            std::clock_t m_started = clock();
            for (int j = 0; j < kRepeats; ++j) {
                response = naive(texts[i], words[i]);
            }
            duration = static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;
        }
        ASSERT_EQ(response.result, compare_results[i]);
        f << fp[i < 4 ? 0 : 1] + "_" + std::to_string(i % 4 + 1) << ",Naive,"
        << std::to_string(duration) << "," << std::to_string(response.iter) << "\n";
    }
}

TEST(ExactPatternMatching, RabinKarp) {
    std::ofstream f("lab1/results.txt", std::ios::binary|std::ios::app);
    for (size_t i = 0; i < texts.size(); ++i) {
        Response response;
        double duration;
        {
            std::clock_t m_started = clock();
            for (int j = 0; j < kRepeats; ++j) {
                response = rabin_karp(texts[i], words[i]);
            }
            duration = static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;
        }
        ASSERT_EQ(response.result, compare_results[i]);
        f << fp[i < 4 ? 0 : 1] + "_" + std::to_string(i % 4 + 1) << ",Rabin-Karp,"
        << std::to_string(duration) << "," << std::to_string(response.iter) << "\n";
    }
}

TEST(ExactPatternMatching, KMP) {
    std::ofstream f("lab1/results.txt", std::ios::binary|std::ios::app);
    for (size_t i = 0; i < texts.size(); ++i) {
        Response response;
        double duration;
        {
            std::clock_t m_started = clock();
            for (int j = 0; j < kRepeats; ++j) {
                response = kmp_matcher(texts[i], words[i]);
            }
            duration = static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;
        }
        ASSERT_EQ(response.result, compare_results[i]);
        f << fp[i < 4 ? 0 : 1] + "_" + std::to_string(i % 4 + 1) << ",Knuth–Morris–Pratt,"
        << std::to_string(duration) << "," << std::to_string(response.iter) << "\n";
    }
}
