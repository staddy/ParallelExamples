#include <thread>
#include <iostream>
#include <functional>
#include <mutex>
#include <atomic>
#include <random>

std::atomic<size_t> result;
std::mt19937 mt_rand(time(0));

auto m = static_cast<double>(std::numeric_limits<std::mt19937::result_type>::max());
double getPoint() {
    return (static_cast<double>(mt_rand()) / m) * 2.0 - 1.0;
}

constexpr size_t POINTS_PER_THREAD = 10000000;

void piWorker() {
    size_t r{0};
    for (size_t i = 0; i < POINTS_PER_THREAD; ++i) {
        auto x = getPoint(), y = getPoint();
        if ((x * x + y * y) < 1.0) {
            ++r;
        }
    }
    result += r;
}

constexpr size_t N_THREADS = 16;

int main(int /*argc*/, char* /*argv*/[]) {
    std::vector<std::thread> threads;
    for (size_t i = 0; i < N_THREADS; ++i) {
        threads.emplace_back(piWorker);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << static_cast<double>(result) / (N_THREADS * POINTS_PER_THREAD) * 4.0 << std::endl;
    return 0;
}
