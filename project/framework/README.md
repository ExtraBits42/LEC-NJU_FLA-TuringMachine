# 项目编译说明
> 作者: 201220096 钟亚晨
> 时间: 2022/12/30

所涉及的源代码文件仅包括:
```txt
# 项目根目录下:
1. ./turing-project/parser.cpp
2. ./turing-project/simulator.cpp
3. ./turing-project/turing.cpp
```
各个文件间的依赖关系为:
`parser.cpp`-\>`simulator.cpp`-\>`turing.cpp`(顶层模块&对外接口)

使用**项目提供的默认**`CMakelists.txt`文件即可完成编译。

```txt
project(tmp)
cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(${PROJECT_SOURCE_DIR}/turing-project)

aux_source_directory(${PROJECT_SOURCE_DIR}/turing-project CPP_SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(turing ${CPP_SRC})
```
