#include "funciones.h"

void menu (int &option) {
  std::cout << "1. Cargar laberinto" << std::endl;     
  std::cout << "2. Ejecutar A*" << std::endl;          
  std::cout << "3. Salir del programa" << std::endl;
  std::cout << "Introduce un numero > ";
  std::cin >> option;
};

void help () {
  std::cout << "Como usarlo: ./p02_ia <input> <output>" << std::endl;     
  std::cout << "<input>: texto en formato .txt con el laberinto" << std::endl;
  std::cout << "<output> Archivo para almacenar resultados de A*" << std::endl;
}

/**
 * @brief Check that the opcode is correct
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
 * @brief function that waits for the user to press any key
 */
void pressanykey() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
  std::cin.get(); 
}

/**
 * @brief function to clear the standard ouput
 */
void clrscr() {
  system("clear");
}
