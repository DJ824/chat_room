//
// Created by Devang Jaiswal on 8/2/24.
//

#include "chat_server.h"

int main() {
    try {
        boost::asio::io_context io_context;
        chat_server server(io_context, 8080);
        std::cout << "server started on port 8080" << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}

