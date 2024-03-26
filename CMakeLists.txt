cmake_minimum_required(VERSION 3.5)

project(CppBackgroundWatcher)

set (CMAKE_CXX_STANDARD 20)

add_library(file_reader src/file_reader.h src/file_reader.cpp)
add_library(system_profiler src/system_profiler.h src/system_profiler.cpp)
add_library(time_formatter src/time_formatter.h src/time_formatter.cpp)


add_executable(CppBackgroundWatcher src/main.cpp)

target_link_libraries(CppBackgroundWatcher PRIVATE file_reader 
                                                    system_profiler 
                                                    time_formatter 
                                                    "-framework CoreFoundation"
                                                    "-framework CoreGraphics"
                                                    "-framework Carbon"
                                                    "-framework IOKit"
)

link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX14.4.sdk/System/Library/Frameworks/CoreFoundation.framework)
link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/System/Library/Frameworks/CoreGraphics.framework)
link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX14.4.sdk/System/Library/Frameworks/Carbon.framework)

include_directories(src)
