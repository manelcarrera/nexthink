#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <array>

using namespace std;

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef BUILD_MY_DLL
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
#endif

int SHARED_LIB get_version();

#ifdef __cplusplus
    }
#endif
