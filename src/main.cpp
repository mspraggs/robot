#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <queue>
#include <thread>
#include <unordered_map>

#include <boost/asio.hpp>
#include <crow_all.h>

#include <robot.hpp>
#include <timer.hpp>


std::mutex autonomous_mutex;


enum class Message {
  GO,
  PAUSE,
  STOP
};


void autonomous_robot(robot::Robot& robot, Message& message)
{
  std::chrono::milliseconds delay(200);
  autonomous_mutex.lock();
  Message my_message = message;
  autonomous_mutex.unlock();
  while (true) {
    switch (my_message) {
      case Message::GO:
        robot.forward();
        std::this_thread::sleep_for(delay);
        robot.stop();
        std::this_thread::sleep_for(delay);
        robot.reverse();
        std::this_thread::sleep_for(delay);
        robot.stop();
        std::this_thread::sleep_for(delay);
        break;
      case Message::STOP:
        return;
      default:
        break;
    }

    autonomous_mutex.lock();
    my_message = message;
    autonomous_mutex.unlock();
  }
}


int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <device> <webserver port>" << std::endl;
    return 1;
  }

  unsigned int port = std::atoi(argv[2]);

  boost::asio::io_service io;
  robot::Robot robot(io, 9600, argv[1]);

  Message message = Message::GO;
  std::thread auto_robot(autonomous_robot, std::ref(robot), std::ref(message));
  robot::Timer timer(2000,
    [&] ()
    {
      autonomous_mutex.lock();
      message = Message::GO;
      autonomous_mutex.unlock();
    }, 10);

  crow::SimpleApp app;
  crow::mustache::set_base("assets/templates");
  std::unordered_map<std::string, std::function<void()> > command_map;
  command_map["forward"] = [&] () { robot.forward(); };
  command_map["reverse"] = [&] () { robot.reverse(); };
  command_map["left"] = [&] () { robot.left(); };
  command_map["right"] = [&] () { robot.right(); };
  command_map["reset"] = [&] () { robot.reset(); };

  CROW_ROUTE(app, "/")
    ([]{
      crow::mustache::context ctx;
      return crow::mustache::load("index.html").render();
    });

  CROW_ROUTE(app, "/command").methods("GET"_method, "POST"_method)
    ([&](const crow::request& req){
      autonomous_mutex.lock();
      message = Message::PAUSE;
      autonomous_mutex.unlock();
      robot.stop();
      timer.reset();
      timer.start();

      command_map[req.body]();
      return "";
    });

  app.port(port).multithreaded().run();
}
