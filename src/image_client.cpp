//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fstream>

using boost::asio::ip::tcp;

//Exact number of bytes have been found on the properties of the "cat.jpg" image. 
constexpr size_t image_size = 17618;

void save_image(char* data, size_t len)
{
  //Creates and opens a file called "copycat.jpg"
  std::ofstream file("copycat.jpg");
  
  //Writes the content of data to the file
  file.write((char*) &data[0], len);

  //Closes file
  file.close();
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "daytime");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    while(true)
    {
      boost::array<char, image_size> buf;
      boost::system::error_code error;

      // read until buffer is full
      boost::asio::read(socket,boost::asio::buffer(buf));

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.

      else if (error)
        throw boost::system::system_error(error); // Some other error.

      save_image(buf.data(),image_size);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}