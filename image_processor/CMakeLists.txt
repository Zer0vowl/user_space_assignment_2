cmake_minimum_required(VERSION 3.8)
project(image_processor)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(image_grayscale src/image_grayscale.cpp)

# Link dependencies
ament_target_dependencies(image_grayscale
  rclcpp
  sensor_msgs
  cv_bridge
  image_transport
  OpenCV
)

# Ensure OpenCV libraries are linked
target_link_libraries(image_grayscale ${OpenCV_LIBRARIES})

# Install the executable
install(TARGETS
  image_grayscale
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
