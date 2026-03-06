cmake_minimum_required(VERSION 3.5) # ROS 2 建议最低版本为 3.5 
project(playgrounds)

# 1. 查找 ament_cmake 
find_package(ament_cmake REQUIRED)

# 2. 如果你有数据文件夹（如 data/ 或 maps/），需要显式安装，否则 ROS 2 找不到它们
# install(DIRECTORY data/
#   DESTINATION share/${PROJECT_NAME}/data
# )

# 3. 声明这是一个 ament 包 
ament_package()