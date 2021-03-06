#include <serial.hpp>

namespace robot {

  Serial::Serial(boost::asio::io_service& io, const unsigned int baud,
                 const std::string& device)
    : device_(device), baud_(baud), io_(io), serial_(io, device)
  {
    if (not serial_.is_open()) {
      std::cerr << "Error opening serial port" << std::endl;
      return;
    }
    serial_.set_option(boost::asio::serial_port_base::baud_rate(baud));
  }


  char Serial::read()
  {
    char ret = 0;
    boost::asio::read(serial_, boost::asio::buffer(&ret, 1));
    return ret;
  }



  std::string Serial::readline()
  {
    std::string ret;
    char buff = 0;
    while (buff != '\n') {
      buff = read();
      if (buff != '\r' and buff != '\n') {
        ret += buff;
      }
    }

    return ret;
  }



  void Serial::write(const std::string& data)
  {
    boost::asio::write(serial_, boost::asio::buffer(data.c_str(),
                                                          data.size()));
  }



  void Serial::close()
  {
    serial_.close();
  }



  void Serial::reset()
  {
    close();
    serial_ = boost::asio::serial_port(io_, device_);
    if (not serial_.is_open()) {
      std::cerr << "Error opening serial port" << std::endl;
      return;
    }
    serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_));
  }

}
