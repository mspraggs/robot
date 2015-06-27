#include <chrono>
#include <thread>


namespace robot {

  void timer_loop(const std::function<void()> func,
    const unsigned int num_loops, const std::chrono::milliseconds interval,
    bool& cont, std::mutex& timer_mutex) {
    for (unsigned int i = 0; i < num_loops; ++i) {
      std::this_thread::sleep_for(interval);
      timer_mutex.lock();
      bool my_cont = cont;
      timer_mutex.unlock();
      if (not my_cont) {
        return;
      }
    }
    func();
  }


  class Timer {
  public:
    Timer(const unsigned int duration, const std::function<void()> func,
      const unsigned int resolution = 10)
      : func_(func), num_loops_(duration / resolution), resolution_(resolution),
        active_(false) { }

    void start()
    {
      active_ = true;
      std::thread t(timer_loop, func_, num_loops_, resolution_,
        std::ref(active_), std::ref(lock_));
      t.detach();
    }

    void reset()
    {
      active_ = false;
      std::this_thread::sleep_for(resolution_);
    };

  private:
    bool active_;
    unsigned int num_loops_;
    std::chrono::milliseconds resolution_;
    std::function<void()> func_;
    std::mutex lock_;
  };

}