# nexthink
_client-server and load lib exercise_

Manel Carrera  
2020-09-27

# 0. Index
1. [Introduction](#intro)
2. [Multi-platform](#multi)
3. [Operational system](#os)
4. [Compiler](#compiler)
5. [Makefile](#makefile)
6. [Libs](#libs)
7. [Files structure](#structure)
8. [Binaries](#binaries)
9. [Improvements](#improvements)
10. [References](#ref)
11. [How to build the project](#build)

# 1. Introduction<a name='intro'></a>

## Goal

### My approach

1. `client` asks (polls) server every 5 seconds for the current dll version.
2. `server` replies 3x times `1` and from then onwards `2`
3. if available version is newer than the curernt one, then
    1. client loads the dll present in the local folder `artifacts` and 
    2. shows the version by calling the `get_version` method implemend on it

### How to run the project

1. git clone git@github.com:manelcarrera/nexthink.git
2. run server or client, by doube clicking for example
3. run the other
4. in the `stdout` you will see logs below
5. `gracefully` quit the client by Ctrl+C
6. not-gracefully quit the server by Ctrl+C

![Client](img/client.png)
![Server](img/server.png)


# 2. Multi-platform <a name='multi'></a>
No, Windows-only

# 3. Operational system <a name='os'></a>
Windows 10

# 4. Compiler<a name='compiler'></a>
* mingw-18.0
* `make all`: builds server.exe, client.exe, shared_lib_v1.dll adn shared_lib_v2.dll

# 5. Makefile<a name='makefile'></a>
* Not used `CMake` as I'm not used to  
I know NexThink uses it for the `collector`
* So a basic `Makefile` writen instead

# 6. Libs<a name='libs'></a>

## 6a. BOOST
* boost_1_77_0
* compiled by me with mingw
* Linked (static) boost_system and boost_thread libs
* bost/assio is a headers-only lib so nothing to be linked

## 6b. Qt
* Qt 6.3.1
* Used pre-compiled libs for mingw_64
* Finally not used as is not straight forward to link the lib and I wanted to keep solution simple  
Qt libs have dependencies with other system libs: libstdc++-6.dll, libgcc_s_seh-1.dll and libwinpthread-1.dll


# 7. Files structure<a name='structure'></a>

* code
    * cpp
        * server.cpp
        * client.cpp
        * shared_lib_v1.cpp
        * shared_lib_v2.cpp
    * h
        * shared_lib_v2.h
* obj (hidden)  
_client.o, server.o, shared_lib_v1.o, shared_lib_v2.o_

* artifacts
    * shared_lib_v1.dll (generated)
    * shared_lib_v2.dll (generated)
* Mafefile
* server.exe (generated)
* client.exe (generated)

# 8. Binaries<a name='binaries'></a>

## 8a. server.exe

* Launches a thread with a boost tcp `listener` on ip 127.0.0.1, port 1234
* Receives latest avialable version requests sent by the client
* Replies 3x times 1 and then 2

## 8b. client.exe

* Launches a thread that `polls` for last available version every 5 seconds to the ip 127.0.0.1, port 1234  
If server not listening on that ip and port client does not die
* For `three times` server says latest dll available version is 1 so
client does nothing
* `Fourth` time server says latest dll version is 2 so 
    1. client loads shared_liv_v2.dll, 
    2. calls method get_version and prints the retrurbed value (2) and says new version
* `Fifth` time server says latest dll version is 2  
so client does nothing as the current dll version is the latest

## 8c. shared_lib_v1.dll

* dll with only one method: `get_version` that returns 1

## 8d. shared_lib_v2.dll

* dll with only one method: `get_version` that returns 2

# 9. Improvements<a name='improvements'></a>
1. Simple no-concurrent tcp client-server used  
`thread-safe` `concurernt` one to be implemented instead
2. Client does everything in the 'latest available dll request' polling thread  
it loads itself the new available dll
polling thread should be dedicated and notify the main thread to load the new dll
3. dll is not downloaded from anywhere, just taken from the artifacts folder  
Use `libCURL` when artifacts in a remote server instead
4. Implemntation is Windsows-only  
It should be `multi-platform`
5. `UT` to be done, with GoggleMoch for example
6. Code to be `commented`
7. `Gracefully shutdown` the server


# 10. References<a name='ref'></a>
* [Installing boost on Windows using MinGW-w64 (gcc 64-bit)](https://gist.github.com/zrsmithson/0b72e0cb58d0cb946fc48b5c88511da8)
* [MinGW Distro - nuwen.net](https://nuwen.net/mingw.html)
* [boost Version 1.80.0](https://www.boost.org/users/history/version_1_80_0.html)
* [boost Version 1.77.0](https://www.boost.org/users/history/version_1_77_0.html)
* [cmake download](https://cmake.org/download) / Latest Release (3.24.2)

# 11. How to build the project<a name='build'></a>
* Install `MinGW`
* Install `boost`, any release
* Compile `boost` with MinGW
* Update Makefile with current `boost` includes and libs locations
* make
* Generated files: `client.exe`, `server.exe`, `shared_lib_v1.dll`, `shared_lib_v2.dll`

