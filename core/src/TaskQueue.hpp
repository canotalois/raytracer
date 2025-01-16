/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace RayTracer {
    class TaskQueue {
    public:
        void addTask(std::function<void(int)> task) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                tasks.emplace(std::move(task));
            }
            cv.notify_one();
        }

        std::function<void(int)> getTask() {
            std::unique_lock<std::mutex> lock(queueMutex);

            cv.wait(lock, [this]() { return !tasks.empty() || done; });
            if (done && tasks.empty()) {
                return nullptr;
            }
            auto task = std::move(tasks.front());
            tasks.pop();
            return task;
        }

        void workerFunction(int threadId) {
            while (true) {
                std::function<void(int)> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    cv.wait(lock, [this] { return !tasks.empty() || done; });
                    if (done && tasks.empty()) {
                        return;
                    }
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                if (task) {
                    task(threadId);
                }
            }
        }

        void reset() {
            std::lock_guard<std::mutex> lock(queueMutex);
            while (!tasks.empty()) {
                tasks.pop();
            }
        }

        void shutdown() {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                done = true;
            }
            cv.notify_all();
        }

    private:
        std::queue<std::function<void(int)>> tasks;
        std::mutex queueMutex;
        std::condition_variable cv;
        bool done = false;
    };
}

#endif
