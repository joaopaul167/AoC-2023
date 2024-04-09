#include <condition_variable>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename F, typename T> struct Task {
  std::function<F> function;
  T data;
};

template <typename F, typename T> 
class TaskScheduler {
private:
  std::queue<Task<F, T>> taskQueue;
  std::mutex mutex;
  std::condition_variable cv;
  std::vector<std::thread> threads;
  bool stop;
  size_t tasks_count = 0;
  size_t tasks_completed = 0;
  std::map<std::string, long> memoizedMap;
  void worker() {
    while (true) {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock, [this] { return !taskQueue.empty() || stop; });
      if (stop)
        return;
      Task<F, T> task = std::move(taskQueue.front());
      taskQueue.pop();
      lock.unlock();
      task.function(task.data, memoizedMap);
      incrementTaskCount();
    }
  }
  void incrementTaskCount() {
    std::lock_guard<std::mutex> lock(mutex);
    ++tasks_completed;
    if (tasks_completed == tasks_count) {
      cv.notify_one();
    }
  }

public:
  TaskScheduler(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
      threads.emplace_back(&TaskScheduler::worker, this);
    }
  }
  ~TaskScheduler() {
    {
      std::unique_lock<std::mutex> lock(mutex);
      stop = true;
    }
    cv.notify_all();
    for (std::thread &thread : threads) {
      thread.join();
    }
  }
  void addTask(Task<F, T> task) {
    std::lock_guard<std::mutex> lock(mutex);
    taskQueue.push(std::move(task));
    ++tasks_count;
    cv.notify_one();
  }
  void waitForCompletion() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this] { return tasks_completed == tasks_count; });
  }
};
