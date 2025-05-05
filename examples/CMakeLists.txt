# Examples CMakeLists.txt

# Basic example
add_executable(basic_example basic_example.cpp)
target_link_libraries(basic_example PRIVATE ccsl)

# Metrics example
add_executable(metrics_example metrics_example.cpp)
target_link_libraries(metrics_example PRIVATE ccsl)

# Payment example
add_executable(payment_example payment_example.cpp)
target_link_libraries(payment_example PRIVATE ccsl)

# Install examples
install(TARGETS 
    basic_example 
    metrics_example 
    payment_example
    RUNTIME DESTINATION bin/examples
)

# Copy example source files to installation directory
install(FILES
    basic_example.cpp
    metrics_example.cpp
    payment_example.cpp
    DESTINATION share/ccsl/examples
)