cmake_minimum_required(VERSION 3.5)

project(CppBackgroundWatcher)

set (CMAKE_CXX_STANDARD 20)

add_library(file_reader src/file_reader.h src/file_reader.cpp)
add_library(system_profiler src/system_profiler.h src/system_profiler.cpp)
add_library(time_formatter src/time_formatter.h src/time_formatter.cpp)
add_library(file_uploader src/file_uploader.h src/file_uploader.cpp)
add_library(event_listener src/event_listener.h src/event_listener.cpp)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(CppBackgroundWatcher src/main.cpp)

target_link_libraries(CppBackgroundWatcher PRIVATE file_reader 
                                                    system_profiler 
                                                    time_formatter
                                                    file_uploader 
                                                    event_listener 
                                                    "-framework CoreFoundation"
                                                    "-framework CoreGraphics"
                                                    "-framework Carbon"
                                                    "-framework IOKit"
                                                    ${CURL_LIBRARIES}
)

link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX14.4.sdk/System/Library/Frameworks/CoreFoundation.framework)
link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/System/Library/Frameworks/CoreGraphics.framework)
link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX14.4.sdk/System/Library/Frameworks/Carbon.framework)

include_directories(src)
