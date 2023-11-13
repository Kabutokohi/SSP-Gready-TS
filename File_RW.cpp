#include "File_RW.h"
using namespace std;
#include <vector>

//! consantes
const int MAX_SHIFTS = 200;  // Cantidad maxima de turnos
const int MAX_EMPLOYEES = 600;  // cantidad maxima de empleados
const int MAX_DAYS = 365; //cantidad maxima de dias

struct shift
{
  string id; //ID del turno
  int duracion; // Duracion del turno
  vector<string> shifts; // Turnos que no pueden ser asigandos a la vez
};


struct empleyee
{
  string id; //Id del empleado
  string cantidadTurnos; //cantidad maxima de turnos en el id (separado por |)
  int maxMin; //cantidad maxima de tiempo que puede trabajar
  int minMin; //cantidad minima que puede trabajar
  int maxConsecutiveShifts;   // cantidad máxima de turnos consecutivos que puede realizar
  int minConsecutiveShifts;   // cantidad mínima de turnos consecutivos que debe realizar
  int minConsecutiveDaysOff;  // cantidad mínima de días libres que debe tener entre dos turnos
  int maxWeekends;            // cantidad máxima de fines de semana que puede trabajar
};


struct daysOff
{
  string id;         // ID del empleado
  int len;            // cant días libres
  vector<int> days; // conjunto de días libres
};


struct ShiftRequest
{
    string id;    // ID del empleado
    int day;           // día que quiere ser asignado
    string shift; // turno que quiere ser asignado en el día day
    int weight;        // peso para la no asignación del día day
};


struct ShiftOffRequest
{
    string id;    // ID del empleado
    int day;           // día que quiere no ser asignado
    string shift; // turno que quiere no ser asignado en el día day
    int weight;        // peso para asignación del día day en el turno shift
};


struct SectionCover
{
    int day;        // día del ciclo
    string shift;  // turno del día
    int requiredEmployees; // cantidad de empleados requeridos
    int lowWeight;   // peso del no cumplimiento de la cobertura
    int highWeight;   // peso del sobre cumplimiento de la cobertura
};

struct Instance
{
    int days;
    int lenShifts;
    vector<shift> shifts;
    int lenStaff;
    vector<empleyee> staff;
    int lenDaysOff;
    vector<daysOff> daysOffData;
    int lenShiftRequests;
    vector<ShiftRequest> shiftRequests;
    int lenShiftOffRequests;
    vector<ShiftOffRequest> shiftOffRequests;
    int lenSectionCover;
    vector<SectionCover> sectionCover;
};

struct Solution
{
    Instance* inst;  // Puntero a una instancia
    vector<int> trabajoHoy;  // 0: no trabaja, 1: trabaja
    vector<int> turnoDiaAnterior;  // id del turno que trabajó el día anterior
    vector<int> turnosConsecutivos;  // cantidad de turnos consecutivos que lleva trabajando, se reinicia a 0 cuando no trabaja
    vector<int> diasLibres;  // cantidad de días libres que lleva, se reinicia a 0 cuando trabaja
    vector<int> finesDeSemana;  // cantidad de fines de semana que lleva trabajando, no se reinicia
    vector<int> minutosTrabajados;  // minutos trabajados por empleado
    vector<int> srequestsCumplidas;  // 0: no cumplido, 1: cumplido
    vector<int> soffrequestsCumplidas;  // 0: no cumplido, 1: cumplido
    vector<vector<int>> tIdStaff;  // cantidad de turnos por empleado
    vector<int> cantAsignados;  // cantidad de empleados asignados en la cobertura, i empleado, j cobertura
    vector<vector<int>> assignados;  // 0: no asignado, 1: asignado, empleado i a la cobertura j
    int funcEval;
};


void print_Horizonte(Instance inst){
  cout << "El horizonte utilizado es: " << inst.days << endl;
}

void print_shifts(Instance inst){
  cout << "Valores en inst.shifts:" << endl;
  for (const auto& shiftData : inst.shifts)
  {
    cout << "ID: " << shiftData.id << ", Duración: " << shiftData.duracion << ", No asignables a la vez: ";
    for (const auto& noAssign : shiftData.shifts)
    {
      cout << noAssign << " ";
    }
    cout << endl;
  }
}


