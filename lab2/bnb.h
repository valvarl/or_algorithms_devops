#ifndef OR_ALGORITHMS_BNB_H
#define OR_ALGORITHMS_BNB_H

#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

// Stores the number of items
int data_size;

// Stores the knapsack capacity
float w_capacity;

typedef struct Node {

    // Upper Bound: Best case (Fractional Knapsack)
    float ub = 0;

    // Lower Bound: Worst case (0/1)
    float lb = 0;

    // Level of the node in the decision tree
    int level = 0;

    // Stores if the current item is selected or not
    bool flag = false;

    // Total Value: Stores the sum of the values of the items included
    float tv = 0;

    // Total Weight: Stores the sum of the weights of the items included
    float tw = 0;
} Node;

// Function to calculate upper bound (includes fractional part of the items)
float upper_bound(float tv, float tw, int idx, const std::vector<Item> &data,
                  const std::vector<size_t> &index) {
    float value = tv;
    float weight = tw;
    for (int i = idx; i < data_size; i++) {
        if (weight + static_cast<float>(data[index[i]].weight) <= w_capacity) {
            weight += static_cast<float>(data[index[i]].weight);
            value -= static_cast<float>(data[index[i]].price);
        } else {
            value -= static_cast<float>(w_capacity - weight) /
                    static_cast<float>(data[index[i]].weight) *
                    static_cast<float>(data[index[i]].price);
            break;
        }
    }
    return value;
}

// Function to calculate lower bound (doesn't
// include fractional part of the items)
float lower_bound(float tv, float tw, int idx, const std::vector<Item> &data,
                  const std::vector<size_t> &index) {
    float value = tv;
    float weight = tw;
    for (int i = idx; i < data_size; i++) {
        if (weight + static_cast<float>(data[index[i]].weight) <= w_capacity) {
            weight += static_cast<float>(data[index[i]].weight);
            value -= static_cast<float>(data[index[i]].price);
        } else {
            break;
        }
    }
    return value;
}

class comp {
public:
    bool operator()(Node lhs, Node rhs) {
        return lhs.lb > rhs.lb;
    }
};

void
assign(Node &node, float ub, float lb, int level, bool flag, float tv, float tw) {
    node.ub = ub;
    node.lb = lb;
    node.level = level;
    node.flag = flag;
    node.tv = tv;
    node.tw = tw;
}

Response
        bnb_backpack(const std::vector<Item> &data, const size_t capacity) {
    size_t iter = 0;
    data_size = static_cast<int>(data.size());
    w_capacity = static_cast<float>(capacity);

    // Sort the items based on the profit/weight ratio
    auto index = descending_index(data);

    // min_lb -> Minimum lower bound of all the nodes explored
    // final_lb -> Minimum lower bound of all the paths that reached
    // the final level
    float min_lb = 0, final_lb = static_cast<float>(std::numeric_limits<int>::max());

    // curr_path -> Boolean array to store
    // at every index if the element is included or not

    // final_path -> Boolean array to store
    // the result of selection array when it reached the last level
    std::vector<bool> curr_path(data.size()), final_path(data.size());

    // Priority queue to store the nodes based on lower bounds
    std::priority_queue<Node, std::vector<Node>, comp> pq;

    Node current, left, right;

    // Insert a dummy node
    pq.push(current);
    iter++;

    while (!pq.empty()) {
        current = pq.top();
        pq.pop();

        if (current.ub > min_lb || current.ub >= final_lb) {

            // If the current node's best case
            // value is not optimal than min_lb,
            // then there is no reason to explore
            // that path including final_lb
            // eliminates all those paths whose
            // best values is equal to final_lb
            continue;
        }

        // update the path
        if (current.level != 0) {
            curr_path[current.level - 1] = current.flag;
        }

        if (current.level == data_size) {
            // Reached last level
            if (current.lb < final_lb) {
                for (int i = 0; i < data_size; i++) {
                    final_path[index[i]] = curr_path[i];
                }
            }
            final_lb = std::min(current.lb, final_lb);
            continue;
        }

        int level = current.level;

        // right node -> Excludes current item
        // Hence, cp, cw will obtain the value
        // of that of parent
        assign(right, upper_bound(current.tv, current.tw, level + 1, data, index),
               lower_bound(current.tv, current.tw, level + 1, data, index),
               level + 1, false, current.tv, current.tw);

        // Check whether adding the current
        // item will not exceed the knapsack weight
        if (current.tw + static_cast<float>(data[index[current.level]].weight) <= w_capacity) {

            // left node -> includes current item
            // c and lb should be calculated
            // including the current item.
            left.ub = upper_bound(
                    current.tv - static_cast<float>(data[index[level]].price),
                    current.tw + static_cast<float>(data[index[level]].weight),
                    level + 1, data, index);

            left.lb = lower_bound(current.tv - static_cast<float>(data[index[level]].price),
                                  current.tw + static_cast<float>(data[index[level]].weight),
                                  level + 1, data, index);

            assign(left, left.ub, left.lb, level + 1, true,
                   current.tv - static_cast<float>(data[index[level]].price),
                   current.tw + static_cast<float>(data[index[level]].weight));
        }

            // If Left node cannot be inserted
        else {

            // Stop the left node from
            // getting added to the
            // priority queue
            left.ub = left.lb = 1;
        }

        // Update the lower bound
        min_lb = std::min(min_lb, left.lb);
        min_lb = std::min(min_lb, right.lb);

        // Exploring nodes whose
        // upper bound is greater than
        // min_lb will never give
        // the optimal result

        if (min_lb >= left.ub) {
            pq.push(left);
            iter++;
        }
        if (min_lb >= right.ub) {
            pq.push(right);
            iter++;
        }
    }

    return {final_path, iter};
}

#endif //OR_ALGORITHMS_BNB_H
