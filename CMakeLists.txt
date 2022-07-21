cmake_minimum_required(VERSION 3.1)

set(project_name mathparser)

project(${project_name})

set(CMAKE_CXX_STANDARD 20)

## EXT LIBS

find_library(MATHPARSER_LIB math_parser-lib REQUIRED)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Get the current working branch
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the latest commit hash
execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the latest commit date
execute_process(
        COMMAND git log -1 --format=%cd
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_DATE
        OUTPUT_STRIP_TRAILING_WHITESPACE)


set(SWAGGER_ROOT_PATH "/swagger" CACHE STRING "Default root path to the Swagger")
set(SWAGGER_UI_PATH "/ui" CACHE STRING "Default path suffix to the Swagger UI")

add_compile_definitions(
        SWAGGER_ROOT_PATH="${SWAGGER_ROOT_PATH}"
        SWAGGER_UI_PATH="${SWAGGER_UI_PATH}"
)

add_definitions(
        -DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"
)

## LIB

add_library(${project_name}-lib
        lib/controller/RootController.cpp
        lib/controller/CalcController.cpp
        lib/dto/DtoExpressionConversion.cpp
        )
target_link_libraries(${project_name}-lib
        PUBLIC CONAN_PKG::oatpp
        PUBLIC CONAN_PKG::oatpp-swagger
        PUBLIC "${MATHPARSER_LIB}"
        )
include_directories("/usr/local/include/mathparser/") # or this CXXFLAGS -I/usr/local/include/
target_include_directories(${project_name}-lib PUBLIC lib)



## EXE

add_executable(${project_name}-exe
        app/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

target_compile_definitions(${project_name}-exe PRIVATE "-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
target_compile_definitions(${project_name}-exe PRIVATE "-DGIT_BRANCH=\"${GIT_BRANCH}\"")
target_compile_definitions(${project_name}-exe PRIVATE "-DGIT_COMMIT_DATE=\"${GIT_COMMIT_DATE}\"")

## TEST

add_executable(${project_name}-test
        test/tests.cpp
        test/CalcControllerTest.cpp
        )
target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

enable_testing()
add_test(project-tests bin/${project_name}-test)

## GENERAL

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 20
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        )


