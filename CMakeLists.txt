cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_TOOLCHAIN_FILE H:/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
project(qtdemo VERSION 0.1.0 LANGUAGES C CXX)
find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)

find_package(glm CONFIG REQUIRED)

# 添加可执行文件
 file(GLOB SRC_FILES *.cpp)
 file(GLOB INC_FILES *.h)
 qt5_add_resources(RESOURCE_FILES main.qrc)


add_executable(qtdemo ${SRC_FILES} ${RESOURCE_FILES} ${INC_FILES})
source_group("Source Files" FILES ${SRC_FILES})
source_group("Header Files" FILES ${INC_FILES})


target_link_libraries(qtdemo PRIVATE Qt5::Widgets glm::glm)
file(GLOB monodll "${monopc_LIBRARY_DIRS}/../bin/mono*.dll")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT qtdemo)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(qtdemo PROPERTIES WIN32_EXECUTABLE TRUE)
