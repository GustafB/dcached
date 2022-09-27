add_library(
dcached_impl client.cpp client.h constants.h file_handler.cpp file_handler.h memtable.h utility.cpp utility.h encoding.cpp encoding.h)

add_executable(dcached.out main.cpp)

target_link_libraries(dcached.out PRIVATE dcached_impl)
