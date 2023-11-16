//! aca va todo lo releacionado con la funcion gready
#include <iostream>
#include <vector>
#include <limits>


#include "Gready.h"
#include "File_RW.h"
using namespace std;


int score_employee(Instance instance, int day, int shift, int employee, vector<vector<string>> schedule) {
  // Calculo la cantidad de turnos que ha realizado el empleado hasta el día actual
  int total_shifts = 0;
  for (int i = 0; i < day; i++) {
    if (schedule[i][shift] == instance.staff[employee].id) {
      total_shifts++;
    }
  }

  // Calculo el tiempo trabajado por el empleado hasta el día actual
  int total_time = 0;
  for (int i = 0; i < day; i++) {
    if (schedule[i][shift] == instance.staff[employee].id) {
      total_time += instance.shifts[shift].duracion;
    }
  }

  // Calculo la cantidad de turnos consecutivos que ha realizado el empleado
  int consecutive_shifts = 1;
  for (int i = day - 1; i >= 0; i--) {
    if (schedule[i][shift] == instance.staff[employee].id) {
      consecutive_shifts++;
    } else {
      break;
    }
  }

  // Calculo la cantidad de días libres que ha tenido el empleado entre turnos
  int consecutive_days_off = 0;
  for (int i = day - 1; i >= 0; i--) {
    if (schedule[i][shift] != instance.staff[employee].id) {
      consecutive_days_off++;
    } else {
      break;
    }
  }

  // Calculo la puntuación del empleado
  int score = 0;
  // Cantidad máxima de turnos
  if (total_shifts > instance.staff[employee].cantidadTurnos.length()) {
    score -= 1000;
  } else if (total_shifts < instance.staff[employee].cantidadTurnos[0]) {
    score -= 100 * (instance.staff[employee].cantidadTurnos[0] - total_shifts);
  } else {
    score += 100;
  }
  // Tiempo trabajado
  if (total_time > instance.staff[employee].maxMin) {
    score -= 100 * (total_time - instance.staff[employee].maxMin);
  } else if (total_time < instance.staff[employee].minMin) {
    score -= 100 * (instance.staff[employee].minMin - total_time);
  }
  // Turnos consecutivos
  if (consecutive_shifts > instance.staff[employee].maxConsecutiveShifts) {
    score -= 100 * (consecutive_shifts - instance.staff[employee].maxConsecutiveShifts);
  } else if (consecutive_shifts < instance.staff[employee].minConsecutiveShifts) {
    score -= 100 * (instance.staff[employee].minConsecutiveShifts - consecutive_shifts);
  }
  // Días libres entre turnos
  if (consecutive_days_off < instance.staff[employee].minConsecutiveDaysOff) {
    score -= 100 * (consecutive_days_off - instance.staff[employee].minConsecutiveDaysOff);
  }
  // Fines de semana
  if (instance.staff[employee].maxWeekends > 0 && total_shifts / 2 > instance.staff[employee].maxWeekends) {
    score -= 100 * (total_shifts / 2 - instance.staff[employee].maxWeekends);
  }

  // Restricciones personalizadas
  // ...

  return score;
}



bool check_coverage(Instance instance, vector<vector<string>> schedule) {
  // Itero por los días
  for (int day = 0; day < instance.days; day++) {
    // Itero por los turnos
    for (int shift = 0; shift < instance.lenShifts; shift++) {
      // Cuento la cantidad de empleados asignados al turno
      int assigned_employees = 0;
      for (int employee = 0; employee < instance.lenStaff; employee++) {
        if (schedule[day][shift] == instance.staff[employee].id) {
          assigned_employees++;
        }
      }

      // Verifico que la cantidad de empleados asignados sea suficiente
      if (assigned_employees < instance.sectionCover[day].requiredEmployees) {
        return false;
      }
    }
  }

  return true;
}
int to_int(string str) {
    int value;
    stringstream ss(str);
    ss >> value;
    return value;

}

int get_value_of_schedule(Instance instance, vector<vector<string>> schedule) {
  int value = 0;
  for (int i = 0; i < schedule.size(); i++) {
    for (int j = 0; j < schedule[i].size(); j++) {
      int shift = to_int(schedule[i][j]);
      value += score_employee(instance, i, j, shift, schedule);
    }
  }
  return value;
}



void print_schedule(vector<vector<string>> schedule) {
  for (int i = 0; i < schedule.size(); i++) {
    for (int j = 0; j < schedule[i].size(); j++) {
      cout << schedule[i][j] << " ";
    }
    cout << endl;
  }
}


vector<vector<string>> greedy_schedule(Instance instance) {
  // Inicializo la matriz de horarios
  vector<vector<string>> schedule(instance.days, vector<string>(instance.lenShifts));

  // Itero por los días
  for (int day = 0; day < instance.days; day++) {
    // Itero por los turnos
    for (int shift = 0; shift < instance.lenShifts; shift++) {
      // Busco al empleado que mejor se adapte al turno
      string best_employee = "";
      int best_score = numeric_limits<int>::min();
      for (int employee = 0; employee < instance.lenStaff; employee++) {
        // Calculo la puntuación del empleado
        int score = score_employee(instance, day, shift, employee, schedule);
        if (score > best_score) {
          best_employee = instance.staff[employee].id;
          best_score = score;
        }
      }

      // Asigno el turno al empleado
      schedule[day][shift] = best_employee;
    }
  }

  // Verifico que la cobertura de turnos esté completa
  if (!check_coverage(instance, schedule)) {
    cout << "La cobertura de turnos no está completa" << endl;
    return vector<vector<string>>();
  }

  return schedule;
}
