cmake_minimum_required(VERSION 3.10)

# set the project name
project(Graph_Implementation)

set(SOURCE_DIR src)
set(HEADER_DIR headers)

include_directories(
    ${PROJECT_NAME} PUBLIC
    ${HEADER_DIR}
)

file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp" )
# file(GLOB HEADER_FILES "${HEADER_DIR}/*.h" )

# add the executable
add_executable(
    ${PROJECT_NAME} 
    ${SOURCE_FILES} 
)