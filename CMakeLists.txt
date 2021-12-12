cmake_minimum_required(VERSION 3.16)

# need setting to `ProjectName` below all context.
# `tmp.code-workspace` needs to be changed as well.
project(mtst_lib CXX)

cmake_minimum_required(VERSION 3.16)

#--------------- CMake compile config ------------------------------
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11) # C++11(setting standard library version)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wconversion -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winit-self -Winline -Wmissing-include-dirs -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wswitch-default -Wswitch-enum -Wvariadic-macros -Wwrite-strings ")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=native")
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -pg")
        set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "-pg")
endif(CMAKE_COMPILER_IS_GNUCXX)

#----------------- clang-format config -------------------------
find_program(CLANG_FORMAT_EXE clang-format)

file(GLOB_RECURSE project_sources CONFIGURE_DEPENDS "[a-zA-Z]*.cpp" "[a-zA-Z]*.hpp")
list(FILTER project_sources EXCLUDE REGEX "build/*")

# add build target (clang-format lints)
# In use,
# `cmake --build . --target ${PROJECT_NAME}-format --verbose`
add_custom_target("${PROJECT_NAME}-format"
  COMMAND ${CLANG_FORMAT_EXE} --style=file --dry-run ${project_sources}
  COMMENT "Running clang-format(lints)"
  VERBATIM
  )

# add build target (clang-format warning overwrite)
# In use,
# `cmake --build . --target ${PROJECT_NAME}-format-fix`
add_custom_target("${PROJECT_NAME}-format-fix"
  COMMAND ${CLANG_FORMAT_EXE} --style=file -i ${project_sources}
  COMMENT "Running clang-format(overwrite)"
  VERBATIM
  )

#------------------ build source config ------------------

# include path
include_directories(include)

# sub directory
add_subdirectory(lib)

# libraries
add_library(${PROJECT_NAME} INTERFACE)
    target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}-common)
    target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}-pso)
    target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}-harmony)

# for development executable(optional activate)
option(DEVELOP_EXECUTABLE_${PROJECT_NAME} "Build develop executable file on ProjectName package" OFF)
if(DEVELOP_EXECUTABLE_${PROJECT_NAME})
# executable
    add_executable(develop_main develop_main.cpp)
        target_link_libraries(develop_main ${PROJECT_NAME})
endif()

# tests
if(NOT without-test)
    enable_testing()
    add_subdirectory(test)
endif()
