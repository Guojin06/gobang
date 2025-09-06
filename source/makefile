# gobang:gobang.cc 
# 	g++ $^ -o $@ -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lstdc++ -ljsoncpp
.PHONY:gobang
gobang:gobang.cc logger.hpp db.hpp online.hpp room.hpp util.hpp
	g++ -g -std=c++11 $^ -o $@ -L/usr/lib/x86_64-linux-gnu -lmysqlclient -ljsoncpp -lpthread