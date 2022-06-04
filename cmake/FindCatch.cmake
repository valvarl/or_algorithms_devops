add_library(contrib_catch_main
  contrib/catch/catch_main.cpp)

target_include_directories(contrib_catch_main
  SYSTEM PUBLIC contrib/catch)

target_link_libraries(contrib_catch_main
  pthread
  dl)