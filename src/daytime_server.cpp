//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

//This function creates a string with data about date and time. 
std::string make_daytime_string()
{
  using namespace std; 
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    //Constructor for I/O context. 
    boost::asio::io_context io_context;

    //Acceptor is used when accepting new sockets. 
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    while (true)
    {
      //Constructor socket
      tcp::socket socket(io_context);

      //The accepter accepts the socket
      acceptor.accept(socket);

      //Creates the string containing date and time for the message.  
      std::string message = make_daytime_string();

      //Constructs error_code
      boost::system::error_code ignored_error;

      //Writes the message 
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}