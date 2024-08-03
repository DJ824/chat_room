//
// Created by Devang Jaiswal on 8/2/24.
//

#include "chat_server.h"

chat_server::chat_server(boost::asio::io_context &io_contex, int port) :
        io_context_{io_contex}, acceptor_(io_contex, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void chat_server::start_accept() {
    auto socket = std::make_shared<tcp::socket>(io_context_);
    acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            std::cout << "New client connected" << std::endl;
            {
                std::lock_guard<std::mutex> lock(clients_mutex_);
                clients_.push_back(socket);
            }
            std::thread([this, socket = std::move(socket)]() {
                this->handle_client(socket);
            }).detach();        }

        start_accept();

    });
}

void chat_server::handle_client(const std::shared_ptr<boost::asio::basic_stream_socket<tcp>> socket_ptr) {
    try {
        boost::asio::streambuf buffer;
        std::string message;

        while (true) {
            boost::asio::read_until(*socket_ptr, buffer, '\n');

            std::istream is(&buffer);
            std::getline(is, message);

            while (!message.empty() && std::isspace(message.back())) {
                message.pop_back();
            }

            if (!message.empty()) {
                std::cout << "received: " << message << std::endl;
                broadcast_message(message, socket_ptr);
            }

            if (message == "exit") {
                break;
            }
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "boost error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "exception in client handler: " << e.what() << std::endl;
    }

    std::cout << "client disconnected" << std::endl;
    remove_client(socket_ptr);
}

void chat_server::broadcast_message(const std::string &message, const std::shared_ptr<tcp::socket>& sender) {
    std::lock_guard<std::mutex> lock(clients_mutex_);

    for (auto it = clients_.begin(); it != clients_.end();) {
        if (*it != sender) {
            boost::system::error_code ec;
            boost::asio::write(**it, boost::asio::buffer(message + "\n"), ec);

            if (ec) {
                std::cerr << "error broadcasting to client: " << ec.message() << std::endl;
                it = clients_.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}

void chat_server::remove_client(std::shared_ptr<tcp::socket> socket_ptr) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    auto it = std::find(clients_.begin(), clients_.end(), socket_ptr);
    if (it != clients_.end()) {
        clients_.erase(it);
        std::cout << "client removed, total clients: " << clients_.size() << std::endl;
    }
}
