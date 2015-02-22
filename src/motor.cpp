#include <motor.hpp>

namespace robot {

  Motor::Motor(const char code, Serial& serial_port)
    : serial_(serial_port)
  {
    state_ = 0;
    std::stringstream stream;
    stream << code << "f;";
    forward_command_ = stream.str();
    stream.str("");
    stream << code << "b;";
    backward_command_ = stream.str();
    stream.str("");
    stream << code << "s;";
    stop_command_ = stream.str();
  }



  void Motor::forward()
  {
    if (state_ <= 0) {
      serial_.write(forward_command_);
      state_ = 1;
    }    
  }



  void Motor::reverse()
  {
    if (state_ >= 0) {
      serial_.write(backward_command_);
      state_ = -1;
    }    
  }



  void Motor::stop()
  {
    if (state_ != 0) {
      serial_.write(stop_command_);
      state_ = 0;
    }    
  }
  
}
