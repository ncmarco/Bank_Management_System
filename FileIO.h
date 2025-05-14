#pragma once
#ifndef FILEIO_H_INCLUDED
#define FILEIO_H_INCLUDED
#include "menuFunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


Cont **readFromCSV(const char* filePath,const char *argPrimit1, const char *argPrimit2, int n);
void writeToCSV(const char* filePath, Cont *cont);
FILE* openFile(const char* filePath, const char* mode);
int countConturi(const char *filePath, const char *argPrimit1, const char *argPrimit2);
#endif
