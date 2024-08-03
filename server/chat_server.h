//
// Created by Devang Jaiswal on 8/2/24.
//

#ifndef CHAT_ROOM_CHAT_SERVER_H
#define CHAT_ROOM_CHAT_SERVER_H
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class chat_server {
private:
    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
    std::vector<std::shared_ptr<tcp::socket>> clients_;
    std::mutex clients_mutex_;

public:
    chat_server(boost::asio::io_context& io_contex, int port);
    void start_accept();
    void handle_client(const std::shared_ptr<boost::asio::basic_stream_socket<tcp>> socket_ptr);
    void broadcast_message(const std::string& message);
    void remove_client(std::shared_ptr<tcp::socket> socket);

};


#endif //CHAT_ROOM_CHAT_SERVER_H
