#include "funciones.h"

int main(int argc, char *argv[]) {
  if (argc == 2 && std::string(argv[1]) == "--help") {
    help();
    exit(0);
  }

  if (argc != 3) {
    help();
    exit(1);
  }

  int option;
  std::string in_file = std::string(argv[1]);
  const std::string out_file = std::string(argv[2]);

  while (true) {
    clrscr();
    menu(option);
    switch (option) {
      case 1 :
        clrscr();
        std::cout << "Introducir la ruta completa del archivo (ejemplo: ../doc/M1.txt)" << std::endl;
        std::cin >> in_file;

        pressanykey();
        clrscr();
        break;
      case 2:
        clrscr();

        break;
      case 3:
        clrscr();
        return 0;
        break;
    }
  }
  return 0;
}
