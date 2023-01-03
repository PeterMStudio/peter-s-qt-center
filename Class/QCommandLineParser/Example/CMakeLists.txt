cmake_minimum_required(VERSION 3.14)

project(Example LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

if (MSVC)
    add_compile_options(/W2)
    set(PYTHON_BIN  "python")
else()
    add_compile_options(-Werror=return-type) #return error if function does not return a value
    add_compile_options(-Wall -Wextra -pedantic)
endif()


add_executable(Example
  main.cpp
)
target_link_libraries(Example Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS Example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
