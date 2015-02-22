#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace robot {

  class Serial
  {
  public:
    Serial(boost::asio::io_service& io, const unsigned int baud,
           const std::string& device);

    char read();
    std::string readline();
    void write(const std::string& data);
    void close();
    void reset();
    
  private:
    std::string device_;
    unsigned int baud_;
    boost::asio::io_service& io_;
    boost::asio::serial_port serial_;
  };

}

#endif
