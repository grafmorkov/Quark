//
// Created by GrafMorkov on 27.02.2026.
//

#ifndef QUARK_COLOR_H
#define QUARK_COLOR_H

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

// Windows colors
#define BLACK   0
#define BLUE    FOREGROUND_BLUE
#define GREEN   FOREGROUND_GREEN
#define CYAN    (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define RED     FOREGROUND_RED
#define MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE)
#define YELLOW  (FOREGROUND_RED | FOREGROUND_GREEN)
#define WHITE   (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define BRIGHT  FOREGROUND_INTENSITY

#define SET_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | BRIGHT)
#define RESET_COLOR() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE)

#else
// Unix-like ANSI escape codes
#define BLACK   "0;30"
#define RED     "0;31"
#define GREEN   "0;32"
#define YELLOW  "0;33"
#define BLUE    "0;34"
#define MAGENTA "0;35"
#define CYAN    "0;36"
#define WHITE   "0;37"
#define BRIGHT  "1"

#define SET_COLOR(color) printf("\033[%sm", color)
#define RESET_COLOR()    printf("\033[0m")

#endif

#endif //QUARK_COLOR_H