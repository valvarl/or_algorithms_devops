option(TEST_SOLUTION "Build solution" OFF)
option(ENABLE_PRIVATE_TESTS "Enable private tests" OFF)
option(GRADER "Building on CI" OFF)

function(patch_include_directories TARGET)
  if (TEST_SOLUTION)
    get_filename_component(TASK_NAME . NAME)
    target_include_directories(${TARGET}
      PUBLIC ../private/${TASK_NAME})
  endif()

  target_include_directories(${TARGET}
    PUBLIC .)
endfunction()

function(prepend VAR PREFIX)
  set(LIST_VAR "")
  foreach(ELEM ${ARGN})
    list(APPEND LIST_VAR "${PREFIX}/${ELEM}")
  endforeach()
  set(${VAR} "${LIST_VAR}" PARENT_SCOPE)
endfunction()

function(add_shad_executable NAME)
  add_shad_module(${NAME} EXE ${ARGN})
endfunction()

function(add_shad_library NAME)
  add_shad_module(${NAME} LIB ${ARGN})
endfunction()

function(add_shad_module NAME)
  set(OPTION_ARGS LIB EXE)
  set(MULTI_VALUE_ARGS PRIVATE_TESTS SOLUTION_SRCS SOLUTION_ONLY_SRCS STUDENT_SRCS)
  cmake_parse_arguments(SHAD_LIBRARY "${OPTION_ARGS}" "" "${MULTI_VALUE_ARGS}" ${ARGN})

  get_filename_component(TASK_NAME . NAME)
  if (TEST_SOLUTION)
    list(APPEND SHAD_LIBRARY_SOLUTION_SRCS ${SHAD_LIBRARY_SOLUTION_ONLY_SRCS})
    prepend(SHAD_LIBRARY_SOLUTION_SRCS "../private/${TASK_NAME}" ${SHAD_LIBRARY_SOLUTION_SRCS})
  else()
    list(APPEND SHAD_LIBRARY_SOLUTION_SRCS ${SHAD_LIBRARY_STUDENT_SRCS})
  endif()

  if (ENABLE_PRIVATE_TESTS)
    prepend(SHAD_LIBRARY_PRIVATE_TESTS "../private/${TASK_NAME}" ${SHAD_LIBRARY_PRIVATE_TESTS})
  endif()

  if (${SHAD_LIBRARY_EXE})
    add_executable(${NAME}
      ${SHAD_LIBRARY_UNPARSED_ARGUMENTS}
      ${SHAD_LIBRARY_SOLUTION_SRCS}
      ${SHAD_LIBRARY_PRIVATE_TESTS})
  else()
    add_library(${NAME}
      ${SHAD_LIBRARY_UNPARSED_ARGUMENTS}
      ${SHAD_LIBRARY_SOLUTION_SRCS}
      ${SHAD_LIBRARY_PRIVATE_TESTS})
  endif()

  patch_include_directories(${NAME})
endfunction()

function(add_shad_python_module NAME)
  set(MULTI_VALUE_ARGS SOLUTION_SRCS)
  cmake_parse_arguments(SHAD_LIBRARY "" "" "${MULTI_VALUE_ARGS}" ${ARGN})

  get_filename_component(TASK_NAME . NAME)
  if (TEST_SOLUTION)
    prepend(SHAD_LIBRARY_SOLUTION_SRCS "../private/${TASK_NAME}" ${SHAD_LIBRARY_SOLUTION_SRCS})
  endif()

  add_library(${NAME} SHARED
    ${SHAD_LIBRARY_UNPARSED_ARGUMENTS}
    ${SHAD_LIBRARY_SOLUTION_SRCS})

  if (APPLE)
    target_link_libraries(${NAME} INTERFACE "-undefined dynamic_lookup")
  endif()

  patch_include_directories(${NAME})

  target_include_directories(${NAME} PRIVATE ${PYTHON_INCLUDE_DIRS})

  set_target_properties(${NAME} PROPERTIES PREFIX "")
  if (APPLE)
    set_target_properties(${NAME} PROPERTIES SUFFIX .so)
  endif()
endfunction()

add_custom_target(test-all)

function(add_shad_test_binary TARGET)
  add_shad_executable(${TARGET}
    ${ARGN})

  if (TEST_SOLUTION)
    add_custom_target(
      run_${TARGET}
      WORKING_DIRECTORY .
      DEPENDS ${TARGET}
      COMMAND ${CMAKE_BINARY_DIR}/${TARGET})

    add_dependencies(test-all run_${TARGET})
  endif()
endfunction()

function(add_catch TARGET)
  add_shad_test_binary(${TARGET}
    ${ARGN})

  target_link_libraries(${TARGET}
    contrib_catch_main)
endfunction()

function(add_gtest TARGET)
  add_shad_test_binary(${TARGET}
    ../contrib/gmock_main.cc
    ${ARGN})

  target_link_libraries(${TARGET}
    gmock)
endfunction()

function(add_benchmark TARGET)
  add_shad_test_binary(${TARGET}
    ${ARGN})

  target_link_libraries(${TARGET}
    benchmark)
endfunction()
