# https://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-for-gcc-on-linux

#
# BOOST
#
BOOST_INC = -IC:\PRG\boost_1_77_0___compiled\include\boost-1_77
BOOST_LIBS_PATH = C:\PRG\boost_1_77_0___compiled
BOOST_LIBS = -lboost_system -lboost_thread

#
# system
#
SYSTEM_LIBS = -lws2_32

#
# Qt
#
QT_INC = -IC:\Qt\6.3.1\mingw_64\include
QT_LIBS_PATH = C:\Qt\6.3.1\mingw_64\lib
QT_LIBS = -lQt6Core

#
# project
#
HEADERS = 
INC = -I./h
BUILD_DIR = obj


all: client server shared_lib_v1 shared_lib_v2

default: all

#
# client
#
client.o: cpp/client.cpp $(HEADERS)
	@echo making client ...
	g++ -c cpp/client.cpp -o obj/client.o $(BOOST_INC) $(QT_INC)

client: client.o
#
# it does not link staticaly Qt
#
	g++ -static -static-libgcc -static-libstdc++ obj/client.o -o client -L $(BOOST_LIBS_PATH) $(BOOST_LIBS) $(SYSTEM_LIBS)
#g++ -static -static-libgcc -static-libstdc++ obj/client.o -o client -L $(BOOST_LIBS_PATH) $(BOOST_LIBS) $(SYSTEM_LIBS) -L $(QT_LIBS_PATH) $(QT_LIBS)
#g++ obj/client.o -o client -static -L $(BOOST_LIBS_PATH) $(BOOST_LIBS) $(SYSTEM_LIBS) -L $(QT_LIBS_PATH) C:\Qt\6.3.1\mingw_64\lib\libQt6Core.a

#
# -static: not needed, just in case
#

#
# full path works
#
# it does not link staticaly Qt
#g++ obj/client.o -o client -L $(BOOST_LIBS_PATH) $(BOOST_LIBS) $(SYSTEM_LIBS) -L $(QT_LIBS_PATH) C:\Qt\6.3.1\mingw_64\lib\libQt6Core.a

#
# server
#
server.o: cpp/server.cpp $(HEADERS)
	@echo making server ...
	g++ -c cpp/server.cpp -o obj/server.o $(BOOST_INC)

server: server.o
	g++ obj/server.o -o server -L $(BOOST_LIBS_PATH) $(BOOST_LIBS) $(SYSTEM_LIBS)


#
# dll 1 
#
shared_lib_v1.o: cpp/shared_lib_v1.cpp
	@echo making dll_1 ...
	g++ -c cpp/shared_lib_v1.cpp -o obj/shared_lib_v1.o -DBUILD_MY_DLL $(INC)

shared_lib_v1: shared_lib_v1.o
	g++ -shared obj/shared_lib_v1.o -o artifacts/shared_lib_v1.dll -Wl,--out-implib,libshared_lib.a


#
# dll 2
#
shared_lib_v2.o: cpp/shared_lib_v2.cpp
	@echo making dll_2 ...
	g++ -c cpp/shared_lib_v2.cpp -o obj/shared_lib_v2.o -DBUILD_MY_DLL $(INC)

shared_lib_v2: shared_lib_v2.o
	g++ -shared obj/shared_lib_v2.o -o artifacts/shared_lib_v2.dll -Wl,--out-implib,libshared_lib.a

clean:
#	-rm -f obj/client.o
#	-rm -f client
	-del -fR $(BUILD_DIR)
#
# next does not work
#
	-del -f client server shared_lib_v1 shared_lib_v2
