#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <gtest/gtest.h>

#include "../common/format.h"
#include "../common/prettyprint.hpp"
#include "local_search.h"
#include "iterated_local_search.h"

struct Bench {
    std::string name;
    int n;
    std::vector<std::vector<int>> md{}, mf{};
};

std::vector<Bench> load_benches() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const std::string path = "lab4/benchmarks/";
    const std::vector<int> bench_names = {20, 40, 60, 80, 100};

    std::vector<Bench> benches;
    for (int i : bench_names) {
        std::string name = string_format("tai%da", i);
        std::string f = string_format("%s%s", path.c_str(), name.c_str());
        std::ifstream inf(f);

        int n;
        inf >> n;

        benches.push_back({name, n});
        benches.back().md.assign(n, std::vector<int>(n));
        benches.back().mf.assign(n, std::vector<int>(n));

        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                inf >> benches.back().md[j][k];
            }
        }

        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                inf >> benches.back().mf[j][k];
            }
        }

        inf.close();
    }

    return benches;
}

const int kRepeats = 5;
auto benches = std::make_shared<std::vector<Bench>>(load_benches());

TEST(QAP, LocalSearch) {
    const std::string test_results_path = "lab4/test_results/ls/";
    std::vector<std::vector<std::vector<uint64_t>>> cost_track;

    for (auto & k : *benches) {
        cost_track.emplace_back();
        double avg_time = 0;
        std::vector<int> best_solution;
        uint64_t best_cost = std::numeric_limits<uint64_t>::max();
        for (int r = 0; r < kRepeats; ++r) {
            const clock_t begin_time = clock(); // gets time
            auto result = local_search(k.n, k.md, k.mf);
            avg_time += static_cast<double>(clock () - begin_time) / CLOCKS_PER_SEC;
            cost_track.back().push_back(result.cost_track);
            if (result.cost_track.back() < best_cost) {
                best_cost = result.cost_track.back();
                best_solution = result.solution;
            }
        }

        avg_time /= kRepeats;

        std::ofstream f(test_results_path + "results.txt", std::ios::binary|std::ios::app);
        f << k.name << ',' << best_cost << ',' << avg_time << "\n";
        std::cerr << k.name << '\t' << best_cost << '\t' << avg_time << "\n";
        f.close();

        std::string name = string_format("%s.sol", k.name.c_str());
        std::ofstream sol(test_results_path + name, std::ios::binary|std::ios::app);
        std::for_each(best_solution.begin(), best_solution.end(), [&](int i){ sol << i + 1 << " "; });
        sol.close();
    }

    std::ofstream f(test_results_path + "costs.txt", std::ios::binary|std::ios::app);
    f << cost_track;
    f.close();
}

TEST(QAP, IteratedLocalSearch) {
    const std::string test_results_path = "lab4/test_results/ils/";
    std::vector<std::vector<std::vector<uint64_t>>> cost_track;

    for (auto & k : *benches) {
        cost_track.emplace_back();
        double avg_time = 0;
        std::vector<int> best_solution;
        uint64_t best_cost = std::numeric_limits<uint64_t>::max();
        for (int r = 0; r < kRepeats; ++r) {
            const clock_t begin_time = clock(); // gets time
            auto result = iterated_local_search(k.n, k.md, k.mf, 5, 5);
            avg_time += static_cast<double>(clock () - begin_time) / CLOCKS_PER_SEC;
            cost_track.back().push_back(result.cost_track);
            if (result.cost_track.back() < best_cost) {
                best_cost = result.cost_track.back();
                best_solution = result.solution;
            }
        }

        avg_time /= kRepeats;

        std::ofstream f(test_results_path + "results.txt", std::ios::binary|std::ios::app);
        f << k.name << ',' << best_cost << ',' << avg_time << "\n";
        std::cerr << k.name << '\t' << best_cost << '\t' << avg_time << "\n";
        f.close();

        std::string name = string_format("%s.sol", k.name.c_str());
        std::ofstream sol(test_results_path + name, std::ios::binary|std::ios::app);
        std::for_each(best_solution.begin(), best_solution.end(), [&](int i){ sol << i + 1 << " "; });
        sol.close();
    }

    std::ofstream f(test_results_path + "costs.txt", std::ios::binary|std::ios::app);
    f << cost_track;
    f.close();
}
