#ifndef ALGS2_TIMER_H
#define ALGS2_TIMER_H

#include <ctime>

class Timer {
public:
    explicit Timer(std::string name = "undef");

    ~Timer();

private:
    std::string m_name;
    std::clock_t m_started;
protected:
};

Timer::Timer(std::string name) : m_name(std::move(name)),
                                 m_started(clock()) {
}

Timer::~Timer() {
    double secs =
            static_cast<double>(std::clock() - m_started) / CLOCKS_PER_SEC;
    std::cout << m_name << ": " << secs << " secs." << std::endl;
}

#endif //ALGS2_TIMER_H
