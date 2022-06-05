#include <iostream>
#include <algorithm> // sort, next_permutation
#include <random>
#include "tsp.h"


Graph::Graph(int V, int initial_vertex) {
    this->V = V; // assigns the number of vertices
    this->initial_vertex = initial_vertex; // assigns initial vertex
}


void Graph::addEdge(int src, int dest, double weight) {
    map_edges[std::make_pair(src, dest)] = weight;
}

double Graph::existsEdge(int src, int dest) {
    auto it = map_edges.find(std::make_pair(src, dest));

    if (it != map_edges.end()) {
        return it->second; // returns cost
    }
    return -1;
}

Genetic::Genetic(Graph *graph, int size_population, int generations,
                 int mutation_rate, bool show_population) {
    this->graph = graph;
    this->size_population = size_population;
    this->real_size_population = 0;
    this->generations = generations;
    this->mutation_rate = mutation_rate;
    this->show_population = show_population;
}

// checks if is a valid solution, then return total cost of path else return -1
double Genetic::isValidSolution(std::vector<int> &solution) {
    double total_cost = 0;
    std::set<int> set_solution;

    // checks if not contains elements repeated
    for (int i = 0; i < graph->V; i++) {
        set_solution.insert(solution[i]);
    }

    if (set_solution.size() != static_cast<size_t>(graph->V)) {
        return -1;
    }

    // checks if connections are valid
    for (int i = 0; i < graph->V; i++) {
        if (i + 1 < graph->V) {
            double cost = graph->existsEdge(solution[i], solution[i + 1]);

            // checks if exists connection
            if (cost == -1)
                return -1;
            else
                total_cost += cost;
        } else {
            double cost = graph->existsEdge(solution[i], solution[0]);

            // checks if exists connection
            if (cost == -1) {
                return -1;
            } else {
                total_cost += cost;
            }
            break;
        }
    }
    return total_cost;
}


bool Genetic::existsChromosome(const std::vector<int> &v) {
    // checks if exists in the population
    for (auto & it : population) {
        const std::vector<int> &vec = it.first; // gets the vector
        if (equal(v.begin(), v.end(), vec.begin())) // compares vectors
            return true;
    }
    return false;
}

// generates the initial population
void Genetic::initialPopulation() {
    std::vector<int> parent;

    // inserts initial vertex in the parent
    parent.push_back(graph->initial_vertex);

    // creates the parent
    for (int i = 0; i < graph->V; i++) {
        if (i != graph->initial_vertex) {
            parent.push_back(i);
        }
    }

    double total_cost = isValidSolution(parent);

    if (total_cost != -1) // checks if the parent is valid
    {
        population.emplace_back(parent, total_cost); // inserts in the population
        real_size_population++; // increments real_size_population
    }

    // makes random permutations "generations" times
    for (int i = 0; i < generations; i++) {
        // generates a random permutation
        shuffle(parent.begin() + 1,
                       parent.begin() + (rand() % (graph->V - 1) + 1), std::mt19937(std::random_device()()));

        total_cost = isValidSolution(parent); // checks if solution is valid

        // checks if permutation is a valid solution and if not exists
        if (total_cost != -1 && !existsChromosome(parent)) {
            population.emplace_back(parent, total_cost); // add in population
            real_size_population++; // increments real_size_population in the unit
        }
        if (real_size_population == size_population) // checks size population
            break;
    }

    sort(population.begin(), population.end(), sort_pred());
}


void Genetic::showPopulation() {
    std::cout << "\nShowing solutions...\n\n";
    for (auto & it : population)
    {
        const std::vector<int> &vec = it.first; // gets the vector

        for (int i = 0; i < graph->V; i++)
            std::cout << vec[i] << " ";
        std::cout << graph->initial_vertex;
        std::cout << " | Cost: " << it.second << "\n\n";
    }
    std::cout << "\nPopulation size: " << real_size_population << std::endl;
}


// inserts in the vector using binary search
void Genetic::insertBinarySearch(std::vector<int> &child, double total_cost) {
    int imin = 0;
    int imax = real_size_population - 1;

    while (imax >= imin) {
        int imid = imin + (imax - imin) / 2;

        if (total_cost == population[imid].second) {
            population.insert(population.begin() + imid, make_pair(child, total_cost));
            return;
        } else if (total_cost > population[imid].second)
            imin = imid + 1;
        else
            imax = imid - 1;
    }
    population.insert(population.begin() + imin, make_pair(child, total_cost));
}

