#include <functional>
#include <iostream>

#include <boost/asio.hpp>
#include <crow_all.h>

#include <robot.hpp>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <device>" << std::endl;
    return 1;
  }

  boost::asio::io_service io;
  robot::Robot robot(io, 9600, argv[1]);

  crow::SimpleApp app;
  crow::mustache::set_base(".");

  CROW_ROUTE(app, "/")
    ([](){
      return "About Crow example.";
    });

  app.port(8080).multithreaded().run();
}