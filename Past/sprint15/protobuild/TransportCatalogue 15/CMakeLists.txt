cmake_minimum_required(VERSION 3.10)

project(TransportCatalogue CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS transport_catalogue.proto
        map_renderer.proto svg.proto graph.proto transport_router.proto)

set(JSON_FORMAT_FILES json.h json.cpp)
set(JSON_PROCESSOR_FILES json_builder.h json_builder.cpp json_reader.h json_reader.cpp)
set(SVG_FORMAT_FILES svg.h svg.cpp svg.proto)

set(TRANSPORT_CATALOGUE_FILES transport_catalogue.h transport_catalogue.cpp transport_catalogue.proto)
set(ROUTER_PROCESSOR_FILES ranges.h router.h graph.h graph.proto) 
set(TRANSPORT_ROUTER_FILES transport_router.h transport_router.cpp transport_router.proto)
set(MAP_RENDERER_FILES map_renderer.h map_renderer.cpp map_renderer.proto)
set(SERIALIZATION_FILES serialization.h serialization.cpp)

set(GENERAL_FILES main.cpp domain.h domain.cpp geo.h geo.cpp)

add_executable(transport_catalogue ${PROTO_SRCS} ${PROTO_HDRS} ${JSON_FORMAT_FILES} ${JSON_PROCESSOR_FILES}
                        ${SVG_FORMAT_FILES} ${TRANSPORT_CATALOGUE_FILES} ${TRANSPORT_ROUTER_FILES} 
                        ${MAP_RENDERER_FILES} ${GENERAL_FILES} ${SERIALIZATION_FILES} ${ROUTER_PROCESSOR_FILES})

target_include_directories(transport_catalogue PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(transport_catalogue PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

string(REPLACE "protobuf.lib" "protobufd.lib" "Protobuf_LIBRARY_DEBUG" "${Protobuf_LIBRARY_DEBUG}")
string(REPLACE "protobuf.a" "protobufd.a" "Protobuf_LIBRARY_DEBUG" "${Protobuf_LIBRARY_DEBUG}")

target_link_libraries(transport_catalogue "$<IF:$<CONFIG:Debug>,${Protobuf_LIBRARY_DEBUG},${Protobuf_LIBRARY}>" Threads::Threads)