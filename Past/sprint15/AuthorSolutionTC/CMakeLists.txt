cmake_minimum_required(VERSION 3.10)

project(TransportCatalogue CXX)
set(CMAKE_CXX_STANDARD 17)

# Эта команда найдёт собранный нами пакет Protobuf.
# REQUIRED означает, что библиотека обязательна.
# Путь для поиска укажем в параметрах команды cmake.
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

# Команда вызова protoc. 
# Ей переданы названиия переменных, в которые будут сохранены 
# списки сгенерированных файлов, а также сам proto-файл.
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
    transport_catalogue.proto
    map_renderer.proto
    svg.proto
    transport_router.proto
    graph.proto
)

# добавляем цель - transport_catalogue
add_executable(
    transport_catalogue
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    main.cpp
    domain.cpp
    geo.cpp
    json_builder.cpp
    json.cpp
    json_reader.cpp
    map_renderer.cpp
    request_handler.cpp
    serialization.cpp
    svg.cpp
    transport_catalogue.cpp
    transport_router.cpp
)

# find_package определила переменную Protobuf_INCLUDE_DIRS,
# которую нужно использовать как include-путь.
# Также нужно добавить как include-путь директорию, куда
# protoc положит сгенерированные файлы.
target_include_directories(transport_catalogue PUBLIC ${Protobuf_INCLUDE_DIRS})
target_include_directories(transport_catalogue PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Также find_package определила Protobuf_LIBRARY и Protobuf_LIBRARY_DEBUG.
# Исправим некорректное значение Protobuf_LIBRARY_DEBUG операцией замены:
string(REPLACE "protobuf.lib" "protobufd.lib" "Protobuf_LIBRARY_DEBUG" "${Protobuf_LIBRARY_DEBUG}")
string(REPLACE "protobuf.a" "protobufd.a" "Protobuf_LIBRARY_DEBUG" "${Protobuf_LIBRARY_DEBUG}")

target_link_libraries(transport_catalogue $<IF:$<CONFIG:Debug>,${Protobuf_LIBRARY_DEBUG},${Protobuf_LIBRARY}> Threads::Threads)
