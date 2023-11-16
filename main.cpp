// Librerias estandar de C
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

// Librerias Kabuto
#include "File_RW.h"
#include "Gready.h"
//#include "Tabu_search.h"

using namespace std;

int main()
{
  //Solicitud del nombre del archivo a leer
  string file;
  cout << "ingrese el Nombre del Archivo: ";
  cin >> file;
  
  //Concatenizaciòn de la ruta
  string fullPath = "Instancias/" + file + ".txt";

  //Lectura de la instancia
  Instance inst = Instant_Reader(fullPath);

  //Generación del horario inicial
  vector<vector<string>> schedule = greedy_schedule(inst);

  //Impresión del horario
  print_schedule(schedule);

  //Valor de la FO solución inicial
  int FO = get_value_of_schedule(inst,schedule);

  cout << "El Valor de la FO del horario es: " << FO << endl;


  /*
  
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
  cout << "\n";
  cout << "largo de los turnos: " << inst.lenShifts <<endl;
  cout << "\n";
  cout << "largo del staff: " << inst.lenStaff <<endl;
  */

  return 0;
}
