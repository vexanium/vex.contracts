cmake_minimum_required(VERSION 3.5)
project(eosio_token_example VERSION 1.0.0)
find_package(eosio.cdt)


file(GLOB_RECURSE SOURCE_FILES "*.hpp" "*.cpp")

add_contract(token  token ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp)

target_include_directories(token.wasm
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(token.wasm PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR})