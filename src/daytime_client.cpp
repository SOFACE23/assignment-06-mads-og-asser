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

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    //If argc != 2, an error message is printet and the program stops
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    //This is an I/O context connstructor
    boost::asio::io_context io_context;

    //A resolver constructor
    tcp::resolver resolver(io_context);

    //Sets the ip-address from the program call to endpoits 
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "daytime");

    //Constructs a socket in the same I/O context
    tcp::socket socket(io_context);

    //Connecter socket med vores endpoints. 
    boost::asio::connect(socket, endpoints);

    while(true)
    {
      //Creates a char array as buffer. This is used when data is written.
      boost::array<char, 128> buf;
      boost::system::error_code error;

      //If there is any data, data is written innto the buffer array made above. 
      //The function returns the length in number of bytes.
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      //Error messages. Either if EEnd of file or another error.
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      //This writes the data from buf. Will stop when len bytes is printed. 
      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}