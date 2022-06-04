#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <gtest/gtest.h>

#include "../common/format.h"
#include "../common/prettyprint.hpp"
#include "utils.h"
#include "dp.h"
#include "2approx.h"
#include "fptas.h"
#include "bnb.h"

using bench = std::tuple<int, std::vector<Item>, std::vector<size_t>>;

auto load_benches(size_t n) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const std::string path = "lab2/benchmarks/";
    std::vector<bench> result;

    for (size_t i = 1; i <= n; ++i) {
        std::string scapacity = string_format("%sp%02d_c.txt", path.c_str(), i);
        std::ifstream inf_capacity(scapacity);

        int capacity;
        inf_capacity >> capacity;

        std::string sweight = string_format("%sp%02d_w.txt", path.c_str(), i);
        std::string sprofit = string_format("%sp%02d_p.txt", path.c_str(), i);
        std::string sselection = string_format("%sp%02d_s.txt", path.c_str(), i);

        std::ifstream inf_weight(sweight);
        std::ifstream inf_profit(sprofit);
        std::ifstream inf_selection(sselection);

        size_t weight, profit, selection;
        std::vector<Item> items;
        std::vector<size_t> selects;

        while (inf_weight >> weight && inf_profit >> profit && inf_selection >> selection) {
            items.push_back({weight, profit, 1});
            selects.push_back(selection);
        }

        inf_weight.close();
        inf_profit.close();
        inf_selection.close();

        result.emplace_back(capacity, items, selects);
    }

    return result;
}

template <class T>
std::tuple<size_t, size_t> get_backpack(const std::vector<Item> &data, const std::vector<T> &vector) {
    size_t price = 0, weight = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        if (vector[i]) {
            weight += data[i].weight;
            price += data[i].price;
        }
    }

    return {weight, price};
}

void write_test_result(size_t bench_num, const std::string& algorithm, size_t price, size_t weight, double duration, size_t iter) {
    std::ofstream f("lab2/results.txt", std::ios::binary|std::ios::app);
    f << string_format("p%02d", bench_num) << ',' << algorithm << ',' << price << ',' << weight << ',' << duration << ',' << iter << "\n";
    f.close();
}

const int kBenches = 7;
const int kRepeats = 100'000;
std::shared_ptr<std::vector<bench>> benches = std::make_shared<std::vector<bench>>(load_benches(kBenches));

TEST(KnapsackProblem, DP) {
    for (int i = 0; i < kBenches; ++i) {
        std::clock_t m_started = clock();
        for (int j = 0; j < kRepeats; ++j) {
            dp_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)));
        }
        double duration = static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;

        Response dp_solution = dp_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)));
        auto solution = dp_solution.result;
        auto iter = dp_solution.iter;

        auto backpack_info = get_backpack(std::get<1>(benches->at(i)), solution);
        auto weight = std::get<0>(backpack_info), price = std::get<1>(backpack_info);

        auto act_backpack_info = get_backpack(std::get<1>(benches->at(i)), std::get<2>(benches->at(i)));
        auto act_weight = std::get<0>(backpack_info), act_price = std::get<1>(act_backpack_info);

        ASSERT_EQ(price, act_price);
        ASSERT_EQ(weight, act_weight);

        write_test_result(i + 1, "DP", price, weight, duration, iter);
    }
}

TEST(KnapsackProblem, TwoApproximation) {
    for (int i = 0; i < kBenches; ++i) {
        std::clock_t m_started = clock();
        for (int j = 0; j < kRepeats; ++j) {
            two_approx_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)));
        }
        double duration = static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;

        Response ta_solution = two_approx_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)));
        auto solution = ta_solution.result;
        auto iter = ta_solution.iter;

        auto backpack_info = get_backpack(std::get<1>(benches->at(i)), solution);
        auto weight = std::get<0>(backpack_info), price = std::get<1>(backpack_info);

        auto act_backpack_info = get_backpack(std::get<1>(benches->at(i)), std::get<2>(benches->at(i)));
        auto act_weight = std::get<0>(backpack_info), act_price = std::get<1>(act_backpack_info);

        EXPECT_EQ(price, act_price);
        EXPECT_EQ(weight, act_weight);

        write_test_result(i + 1, "2-Approx", price, weight, duration, iter);
    }
}

TEST(KnapsackProblem, FPTAS) {
    for (int i = 0; i < kBenches; ++i) {
        std::clock_t m_started = clock();
        for (int j = 0; j < kRepeats; ++j) {
            fptas_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)), 0.5);
        }
        double duration = static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;

        Response fptas_solution = fptas_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)), 0.5);
        auto solution = fptas_solution.result;
        auto iter = fptas_solution.iter;

        auto backpack_info = get_backpack(std::get<1>(benches->at(i)), solution);
        auto weight = std::get<0>(backpack_info), price = std::get<1>(backpack_info);

        auto act_backpack_info = get_backpack(std::get<1>(benches->at(i)), std::get<2>(benches->at(i)));
        auto act_weight = std::get<0>(backpack_info), act_price = std::get<1>(act_backpack_info);

        EXPECT_EQ(price, act_price);
        EXPECT_EQ(weight, act_weight);

        write_test_result(i + 1, "FPTAS", price, weight, duration, iter);
    }
}

TEST(KnapsackProblem, BranchAndBound) {
    for (int i = 0; i < kBenches; ++i) {
        std::clock_t m_started = clock();
        for (int j = 0; j < kRepeats; ++j) {
            bnb_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)));
        }
        double duration = static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;

        Response bnb_solution = bnb_backpack(std::get<1>(benches->at(i)), std::get<0>(benches->at(i)));
        auto solution = bnb_solution.result;
        auto iter = bnb_solution.iter;

        auto backpack_info = get_backpack(std::get<1>(benches->at(i)), solution);
        auto weight = std::get<0>(backpack_info), price = std::get<1>(backpack_info);

        auto act_backpack_info = get_backpack(std::get<1>(benches->at(i)), std::get<2>(benches->at(i)));
        auto act_weight = std::get<0>(backpack_info), act_price = std::get<1>(act_backpack_info);

        ASSERT_EQ(price, act_price);
        ASSERT_EQ(weight, act_weight);

        write_test_result(i + 1, "B&B", price, weight, duration, iter);
    }
}
