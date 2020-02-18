cmake_minimum_required(VERSION 3.2)

# build number for travis
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(coursework VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(coursework VERSION 0.0.1)
endif()

if (WIN32)
    # dumb mf-ing boost+mgw sets mgw9 as a compiler
    set(Boost_COMPILER mgw91)
endif()

set(USE_TEST OFF)
find_package(Threads REQUIRED)

# boost dependensies
find_package(Boost COMPONENTS unit_test_framework system REQUIRED)

# source
set(SOURCE
        task_generator.cpp task_generator.h
        task_generator_interface.h
        calc_task_mgr.cpp calc_task_mgr.h
        result_saver.cpp result_saver.h
        thread_pool.cpp thread_pool.h
        project_config.h
)
set(EXE_SOURCE main.cpp ${SOURCE})
if (USE_TEST)
    set(TEST_SOURCE test_course.cpp ${SOURCE})
endif()

# targets and libraries
set(EXE_NAME coursework)
if (USE_TEST)
    set(TEST_NAME test_coursework)
endif()
add_executable(${EXE_NAME} ${EXE_SOURCE})
if (USE_TEST)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
endif()

# compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMP_OPTIONS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Woverloaded-virtual;-Wconversion;-Wsign-conversion;-Wduplicated-cond")
  if(CMAKE_BUILD_TYPE MATCHES Debug)
      set(CMP_OPTIONS ${CMP_OPTIONS}";-D_GLIBCXX_DEBUG;-D_GLIBCXX_DEBUG_PEDANTIC")
      message("debug")
  endif()
else()
  set(CMP_OPTIONS "/W4")
endif()

# target properties
set_target_properties(${EXE_NAME} ${TEST_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS ${CMP_OPTIONS}
)

target_include_directories(${EXE_NAME}
        PRIVATE ${Boost_INCLUDE_DIR}
    )

# add boost headers for test
if (USE_TEST)
    if(UNIX)
        set_target_properties(${TEST_NAME} PROPERTIES
            COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )
    endif()
    target_include_directories(${TEST_NAME}
            PRIVATE ${Boost_INCLUDE_DIR}
    )
endif()

# target linking
target_link_libraries(${EXE_NAME} Threads::Threads ${Boost_LIBRARIES})
if (USE_TEST)
    target_link_libraries(${TEST_NAME}
        ${Boost_LIBRARIES}
    )
endif()

# installation
install(TARGETS ${EXE_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT gwrass@mail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

# tests
if (USE_TEST)
    enable_testing()
    add_test(${TEST_NAME} ${TEST_NAME})
endif()
