cmake_minimum_required(VERSION 3.20)
project(OmSTU_CS)

set(CMAKE_CXX_STANDARD 20)

set(HEADERS omstl/list.h omstl/iterator.h)

add_executable(OmSTU_CS main.cpp ${HEADERS})
