cmake_minimum_required(VERSION 3.10)

project(PhoneBook CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS contact.proto)

set(PHONEBOOK_FILES iterator_range.h main.cpp phone_book.h phone_book.cpp contact.proto)

add_executable(phonebook ${PROTO_SRCS} ${PROTO_HDRS} ${PHONEBOOK_FILES})
target_include_directories(phonebook PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(phonebook PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

string(REPLACE "protobuf.lib" "protobufd.lib" "Protobuf_LIBRARY_DEBUG" "${Protobuf_LIBRARY_DEBUG}")
string(REPLACE "protobuf.a" "protobufd.a" "Protobuf_LIBRARY_DEBUG" "${Protobuf_LIBRARY_DEBUG}")

target_link_libraries(phonebook "$<IF:$<CONFIG:Debug>,${Protobuf_LIBRARY_DEBUG},${Protobuf_LIBRARY}>" Threads::Threads)