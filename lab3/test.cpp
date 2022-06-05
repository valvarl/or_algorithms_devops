#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <gtest/gtest.h>

#include "../common/format.h"
#include "../common/prettyprint.hpp"
#include "knapsack.h"
#include "tsp.cpp"
#include "utils.h"

using bench = std::tuple<int, std::vector<Item>, std::vector<size_t>>;

auto load_knapsack_benches(size_t n) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const std::string path = "lab3/benchmarks/knapsack/";
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

const int kKnapsackBenches = 7;
std::shared_ptr<std::vector<bench>> knapsack_benches =
        std::make_shared<std::vector<bench>>(load_knapsack_benches(kKnapsackBenches));

TEST(GeneticAlgorithm, KnapsackProblem) {
    const std::string test_results_path = "lab3/test_results/knapsack/";
    std::vector<std::vector<int>> value_track, weight_track;
    for (int i = 0; i < kKnapsackBenches; ++i) {
        KnapsackResponse kr = knapsack(std::get<1>(knapsack_benches->at(i)), std::get<0>(knapsack_benches->at(i)));

        auto backpack_info = get_backpack(std::get<1>(knapsack_benches->at(i)), kr.result);
        auto weight = std::get<0>(backpack_info), price = std::get<1>(backpack_info);

        auto act_backpack_info = get_backpack(std::get<1>(knapsack_benches->at(i)), std::get<2>(knapsack_benches->at(i)));
        auto act_weight = std::get<0>(backpack_info), act_price = std::get<1>(act_backpack_info);

        EXPECT_EQ(price, act_price);
        EXPECT_EQ(weight, act_weight);

        std::ofstream f(test_results_path + "results.txt", std::ios::binary|std::ios::app);
        f << string_format("p%02d", i + 1) << ',' << price << ',' << weight << ',' << kr.time << "\n";
        f.close();

        value_track.push_back(kr.value_track);
        weight_track.push_back(kr.weight_track);
    }

    std::ofstream kv(test_results_path + "values.txt", std::ios::binary|std::ios::app);
    kv << value_track;
    kv.close();

    std::ofstream kw(test_results_path + "weights.txt", std::ios::binary|std::ios::app);
    kw << weight_track;
    kw.close();
}

struct Point {
    double x, y;
};

using adjacency_matrix = std::vector<std::vector<double>>;

auto load_tsp_benches() {
    const std::string path = "lab3/benchmarks/tsp/";
    std::vector<std::string> coord_bench_names = {"a280", "att48", "ch150", "fl417"};

    std::vector<std::vector<Point>> coord_benches;
    for (auto &s : coord_bench_names) {
        std::string f = string_format("%s%s.tsp", path.c_str(), s.c_str());
        std::ifstream inf(f);

        int idx;
        double x, y;

        coord_benches.emplace_back();
        while (inf >> idx && inf >> x && inf >> y) {
            coord_benches.back().push_back({x, y});
        }

        inf.close();
    }

    std::vector<adjacency_matrix> am;
    for (size_t k = 0; k < coord_bench_names.size(); ++k) {
        am.emplace_back(coord_benches[k].size(), std::vector<double>(coord_benches[k].size()));
        for (size_t i = 0; i < coord_benches[k].size(); ++i) {
            for (size_t j = i + 1; j < coord_benches[k].size(); ++j) {
                am.back()[j][i] = std::sqrt(std::pow(coord_benches[k][j].x - coord_benches[k][i].x, 2) +
                        std::pow(coord_benches[k][j].y - coord_benches[k][i].y, 2));
            }
        }
    }

    std::ifstream inf_bays29(path + "bays29.tsp");
    am.emplace_back(29, std::vector<double>(29));
    for (int i = 0; i < 29; ++i) {
        for (int j = 0; j < 29; ++j) {
            inf_bays29 >> am.back()[i][j];
        }
    }
    inf_bays29.close();

    std::ifstream inf_gr17(path + "gr17.tsp");
    am.emplace_back(17, std::vector<double>(17));
    for (int i = 0; i < 17; ++i) {
        for (int j = 0; j <= i; ++j) {
            inf_gr17 >> am.back()[i][j];
        }
    }
    inf_gr17.close();

    return am;
}

const int kTSPBenches = 6;
std::vector<std::string> bench_names = {"a280", "att48", "ch150", "fl417", "bays29", "gr17"};
auto tsp_benches = std::make_shared<std::vector<adjacency_matrix>>(load_tsp_benches());

TEST(GeneticAlgorithm, TSP) {
    srand(time(NULL)); // random numbers
    const std::string test_results_path = "lab3/test_results/tsp/";
    std::vector<std::vector<double>> cost_track;

    std::vector<Graph *> graphs(kTSPBenches);
    for (int k = 0; k < kTSPBenches; ++k) {
        graphs[k] = new Graph(tsp_benches->at(k).size(), 0);
        for (size_t i = 0; i < tsp_benches->at(k).size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                graphs[k]->addEdge(static_cast<int>(i), static_cast<int>(j), tsp_benches->at(k)[i][j]);
                graphs[k]->addEdge(static_cast<int>(j), static_cast<int>(i), tsp_benches->at(k)[i][j]);
            }
        }

        Genetic genetic(graphs[k], 50, 10000, 10, false);

        const clock_t begin_time = clock(); // gets time
        auto result = genetic.run(); // runs the genetic algorithm
        auto time = static_cast<double>(clock () - begin_time) / CLOCKS_PER_SEC;

        std::ofstream f(test_results_path + "results.txt", std::ios::binary|std::ios::app);
        f << bench_names[k] << ',' << result.best_cost << ',' << time << "\n";
        f.close();

        std::cout << bench_names[k] << "\t" << result.best_cost << "\t" << result.result << "\n";
        cost_track.push_back(result.cost_track);
    }

    std::ofstream f(test_results_path + "costs.txt", std::ios::binary|std::ios::app);
    f << cost_track;
    f.close();
}
