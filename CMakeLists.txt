cmake_minimum_required(VERSION 3.10)

project(Quantiq)

add_executable(messenger Messenger/messenger.cpp messengerMain.cpp)

add_executable(receiver Receiver/receiver.cpp receiverMain.cpp)