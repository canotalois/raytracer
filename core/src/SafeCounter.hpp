/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef SAFE_COUNTER_HPP
#define SAFE_COUNTER_HPP

#include <atomic>

namespace RayTracer {
    class SafeCounter {
    public:
        SafeCounter() : counter(0) {}

        int getNextID() {
            return counter++;
        }

    private:
        std::atomic<int> counter;
    };
}

#endif
