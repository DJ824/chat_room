//
// Created by Devang Jaiswal on 8/2/24.
//


#include "chat_client.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "usage: chat_client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        chat_client client(io_context, argv[1], argv[2]);
        client.run();
    }
    catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}
