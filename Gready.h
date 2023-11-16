#ifndef Greedy
#define Greedy

//Librerias estandar de c++
#include <iostream>
#include <vector>
#include <limits>


//Librerias Kabuto
#include "File_RW.h"

using namespace std;


//! Declaración de las funciones

int score_employee(Instance instance, int day, int shift, int employee, vector<vector<string>> schedule);

bool check_coverage(Instance instance, vector<vector<string>> schedule);

int to_int(string str);

int get_value_of_schedule(Instance instance, vector<vector<string>> schedule);

void print_schedule(vector<vector<string>> schedule);

vector<vector<string>> greedy_schedule(Instance instance);



#endif //Greedy