void print_staff(Instance inst){
  // Itera a través del vector inst.staff
  for (const auto &empleado : inst.staff)
  {
    // Imprime la información de cada empleado
    cout << "ID: " << empleado.id << endl;
    cout << "Cantidad de turnos: " << empleado.cantidadTurnos << endl;
    cout << "MaxMin: " << empleado.maxMin << endl;
    cout << "MinMin: " << empleado.minMin << endl;
    cout << "MaxConsecutiveShifts: " << empleado.maxConsecutiveShifts << endl;
    cout << "MinConsecutiveShifts: " << empleado.minConsecutiveShifts << endl;
    cout << "MinConsecutiveDaysOff: " << empleado.minConsecutiveDaysOff << endl;
    cout << "MaxWeekends: " << empleado.maxWeekends << endl;

    // Separador entre empleados
    cout << "------------------------" << endl;
  }
}


void print_DaysOff(Instance inst){
  cout << "Valores en inst.daysOffData:" << endl;
  for (const auto& daysData : inst.daysOffData)
  {
    cout << "ID: " << daysData.id << ", Días libres: ";
    for (int day : daysData.days)
    {
      cout << day << " ";
    }
    cout << endl;
    }
}


void print_SectionCover(const Instance& inst) {
    cout << "Section Cover:\n";
    for (const auto& sectionCover : inst.sectionCover) {
        cout << "Día: " << sectionCover.day << ", Turno: " << sectionCover.shift << ", Empleados Requeridos: " << sectionCover.requiredEmployees
             << ", Peso para Under: " << sectionCover.lowWeight << ", Peso para Over: " << sectionCover.highWeight << "\n";
    }
    cout << "\n";
}


void print_ShiftRequests(const Instance& inst) {
    cout << "Shift Requests:\n";
    for (const auto& shiftRequest : inst.shiftRequests) {
        cout << "ID: " << shiftRequest.id << ", Día: " << shiftRequest.day << ", Turno: " << shiftRequest.shift << ", Peso: " << shiftRequest.weight << "\n";
    }
    cout << "\n";
}


void print_ShiftOffRequests(const Instance& inst) {
    cout << "Shift Off Requests:\n";
    for (const auto& shiftOffRequest : inst.shiftOffRequests) {
        cout << "ID: " << shiftOffRequest.id << ", Día: " << shiftOffRequest.day << ", Turno: " << shiftOffRequest.shift << ", Peso: " << shiftOffRequest.weight << "\n";
    }
    cout << "\n";
}


