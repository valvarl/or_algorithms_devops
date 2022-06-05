#ifndef OR_ALGORITHMS_SIMULATED_ANNEALING_H
#define OR_ALGORITHMS_SIMULATED_ANNEALING_H

#include <vector>
#include <algorithm>
#include <utility>
#include <limits>
#include <random>

using matrix = std::vector<std::vector<int>>;

std::vector<std::vector<double>> get_part_similarity(const matrix &data,
                                                     int num_machines, int num_parts) {

    auto part_similarity = std::vector<std::vector<double>>(num_parts, std::vector<double>(num_parts));
    std::vector<double> median;
    median.reserve(num_parts * (num_parts - 1));
    for (int i = 0; i < num_parts; ++i) {
        for (int j = 0; j < num_parts; ++j) {
            if (i != j) {
                double a_ij = 0, b_ij = 0, c_ij = 0;
                for (int k = 0; k < num_machines; ++k) {
                    if (data[k][i] == 1 && data[k][j] == 1) {
                        a_ij += 1;
                    }
                    if (data[k][i] == 1 && data[k][j] == 0) {
                        b_ij += 1;
                    }
                    if (data[k][i] == 0 && data[k][j] == 1) {
                        c_ij += 1;
                    }
                }
                double similarity = a_ij / (a_ij + b_ij + c_ij);
                part_similarity[i][j] = similarity;
                part_similarity[j][i] = similarity;
                if (similarity > 0) {
                    median.push_back(similarity);
                }
            }
        }
    }

    return part_similarity;
}

std::vector<std::set<int>> init_solution(const matrix &data,
                                        int num_machines, int num_parts, int num_cells) {

    // считаем матрицу схожести деталей
    auto part_similarity = get_part_similarity(data, num_machines, num_parts);

    // разбиваем на `num_cells` ячеек одинакового размера
    // по построенной матрице схожести деталей
    std::vector<std::set<int>> cells(num_cells);
    std::set<int> used;
    int temp_cell = 0;
    std::vector<int> parts(num_parts);
    for (int i = 0; i < num_parts; ++i) {
        parts[i] = i;
    }
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(parts.begin(), parts.end(), rng);
    for (int i : parts) {
        if (used.find(i) != used.end()) {
            continue;
        }
        cells[temp_cell].insert(i);
        used.insert(i);
        while (true) {
            double max_similarity = 0;
            int similar_part;
            for (int part: cells[temp_cell]) {
                for (int j = 0; j < num_parts; ++j) {
                    if (j != part && cells[temp_cell].find(j) == cells[temp_cell].end()) {
                        if (part_similarity[part][j] > max_similarity && used.find(j) == used.end()) {
                            max_similarity = part_similarity[part][j];
                            similar_part = j;
                        }
                    }
                }
            }

            if (static_cast<int>(cells[temp_cell].size()) < num_parts / num_cells) {
                if (used.find(similar_part) != used.end()) {
                    break;
                }
                cells[temp_cell].insert(similar_part);
                used.insert(similar_part);
            } else {
                if (temp_cell != num_cells - 1) {
                    ++temp_cell;
                }
                break;
            }
        }
    }

    return cells;
}

std::pair<matrix, matrix> get_xy(const matrix &data, int num_machines,
                                 int num_parts, int num_cells, std::vector<std::set<int>> cells) {

    auto x = matrix(num_machines, std::vector<int>(num_cells));
    auto y = matrix(num_parts, std::vector<int>(num_cells));

    for (int i = 0; i < num_cells; ++i) {
        for (int j: cells[i]) {
            y[j][i] = 1;
        }
    }

    for (int k = 0; k < num_machines; ++k) {
        int min_sum = std::numeric_limits<int>::max(),
        min_voids = std::numeric_limits<int>::max(), chosen_cell;

        int total_e = 0;
        for (int i = 0; i < num_parts; ++i) {
            if (data[k][i] == 1) {
                total_e++;
            }
        }

        for (int i = 0; i < num_cells; ++i) {
            int v = 0;
            for (int j: cells[i]) {
                if (data[k][j] == 0) {
                    v++;
                }
            }
            int sum = v + total_e - (static_cast<int>(cells[i].size()) - v);
            if (sum < min_sum || (sum == min_sum && v < min_voids)) {
                min_sum = sum;
                min_voids = v;
                chosen_cell = i;
            }
        }

        x[k][chosen_cell] = 1;
    }

    return {x, y};
}

double cost_function(const matrix &data, const matrix &x, const matrix &y,
                     int num_machines, int num_parts, int num_cells) {
    int n_1 = 0, n_1_in = 0, n_0_in = 0, n_1_out = 0;
    for (int i = 0; i < num_machines; ++i) {
        for (int j = 0; j < num_parts; ++j) {
            n_1 += data[i][j];
        }
    }
    for (int i = 0; i < num_machines; ++i) {
        for (int j = 0; j < num_parts; ++j) {
            for (int k = 0; k < num_cells; ++k) {
//                n_1_out += data[i][j] * (1 - x[i][k] * y[j][k]);
                n_1_in += data[i][j] * x[i][k] * y[j][k];
                n_0_in += (1 - data[i][j]) * x[i][k] * y[j][k];
            }
        }
    }
//    n_1_out /= num_cells;
    return static_cast<double>(n_1_in) / (n_1 + n_0_in);
//    return static_cast<double>(n_1 - n_1_out) / (n_1 + n_0_in);
}

