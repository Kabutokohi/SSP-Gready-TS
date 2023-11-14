// Librerias estandar de C
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

// Librerias Kabuto
#include "File_RW.h"
//#include "Gready.h"
//#include "Tabu_search.h"

using namespace std;

int main(int argc, char const *argv[])
{
  string path = "Instancias/Instance1.txt";
  Instance inst = Instant_Reader(path);

  print_Horizonte(inst);
  cout << "\n"; 
  print_shifts(inst);
  cout << "\n";
  print_staff(inst);
  cout << "\n";
  print_DaysOff(inst);
  cout << "\n";
  print_ShiftRequests(inst);
  cout << "\n";
  print_ShiftOffRequests(inst);
  cout << "\n";
  print_SectionCover(inst);

  return 0;
}
