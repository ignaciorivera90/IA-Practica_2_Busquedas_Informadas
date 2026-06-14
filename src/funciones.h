// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Inteligencia Artificial
// Curso: 2025-2026
// Práctica 2: Búsqueda A*
//
// Autor: Ignacio Andrés Rivera Barrientos
// Correo: alu0101675053@ull.edu.es
// Fecha: 10/06/2026
// Archivo funciones.h: declaración de funciones auxiliares utilizadas
//                      por la aplicación.
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