#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <cstdlib> 
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <filesystem>
#include <string>
#include <system_error>

void menu(int &option);
void help();
bool CheckOption(const int &option);
void pressanykey();
void clrscr();

#endif