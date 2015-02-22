#include <motor.hpp>

namespace robot {

  Motor::Motor(const char code, Serial& serial_port)
    : serial_(serial_port)
  {
    this->state_ = 0;
    std::stringstream stream;
    stream << code << "f;";
    this->forward_command_ = stream.str();
    stream.str("");
    stream << code << "b;";
    this->backward_command_ = stream.str();
    stream.str("");
    stream << code << "s;";
    this->stop_command_ = stream.str();
  }



  void Motor::forward()
  {
    if (this->state_ <= 0) {
      this->serial_.write(this->forward_command_);
      this->state_ = 1;
    }    
  }



  void Motor::reverse()
  {
    if (this->state_ >= 0) {
      this->serial_.write(this->backward_command_);
      this->state_ = -1;
    }    
  }



  void Motor::stop()
  {
    if (this->state_ != 0) {
      this->serial_.write(this->stop_command_);
      this->state_ = 0;
    }    
  }
  
}
