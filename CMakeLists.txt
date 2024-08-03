cmake_minimum_required(VERSION 3.27)
project(chat_room)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "/opt/homebrew/opt/boost")

find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(chat_server
        server/chat_server.cpp
        server/chat_server.h
        server/server_main.cpp
)

target_link_libraries(chat_server PRIVATE
        Boost::system
        Boost::thread
        pthread
)

target_include_directories(chat_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/server
        ${Boost_INCLUDE_DIRS}
)

add_executable(chat_client
        client/chat_client.cpp
        client/chat_client.h
        client/client_main.cpp
)

target_link_libraries(chat_client PRIVATE
        Boost::system
        Boost::thread
        pthread
)

target_include_directories(chat_client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/client
        ${Boost_INCLUDE_DIRS}
)