# This file is part of hvtool project.
# Licensed under the terms of the GNU General Public License v3 or later.

project(hvtool)

add_executable(hvtool
src/helper.cpp
src/hvtool.cpp
src/main.cpp
)

target_link_libraries(hvtool
computecore
computenetwork
)
