cmake_minimum_required(VERSION 3.12)
project(MyProject)

# 添加主源文件
add_executable(MyProject 
			background.cpp		
			cd.cpp
			cp.cpp
			help.cpp
			history.cpp
			ls.cpp
			main.cpp
			mv.cpp
			pipe.cpp
			ps.cpp
			pwd.cpp
			redirect.cpp
			rm.cpp
			tree.cpp)

target_sources(MyProject PRIVATE ${SOURCES})