void Genetic::crossOver(std::vector<int> &parent1, std::vector<int> &parent2) {
    std::vector<int> child1, child2;

    // map of genes, checks if already are selected
    std::map<int, int> genes1, genes2;

    for (int i = 0; i < graph->V; i++) {
        // initially the genes not are used
        genes1[parent1[i]] = 0;
        genes2[parent2[i]] = 0;
    }

    // generates random points
    int point1 = rand() % (graph->V - 1) + 1;
    int point2 = rand() % (graph->V - point1) + point1;

    // adjusts the points if they are equal
    if (point1 == point2) {
        if (point1 - 1 > 1) {
            point1--;
        } else if (point2 + 1 < graph->V) {
            point2++;
        } else {
            // point1 or point2 ?? random...
            int point = rand() % 10 + 1; // number in the range 1 to 10
            if (point <= 5)
                point1--;
            else
                point2++;
        }
    }

    // generates childs

    // until point1, child1 receives genes of the parent1
    // and child2 receives genes of the parent2
    for (int i = 0; i < point1; i++) {
        // adds genes
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
        // marks genes
        genes1[parent1[i]] = 1;
        genes2[parent2[i]] = 1;
    }

    // marks remaining genes
    for (int i = point2 + 1; i < graph->V; i++) {
        genes1[parent1[i]] = 1;
        genes2[parent2[i]] = 1;
    }

    // here is the substring inverted
    // child1 receives genes of the parent2 and
    // child2 receives genes of the parent1
    for (int i = point2; i >= point1; i--) {
        if (genes1[parent2[i]] == 0) // if the gene is not used
        {
            child1.push_back(parent2[i]);
            genes1[parent2[i]] = 1; // marks the gene
        } else {
            // if the gene already is used, chooses gene that is not used
            for (auto it = genes1.begin();
                 it != genes1.end(); ++it) {
                if (it->second == 0) // checks if is not used
                {
                    child1.push_back(it->first);
                    genes1[it->first] = 1; // marks as used
                    break; // left the loop
                }
            }
        }

        if (genes2[parent1[i]] == 0) // if the gene is not used
        {
            child2.push_back(parent1[i]);
            genes2[parent1[i]] = 1; // marks the gene
        } else {
            // if the gene already is used, chooses gene that is not used
            for (auto it = genes2.begin();
                 it != genes2.end(); ++it) {
                if (it->second == 0) // checks if is not used
                {
                    child2.push_back(it->first);
                    genes2[it->first] = 1; // marks as used
                    break; // left the loop
                }
            }
        }
    }

    // ramaining genes: child1 receives genes of the parent1
    // and child2 receives genes of the parent2
    for (int i = point2 + 1; i < graph->V; i++) {
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
    }

    // mutation
    int mutation = rand() % 100 + 1; // random number in [1,100]
    if (mutation <= mutation_rate) { // checks if the random number <= mutation rate
        // makes a mutation: change of two genes

        int index_gene1, index_gene2;
        index_gene1 = rand() % (graph->V - 1) + 1;
        index_gene2 = rand() % (graph->V - 1) + 1;

        // makes for child1
        int aux = child1[index_gene1];
        child1[index_gene1] = child1[index_gene2];
        child1[index_gene2] = aux;

        // makes for child2
        aux = child2[index_gene1];
        child2[index_gene1] = child2[index_gene2];
        child2[index_gene2] = aux;
    }

    double total_cost_child1 = isValidSolution(child1);
    double total_cost_child2 = isValidSolution(child2);

    // checks if is a valid solution and not exists in the population
    if (total_cost_child1 != -1 && !existsChromosome(child1)) {
        // add child in the population
        insertBinarySearch(child1, total_cost_child1); // uses binary search to insert
        real_size_population++; // increments the real_size_population
    }

    // checks again...
    if (total_cost_child2 != -1 && !existsChromosome(child2)) {
        // add child in the population
        insertBinarySearch(child2, total_cost_child2); // uses binary search to insert
        real_size_population++; // increments the real_size_population
    }
}

// runs the genetic algorithm
TCPResponse Genetic::run() {
    initialPopulation(); // gets initial population

    std::vector<double> cost_track;
    for (int i = 0; i < generations; i++) {
        int old_size_population = real_size_population;

        /* selects two parents (if exists) who will participate
			of the reproduction process */
        if (real_size_population >= 2) {
            if (real_size_population == 2) {
                // applying crossover in the parents
                crossOver(population[0].first, population[1].first);
            } else {
                // real_size_population > 2

                int parent1, parent2;

                do {
                    // select two random parents
                    parent1 = rand() % real_size_population;
                    parent2 = rand() % real_size_population;
                } while (parent1 == parent2);

                // applying crossover in the two parents
                crossOver(population[parent1].first, population[parent2].first);
            }

            // gets difference to check if the population grew
            int diff_population = real_size_population - old_size_population;

            if (diff_population == 2) {
                if (real_size_population > size_population) {
                    // removes the two worst parents of the population
                    population.pop_back();
                    population.pop_back();

                    // decrements the real_size_population in 2 units
                    real_size_population -= 2;
                }
            } else if (diff_population == 1) {
                if (real_size_population > size_population) {
                    population.pop_back(); // removes the worst parent of the population
                    real_size_population--; // decrements the real_size_population in the unit
                }
            }
        } else { // population contains only 1 parent
            // applying crossover in the parent
            crossOver(population[0].first, population[0].first);

            if (real_size_population > size_population) {
                population.pop_back(); // removes the worst parent of the population
                real_size_population--; // decrements the real_size_population in the unit
            }
        }

        cost_track.push_back(population[0].second);
    }

    if (show_population) {
        showPopulation(); // shows the population
    }

    std::vector<int> result = population[0].first;
    result.push_back(graph->initial_vertex);

    return {cost_track, result, population[0].second};
}
