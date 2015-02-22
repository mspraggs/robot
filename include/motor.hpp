#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <serial.hpp>

namespace robot {

  class Motor {
  public:
    Motor(const char code, Serial& serial_port);

    void forward();
    void reverse();
    void stop();

  private:
    int state_;
    std::string forward_command_;
    std::string backward_command_;
    std::string stop_command_;
    Serial& serial_;
  };
  
}

#endif