double cost_function(const matrix &data, int num_machines, int num_parts, int num_cells,
                     const std::vector<std::set<int>> &cells) {
    auto xy = get_xy(data, num_machines, num_parts, num_cells, cells);
    auto x = xy.first, y = xy.second;
    return cost_function(data, x, y, num_machines, num_parts, num_cells);
}

std::vector<std::set<int>> single_move(const matrix &data, int num_machines,
                                       int num_parts, int num_cells, const std::vector<std::set<int>> &cells) {
    auto tmp_cells(cells);
    auto best_sol(cells);
    double best_cost = cost_function(data, num_machines, num_parts, num_cells, cells);
    for (int i = 0; i < num_cells; ++i) {
        for (int j : cells[i]) {
            for (int k = 0; k < num_cells; ++k) {
                if (i != k) {
                    tmp_cells[i].erase(j);
                    tmp_cells[k].insert(j);
                    auto new_cost = cost_function(data, num_machines, num_parts, num_cells, tmp_cells);
                    if (new_cost > best_cost) {
                        best_cost = new_cost;
                        best_sol = tmp_cells;
                    }
                    tmp_cells[k].erase(j);
                    tmp_cells[i].insert(j);
                }
            }
        }
    }
    return best_sol;
}

std::vector<std::set<int>> exchange_move(const matrix &data, int num_machines,
                                       int num_parts, int num_cells, const std::vector<std::set<int>> &cells) {
    auto tmp_cells(cells);
    auto best_sol(cells);
    double best_cost = cost_function(data, num_machines, num_parts, num_cells, cells);
    for (int i = 0; i < num_cells; ++i) {
        for (int j : cells[i]) {
            for (int k = 0; k < num_cells; ++k) {
                if (i != k) {
                    for (int l : cells[k]) {
                        tmp_cells[i].erase(j);
                        tmp_cells[k].insert(j);
                        tmp_cells[k].erase(l);
                        tmp_cells[i].insert(l);
                        auto new_cost = cost_function(data, num_machines, num_parts, num_cells, tmp_cells);
                        if (new_cost > best_cost) {
                            best_cost = new_cost;
                            best_sol = tmp_cells;
                        }
                        tmp_cells[k].erase(j);
                        tmp_cells[i].insert(j);
                        tmp_cells[i].erase(l);
                        tmp_cells[k].insert(l);
                    }
                }
            }
        }
    }
    return best_sol;
}

struct SAResponse {
    int num_cells;
    double best_cost;
    std::vector<std::set<int>> solution;
};

SAResponse simulated_annealing(const std::vector<std::vector<int>> &data, int num_machines, int num_parts,
                         double T_0, double T_f, double alpha, int L, int D, int check=4) {
    int C = 2, C_(C);

    double best_cost = 0;
    std::vector<std::set<int>> best_S;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0, 1);

    step2:
    auto S = init_solution(data, num_machines, num_parts, C), Sc(S), S_(S);
    double cost_S = cost_function(data, num_machines, num_parts, C, S), cost_Sc(cost_S), cost_S_(cost_S);
    int counter = 0, counter_mc = 0, counter_trapped = 0, counter_stagnant = 0;
    double T_k(T_0);

    step3:
    while (counter_mc < L && counter_trapped < L / 2) {
        Sc = single_move(data, num_machines, num_parts, C, S);

        if (counter % D == 0) {
            Sc = exchange_move(data, num_machines, num_parts, C, Sc);
        }

        cost_Sc = cost_function(data, num_machines, num_parts, C, Sc);

        if (cost_Sc > cost_S_) {
            S_ = Sc;
            S = Sc;
            cost_S_ = cost_Sc;
            cost_S = cost_Sc;
            counter_stagnant = 0;
            counter_mc++;
            continue;
        }

        if (cost_Sc == cost_S_) {
            S = Sc;
            cost_S = cost_Sc;
            counter_stagnant++;
            counter_mc++;
            continue;
        }

        auto delta = cost_Sc - cost_S;
        if (std::exp(delta / T_k) > distrib(gen)) {
            S = Sc;
            counter_trapped = 0;
        } else {
            counter_trapped++;
        }

        counter_mc += 1;
    }

    if (T_k <= T_f || counter_stagnant >= check) {
        if (cost_S_ >= best_cost) {
            best_cost = cost_S_;
            best_S = S_;
            C_ = C;
            C++;
            goto step2;
        } else {
            return {C_, best_cost, best_S};
        }
    } else {
        T_k *= alpha;
        counter_mc = 0;
        counter++;
        goto step3;
    }
}

#endif //OR_ALGORITHMS_SIMULATED_ANNEALING_H
