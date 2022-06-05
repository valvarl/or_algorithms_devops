#ifndef OR_ALGORITHMS_TSP_H
#define OR_ALGORITHMS_TSP_H

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <ctime>
#include <cstdlib>


class Graph {
private:
    int V; // number of vertices
    int initial_vertex; // initial vertex
    std::map<std::pair<int, int>, double> map_edges; // map of the edges
public:
    Graph(int V, int initial_vertex);
    void addEdge(int v1, int v2, double weight); // adds a edge
    double existsEdge(int src, int dest); // checks if exists a edge
    friend class Genetic;
};

typedef std::pair<std::vector<int>, double> my_pair;

struct sort_pred {
    bool operator()(const my_pair& firstElem, const my_pair& secondElem) {
        return firstElem.second < secondElem.second;
    }
};

struct TCPResponse {
    std::vector<double> cost_track;
    std::vector<int> result;
    double best_cost;
};

class Genetic {
private:
    Graph* graph;
    std::vector<my_pair> population; // each element is a pair: vector and total cost
    int size_population; // size of population
    int real_size_population; // real size population
    int generations; // amount of generations
    int mutation_rate; // mutation rate
    bool show_population; // flag to show population
private:
    void initialPopulation(); // generates the initial population
public:
    Genetic(Graph* graph, int amount_population, int generations, int mutation_rate, bool show_population = true); // constructor
    double isValidSolution(std::vector<int>& solution); // checks if a solution is valid
    void showPopulation(); // shows population
    void crossOver(std::vector<int>& parent1, std::vector<int>& parent2); // makes the crossover
    void insertBinarySearch(std::vector<int>& child, double total_cost); // uses binary search to insert
    TCPResponse run(); // runs genetic algorithm
    bool existsChromosome(const std::vector<int> & v); // checks if exists the chromosome
};

#endif //OR_ALGORITHMS_TSP_H
