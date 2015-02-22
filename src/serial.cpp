#include <serial.hpp>

namespace robot {

  Serial::Serial(boost::asio::io_service& io, const unsigned int baud,
                 const std::string& device) : io_(io), serial_(io, device)
  {
    if (not this->serial_.is_open()) {
      std::cerr << "Error opening serial port" << std::endl;
      return;
    }
    this->serial_.set_option(boost::asio::serial_port_base::baud_rate(baud));
  }


  char Serial::read()
  {
    char ret = 0;
    boost::asio::read(this->serial_, boost::asio::buffer(&ret, 1));
    return ret;
  }



  std::string Serial::readline()
  {
    std::string ret;
    char buff = 0;
    while (buff != '\n') {
      buff = this->read();
      if (buff != '\r' and buff != '\n') {
        ret += buff;
      }
    }

    return ret;
  }



  void Serial::write(const std::string& data)
  {
    boost::asio::write(this->serial_, boost::asio::buffer(data.c_str(),
                                                          data.size()));
  }



  void Serial::close()
  {
    this->serial_.close();
  }

}
