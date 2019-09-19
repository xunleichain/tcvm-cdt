add_clang_executable(TCabigen main.cpp)
target_link_libraries(TCabigen PRIVATE clangTooling)
