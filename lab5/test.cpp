#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <sstream>
#include <gtest/gtest.h>

#include "../common/format.h"
#include "../common/prettyprint.hpp"
#include "simulated_annealing.h"

struct Bench {
    std::string name;
    int num_machines, num_parts;
    std::vector<std::vector<int>> data{};
};

std::vector<Bench> load_benches() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const std::string path = "lab5/benchmarks/";
    const std::vector<std::string> bench_names = {"20x20", "24x40", "30x50", "30x90", "37x53"};

    std::vector<Bench> benches;
    for (const std::string &i : bench_names) {
        std::string f = string_format("%s%s.txt", path.c_str(), i.c_str());
        std::ifstream inf(f);

        int m, p;
        inf >> m >> p;

        benches.push_back({i, m, p});
        benches.back().data.assign(m, std::vector<int>(p));

        inf.get();
        inf.get();
        for (int j = 0; j < m; ++j) {
            std::string line;
            std::getline(inf, line);
            std::stringstream ss(line);

            int part;
            ss >> part;
            while (ss >> part) {
                benches.back().data[j][part - 1] = 1;
            }
        }

        inf.close();
    }

    return benches;
}

auto benches = std::make_shared<std::vector<Bench>>(load_benches());
const int kRepeats = 5;

TEST(CFP, SimulatedAnnealing) {
    srand(time(NULL)); // random numbers
    const std::string test_results_path = "lab5/test_results/";

    for (Bench &bench : *benches) {
        double total_time = 0;
        SAResponse best_result{0, 0, {}};
        for (int i = 0; i < kRepeats; ++i) {
            const clock_t begin_time = clock(); // gets time
            auto result = simulated_annealing(bench.data, bench.num_machines, bench.num_parts, 10, 0.002, 0.7, 10, 6);
            total_time += static_cast<double>(clock () - begin_time) / CLOCKS_PER_SEC;
            if (result.best_cost > best_result.best_cost) {
                best_result = result;
            }
            std::cerr << bench.name << '\t' << result.num_cells << '\t' << result.best_cost << '\n';
        }
        std::cerr << '\n';

        total_time /= kRepeats;
        std::string sol_filename = string_format("%s%s.sol", test_results_path.c_str(), bench.name.c_str());


        std::ofstream f(test_results_path + "results.txt", std::ios::binary|std::ios::app);
        f << bench.name << ',' << best_result.num_cells << ',' << best_result.best_cost << ',' << sol_filename << ',' << total_time << "\n";
        f.close();

        f.open(sol_filename, std::ios::binary|std::ios::app);
        auto xy = get_xy(bench.data, bench.num_machines, bench.num_parts, best_result.num_cells, best_result.solution);
        auto x = xy.first, y = xy.second;
        for (int i = 0; i < bench.num_machines; ++i) {
            for (int j = 0; j < best_result.num_cells; ++j) {
                if (x[i][j] == 1) {
                    f << j + 1 << ' ';
                    continue;
                }
            }
        }
        f << '\n';
        for (int i = 0; i < bench.num_parts; ++i) {
            for (int j = 0; j < best_result.num_cells; ++j) {
                if (y[i][j] == 1) {
                    f << j + 1 << ' ';
                    continue;
                }
            }
        }
        f.close();
    }
}
