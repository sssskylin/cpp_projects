add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE battleship)
target_link_libraries(${PROJECT_NAME} PRIVATE strategy)
target_link_libraries(${PROJECT_NAME} PRIVATE board)
target_link_libraries(${PROJECT_NAME} PRIVATE parser)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})