Instance Instant_Reader(const string& path){
  Instance inst;
  ifstream file(path);
  if (!file.is_open())
  {
    cerr << "Error al abrir el archivo " << path << endl;
    exit(1);
  }
  string linea;
  
  while (getline(file,linea))
  {
    if (!linea.empty())
    {
      linea.pop_back();
      if (linea == "SECTION_HORIZON")
      {
        for (int i = 0; i < 3; i++)
        {
          getline(file, linea);
        }
        inst.days = stoi(linea);
        //print_Horizonte(inst);
      }
      else if (linea == "SECTION_SHIFTS")
      {
        for (int i = 0; i < 1; i++)
        {
          getline(file,linea);
        }
        while (getline(file,linea) && linea[0] != '\r')
        {
          
          shift shifAux;
          size_t pos = 0;
          //lectura del ID
          pos = linea.find(',');
          shifAux.id = linea.substr(0,pos);
          linea.erase(0, pos +1);

          //duracion del turno
          pos = linea.find(',');
          shifAux.duracion = stoi(linea.substr(0,pos));
          linea.erase(0,pos+1);

          //turnos que no pueden ser asignados a la vez
          while ((pos = linea.find('|')) != string::npos)
          {
            string token = linea.substr(0,pos);
            shifAux.shifts.push_back(token);
            linea.erase(0, pos+1);
          }
          //se agregan los datos
          inst.shifts.push_back(shifAux);
        }
        //print_shifts(inst);
        
      }
      else if (linea == "SECTION_STAFF")
      {
        for (int i = 0; i < 1; i++)
        {
          getline(file,linea);
        }
        while (getline(file,linea) && linea[0] != '\r')
        {
          empleyee empleAux;
          size_t pos = 0;
          int tokenCount = 0;
          while ((pos = linea.find(',')) != string::npos)
          {
            string token = linea.substr(0,pos);
            linea.erase(0,pos+1);
            switch (tokenCount)
            {
            case 0:
              empleAux.id = token;
              break;
            case 1:
              empleAux.cantidadTurnos = token;
              break;
            case 2:
              empleAux.maxMin = stoi(token);
              break;
            case 3:
              empleAux.minMin = stoi(token);
              break;
            case 4:
              empleAux.maxConsecutiveShifts = stoi(token);
              break;
            case 5:
              empleAux.minConsecutiveShifts = stoi(token);
              break;
            case 6:
              empleAux.minConsecutiveDaysOff = stoi(token);
              break;
            case 7:
              empleAux.maxWeekends = stoi(token);
              break;
            default:
              break;
            }
            tokenCount ++;
          }
          if (!linea.empty())
          {
            switch (tokenCount)
            {
            case 7:
              empleAux.maxWeekends = stoi(linea);
              break;
            
            default:
              break;
            }
          }
          
          inst.staff.push_back(empleAux);
        }
        //print_staff(inst);
      }
      else if (linea == "SECTION_DAYS_OFF" )
      {
        getline(file,linea); // se salta la primera linea
        //while (getline(file,linea) && linea[0] != '\r')
        
        while (getline(file, linea) && linea[0] !='\r')
        {
          daysOff daysAux;
          istringstream ss(linea);
          string token;
          
          //Lectura del ID del empleado
          getline(ss, token, ',');
          daysAux.id = token;

          //Lee los dias libres y se agregan al vector
          while (getline(ss,token, ','))
          {
            daysAux.days.push_back(stoi(token));
          }
          inst.daysOffData.push_back(daysAux);
        }
        //print_DaysOff(inst);
      }
      else if (linea == "SECTION_SHIFT_ON_REQUESTS")
      {
        getline(file,linea); // se salta la primera linea
        while (getline(file,linea) && linea[0] != '\r')
        {
          ShiftRequest srAux;
          istringstream ss(linea);
          string token;

          //lectura de los valores de la linea separados por comas
          getline(ss, token, ',');
          srAux.id = token;

          getline(ss, token, ',');
          srAux.day = stoi(token);

          getline(ss, token, ',');
          srAux.shift = token;

          getline(ss, token, ',');
          srAux.weight = stoi(token);

          //cout << "Employee ID: " << srAux.id << ", Day: " << srAux.day << ", Shift: " << srAux.shift << ", Weight: " << srAux.weight << "\n";

          //Agregar los valores al vector en la instancia
          inst.shiftRequests.push_back(srAux);
        }
      }
      else if (linea == "SECTION_SHIFT_OFF_REQUESTS")
      {
        getline(file,linea);

        while (getline(file,linea) && linea[0] != '\r')
        {
          ShiftOffRequest sorAux;
          istringstream ss(linea);
          string token;

          //lectura de la linea separada por comas
          getline(ss, token, ',');
          sorAux.id = token;

          getline(ss, token, ',');
          sorAux.day = stoi(token);

          getline(ss, token, ',');
          sorAux.shift = token;

          getline(ss, token, ',');
          sorAux.weight = stoi(token);

          // Imprime los datos para verificar
          //cout << "Employee ID: " << sorAux.id << ", Day: " << sorAux.day << ", Shift: " << sorAux.shift << ", Weight: " << sorAux.weight << "\n";
          inst.shiftOffRequests.push_back(sorAux);
        }
        
      }
      else if (linea == "SECTION_COVER")
      {
        getline(file,linea); //se salta la primera linea
        while (getline(file,linea) && linea[0] != '\r')
        {
          SectionCover scAux;
          istringstream ss(linea);
          string token;

          //lectura del Día del ciclo
          getline(ss, token, ',');
          scAux.day = stoi(token);

          // Lee el turno del día
          getline(ss, token, ',');
          scAux.shift = token;

          // Lee la cantidad de empleados requeridos
          getline(ss, token, ',');
          scAux.requiredEmployees = stoi(token);

          // Lee el peso para no cumplimiento de la cobertura
          getline(ss, token, ',');
          scAux.lowWeight = stoi(token);

          // Lee el peso para sobre cumplimiento de la cobertura
          getline(ss, token, ',');
          scAux.highWeight = stoi(token);

          // Agrega los datos al vector en la instancia
          inst.sectionCover.push_back(scAux);
        }
        //print_SectionCover(inst);
      }
      
    }
    
  }
  file.close();
  return inst;
}


