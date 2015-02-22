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
  crow::mustache::set_base("assets");

  CROW_ROUTE(app, "/")
    ([]{
      crow::mustache::context ctx;
      return crow::mustache::load("index.html").render();
    });

  CROW_ROUTE(app, "/command").methods("GET"_method, "POST"_method)
    ([&](const crow::request& req){
      if (req.body == "forward") {
        robot.forward();
      }
      else if (req.body == "reverse") {
        robot.reverse();
      }
      else if (req.body == "right") {
        robot.right();
      }
      else if (req.body == "left") {
        robot.left();
      }
      else if (req.body == "stop") {
        robot.stop();
      }
      return "";
    });

  app.port(8080).multithreaded().run();
}