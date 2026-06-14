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
// Archivo funciones.cc: implementación de funciones auxiliares para
//                       mostrar ayuda, validar opciones y limpiar pantalla.

#include "funciones.h"



/**
 * @brief Muestra el menú principal del programa.
 *
 * Lee por teclado la opción seleccionada por el usuario y la almacena
 * en la variable recibida por referencia.
 */
void menu (int &option) {
  std::cout << "1. Cargar laberinto" << std::endl;     
  std::cout << "2. Ejecutar A*" << std::endl;          
  std::cout << "3. Salir del programa" << std::endl;
  std::cout << "Introduce un numero > ";
  std::cin >> option;
};



/**
 * @brief Muestra la forma correcta de ejecutar el programa.
 */
void help () {
  std::cout << "Como usarlo: ./p02_ia <input> <output>" << std::endl;     
  std::cout << "<input>: texto en formato .txt con el laberinto" << std::endl;
  std::cout << "<output> Archivo para almacenar resultados de A*" << std::endl;
}



/**
 * @brief Comprueba si la opción introducida es válida.
 *
 * Las opciones válidas del menú son 1, 2 y 3.
 */
bool CheckOption(const int &option) {
  std::vector<int> opciones = {1, 2, 3};
  for (const int& num : opciones) {
    if (option == num) {
      return true;
    }
  }
  std::cerr << "Error: A usado una opcion incorrecta" << std::endl;
  return false;
}



/**
 * @brief Pausa la ejecución hasta que el usuario pulse una tecla.
 */
void pressanykey() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
  std::cin.get(); 
}



/**
 * @brief Limpia la salida estándar de la terminal.
 */
void clrscr() {
  system("clear");
}