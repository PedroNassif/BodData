#include <Arduino.h>
#include <LittleFS.h>
#include "FS.h"

#ifndef _LTTLEFS_FUNCTIONS_H
#define _LITTLEFS_FUNCTIONS_H

class FileClass {
public:
    static void LittleFSbegin();
    static void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    static void createDir(fs::FS &fs, const char * path);
    static void removeDir(fs::FS &fs, const char * path);
    static void readFile(fs::FS &fs, const char * path);
    static void writeFile(fs::FS &fs, const char * path, const char * message);
    static void appendFile(fs::FS &fs, const char * path, const char * message);
    static void renameFile(fs::FS &fs, const char * path1, const char * path2);
    static void deleteFile(fs::FS &fs, const char * path);
};

#endif