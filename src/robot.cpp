#include <robot.hpp>

namespace robot {

  Robot::Robot(boost::asio::io_service& io, const unsigned int baud_rate,
               const std::string& device)
    : serial_(io, baud_rate, device), left_motor_('l', serial_),
      right_motor_('r', serial_)
  {
    std::string ready = "";
    while (ready.size() < 2) {
      ready = this->serial_.readline();
    }
  }

  

  void Robot::forward(const int duration)
  {
    this->run_motors(std::bind(&Motor::forward, &this->left_motor_),
                     std::bind(&Motor::forward, &this->right_motor_),
                     duration);
  }

  

  void Robot::reverse(const int duration)
  {
    this->run_motors(std::bind(&Motor::reverse, &this->left_motor_),
                     std::bind(&Motor::reverse, &this->right_motor_),
                     duration);
  }

  

  void Robot::left(const int duration)
  {
    this->run_motors(std::bind(&Motor::reverse, &this->left_motor_),
                     std::bind(&Motor::forward, &this->right_motor_),
                     duration);
  }

  

  void Robot::right(const int duration)
  {
    this->run_motors(std::bind(&Motor::forward, &this->left_motor_),
                     std::bind(&Motor::reverse, &this->right_motor_),
                     duration);
  }

  

  void Robot::stop()
  {
    this->run_motors(std::bind(&Motor::stop, &this->left_motor_),
                     std::bind(&Motor::stop, &this->right_motor_),
                     -1);
  }



  template <typename T1, typename T2>
  void Robot::run_motors(const T1& left_func, const T2& right_func,
                         const int duration)
  {
    left_func();
    right_func();
    if (duration > -1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(duration));
      this->left_motor_.stop();
      this->right_motor_.stop();
    }
  }
    
}
