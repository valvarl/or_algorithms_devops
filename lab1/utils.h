#ifndef OR_ALGORITHMS_UTILS_H
#define OR_ALGORITHMS_UTILS_H

bool inc(int *iter) {
    (*iter)++;
    return true;
}

bool cmpstr(std::string_view s1, std::string_view s2, int *iter) {
    bool eq = true;
    for (size_t i = 0; inc(iter) && i < s1.size(); ++i) {
        if (inc(iter) && s1[i] != s2[i]) {
            eq = false;
            break;
        }
    }
    return eq;
}

struct Response {
    int result;
    int iter;
};

#endif //OR_ALGORITHMS_UTILS_H
