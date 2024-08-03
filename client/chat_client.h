//
// Created by Devang Jaiswal on 8/2/24.
//

#ifndef CHAT_ROOM_CHAT_CLIENT_H
#define CHAT_ROOM_CHAT_CLIENT_H
#include <iostream>
#include <thread>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class chat_client {
private:

    // used to handle async operations
    boost::asio::io_context& io_context_;

    tcp::socket socket_;
    boost::asio::streambuf read_buffer_;
public:
    chat_client(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    void start_read();
    void write(const std::string& message);
    void run();


};


#endif //CHAT_ROOM_CHAT_CLIENT_H
