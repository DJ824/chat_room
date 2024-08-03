//
// Created by Devang Jaiswal on 8/2/24.
//

#include "chat_client.h"

chat_client::chat_client(boost::asio::io_context &io_context, const std::string &host, const std::string &port)
    : io_context_(io_context), socket_(io_context) {
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket_, endpoints);
}

void chat_client::start_read() {
    boost::asio::async_read_until(socket_, read_buffer_, '\n',
                                  [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::string message;
            std::istream is(&read_buffer_);
            std::getline(is, message);
            std::cout << message << std::endl;
            start_read();
        } else {
            std::cout << "error: " << ec.message() << std::endl;
        }
    });
}

void chat_client::run() {
    // start new thread to run io_context
    std::thread t([this]() { io_context_.run(); });
    std::string message;
    while (std::getline(std::cin, message)) {
        if (message == "exit") {
            break;
        }
        write(message);
    }
    socket_.close();
    t.join();
}

void chat_client::write(const std::string &message) {
    boost::asio::async_write(socket_, boost::asio::buffer(message + "\n"),
            [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (ec) {
            std::cout << "Error: " << ec.message() << std::endl;
        }
    });
}

