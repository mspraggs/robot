#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <chrono>
#include <functional>
#include <thread>

#include <serial.hpp>
#include <motor.hpp>

namespace robot {

  class Robot
  {
  public:
    Robot(boost::asio::io_service& io, const unsigned int baud_rate,
          const std::string& device);
    ~Robot()
    {
      this->stop();
      this->serial_.close();
    }
    
    void forward(const int duration = -1);
    void reverse(const int duration = -1);
    void left(const int duration = -1);
    void right(const int duration = -1);
    void stop();
    void reset() { serial_.reset(); };
    const Motor& left_motor() const { return left_motor_; }
    const Motor& right_motor() const { return right_motor_; }
  private:
    template <typename T1, typename T2>
    void run_motors(const T1& left_func, const T2& right_func,
                    const int duration);
    
    Serial serial_;
    Motor left_motor_;
    Motor right_motor_;
  };
  
}

#endif
