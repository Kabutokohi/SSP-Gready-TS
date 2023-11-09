#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;


//! consantes
const int MAX_SHIFTS = 200;  // Cantidad maxima de turnos
const int MAX_EMPLOYEES = 600;  // cantidad maxima de empleados
const int MAX_DAYS = 365; //cantidad maxima de dias

/**
 * La estructura "turno" representa un turno con un ID, duración y una lista de turnos que no se pueden
 * asignado al mismo tiempo.
 * @property {string} id: el ID del turno, que es una cadena.
 * @property {int} duracion - La propiedad "duracion" representa la duración del turno, que es un
 * valor entero. Indica cuánto dura el turno en alguna unidad de tiempo (por ejemplo, horas, minutos).
 * @property {string} turnos: la propiedad "turnos" es una cadena que representa los turnos que no pueden
 * ser asignado al mismo tiempo que el turno actual. Se utiliza para garantizar que ciertos turnos no
 * se superponen o entran en conflicto entre sí.
 */
struct shift
{
  string id; //ID del turno
  int duracion; // Duracion del turno
  string shifts; // Turnos que no pueden ser asigandos a la vez
};

/**
 * La estructura "empleado" representa un empleado con varios atributos como ID, máximo y mínimo.
 * tiempo de trabajo, turnos consecutivos máximos y mínimos, días libres mínimos consecutivos y máximos
 * fines de semana.
 * @property {string} id: la propiedad id representa el identificador único de un empleado. Es un
 * valor de cadena.
 * @property {string} cantidadTurnos - La propiedad "cantidadTurnos" representa el número máximo de
 * turnos que puede trabajar un empleado en un determinado ID. Los turnos están separados por un "|" símbolo.
 * @property {int} maxMin: la cantidad máxima de minutos que un empleado puede trabajar en un solo turno.
 * @property {int} minMin: la cantidad mínima de minutos que el empleado puede trabajar en un turno.
 * @property {int} maxConsecutiveShifts: el número máximo de turnos consecutivos que un empleado puede
 * trabajar.
 * @property {int} minConsecutiveShifts: el número mínimo de turnos consecutivos que un empleado
 * debe trabajar.
 * @property {int} minConsecutiveDaysOff: el número mínimo de días libres consecutivos que un empleado
 *debe tener entre dos turnos.
 * @property {int} maxWeekends: el número máximo de fines de semana que un empleado puede trabajar.
 */
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

/**
  * La estructura "daysOff" representa los días libres de un empleado, incluido su ID, la cantidad de días
  * descanso, y los días libres específicos.
  * @property {string} id: el ID del empleado.
  * @property {int} len: la propiedad "len" representa el número de días libres para un determinado
  * empleado.
  * @property días - La propiedad "días" es un vector que almacena los días específicos que tiene un empleado
  * apagado. Cada elemento del vector representa un día y el valor del elemento representa el día.
  * número. Por ejemplo, si un empleado tiene tiempo libre los días 1, 5 y 10
  */
struct daysOff
{
  string id;         // ID del empleado
  int len;            // cant días libres
  vector<int> days; // conjunto de días libres
};

/**
  * La estructura ShiftRequest representa una solicitud de un empleado para que se le asigne un turno específico en un
  * día específico, con un peso asociado por no estar asignado.
  * @property {string} id: el ID del empleado que realiza la solicitud de turno.
  * @property {int} día: la propiedad "día" representa el día en el que el empleado quiere estar
  * turno asignado. Es un valor entero que representa el día del mes.
  * @property {string} turno: la propiedad "turno" representa el turno deseado que desea un empleado
  *para ser asignado en un día específico.
  * @property {int} peso - La propiedad peso representa la importancia o prioridad de no asignar
  * la solicitud de turno para un día en particular. Se puede utilizar para priorizar ciertas solicitudes de turno sobre
  * otros al tomar decisiones de programación.
  */
struct ShiftRequest
{
    string id;    // ID del empleado
    int day;           // día que quiere ser asignado
    string shift; // turno que quiere ser asignado en el día day
    int weight;        // peso para la no asignación del día day
};

/**
  * La estructura ShiftOffRequest representa una solicitud de un empleado para que no se le asigne un turno específico
  *en un día específico.
  * @property {string} id: el ID del empleado que realiza la solicitud de turno libre.
  * @property {int} día: la propiedad "día" representa el día en el que el empleado quiere estar libre
  *del trabajo. Es un valor entero que representa el día del mes.
  * @property {string} turno: la propiedad "turno" representa el turno que el empleado no quiere
  * ser asignado en un día específico.
  * @property {int} peso - La propiedad peso representa la importancia o prioridad de asignar el
  * día y turno del empleado. Se puede utilizar para determinar el orden en el que se envían las solicitudes.
  * procesado o para calcular la puntuación general o el costo de la tarea.
  */
struct ShiftOffRequest
{
    string id;    // ID del empleado
    int day;           // día que quiere no ser asignado
    string shift; // turno que quiere no ser asignado en el día day
    int weight;        // peso para asignación del día day en el turno shift
};

/**
  * La estructura SecciónCubierta representa los requisitos de cobertura de una sección para un día y turno específicos.
  * incluyendo el número de empleados requeridos y los pesos por no cumplir o exceder el
  * cobertura.
  * @property {int} día: el día del ciclo, representado como un número entero.
  * @property {string} turno: la propiedad "turno" representa el turno del día, como "mañana",
  * "tarde", o "noche".
  * @property {int} requireEmployees: el número de empleados necesarios para la cobertura de la sección
  *en un día y turno específico.
  * @property {int} lowWeight: la propiedad lowWeight representa el peso asignado al
  *incumplimiento de la cobertura. Se utiliza para calcular la penalidad o costo asociado a no
  * tener suficientes empleados programados para un día y turno en particular en una sección.
  * @property {int} highWeight: la propiedad highWeight representa el peso asignado al
  * sobrecumplimiento del requisito de cobertura. Se utiliza para calcular el peso total de un
  * Cobertura de la sección en función del número de empleados presentes.
  */
struct SectionCover
{
    int day;        // día del ciclo
    string shift;  // turno del día
    int requiredEmployees; // cantidad de empleados requeridos
    int lowWeight;   // peso del no cumplimiento de la cobertura
    int highWeight;   // peso del sobre cumplimiento de la cobertura
};

/**
  * La instancia de estructura representa una instancia de un problema de programación y contiene información sobre
  * turnos, personal, días libres, solicitudes de turno, solicitudes de turno libre y cobertura de sección.
  * @property {int} días: el número de días de la instancia.
  * @property {int} lenShifts: la variable "lenShifts" representa la duración o el número de turnos en
  * la instancia.
  * @property turnos: un vector de objetos de turno, que representa los turnos disponibles en la instancia.
  * @property {int} lenStaff - La propiedad "lenStaff" representa la longitud o tamaño del vector
  * "personal", que contiene instancias de la estructura "empleado". Indica el número de empleados en
  * el vector "personal".
  * @property staff: la propiedad "staff" es un vector de objetos "empleados". Representa la lista de
  * empleados disponibles para programar turnos en la instancia.
  * @property {int} lenDaysOff - La variable "lenDaysOff" representa la longitud o tamaño del vector
  * "díasSinDatos". Indica la cantidad de elementos u objetos almacenados en el vector "daysOffData".
  * @property daysOffData - daysOffData es un vector que almacena información sobre los días libres de cada uno
  * empleado. Contiene objetos del tipo días libres, que probablemente tengan propiedades como ID de empleado.
  * y los días libres específicos para ese empleado.
  * @property {int} lenShiftRequests: la propiedad "lenShiftRequests" representa la longitud o el tamaño de
  * el vector "shiftRequests". Indica el número de elementos u objetos almacenados en el
  * Vector "Solicitudes de cambio".
  * @property shiftRequests: un vector de objetos ShiftRequest, que representa las solicitudes de turno realizadas por
  * empleados.
  * @property {int} lenShiftOffRequests: la propiedad "lenShiftOffRequests" representa la longitud o
  * tamaño del vector "shiftOffRequests". Indica el número de elementos u objetos almacenados en el
  * Vector "shiftOffRequests".
  * @property shiftOffRequests: la propiedad `lenShiftOffRequests` representa la longitud o el tamaño del
  * Vector `shiftOffRequests`, que contiene instancias de la estructura `ShiftOffRequest`. este vector
  * almacena las solicitudes de descanso realizadas por los empleados, indicando los turnos que desean tomar descanso
  * de.
  * @property {int} lenSectionCover: la propiedad "lenSectionCover" representa la longitud o el tamaño de
  * el vector "secciónCubierta". Indica el número de elementos u objetos almacenados en el
  * Vector "cubierta de sección".
  * @property sectionCover: la propiedad "sectionCover" es un vector que almacena objetos de tipo
  * "Cubierta de Sección". Tiene una longitud de "lenSectionCover" y contiene información sobre la cobertura de
  * diferentes secciones o áreas dentro de la instancia.
  */
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

/**
  * El código define una estructura Solución en C++ que representa una solución a un problema de programación, con
  * diversos atributos y métodos para evaluar y aplicar movimientos.
  *
  * @param instancia Una instancia del problema, que contiene todos los datos necesarios para resolverlo.
  * @param solucion El parámetro "solucion" es un objeto de la estructura Solución. Representa un
  * solución a una instancia de problema. Contiene varios vectores y variables que almacenan información.
  * sobre la solución, como las asignaciones de empleados a turnos, el número de turnos consecutivos
  * trabajado por cada empleado, el número de días libres
  * @param empleado El parámetro "empleado" representa el índice o ID del empleado. Se utiliza para identificar
  * un empleado específico en la solución.
  * @param turno El parámetro "turno" representa el ID del turno que está realizando el empleado
  * asignado a. Se utiliza para acceder a información sobre el turno, como su duración y costo, desde
  * el vector "instancia.shifts".
  * @param dia El parámetro "dia" representa el día para el cual se está evaluando el movimiento o
  * aplicado. Es un valor entero que representa el día del horario.
  *
  * @return La función `evaluar_movimiento` devuelve el costo total del movimiento.
  */
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

/**
  * La función "Instant_Reader" lee datos de un archivo y los almacena en un objeto "Instancia".
  *
  * @param filename El parámetro filename es una cadena que representa el nombre del archivo que
  * quiero leer de.
  * @param instancia El parámetro `instancia` es un objeto de tipo `Instancia`. se pasa por
  * referencia a la función `Instant_Reader`.
  */
void Instant_Reader(const string &filename, Instance &instancia){
  ifstream file(filename);
  if (!file.is_open())
  {
    cerr << "Error al abrir el archivo " << filename << endl;
    exit(1);
  }
  string linea;
  shift shifAux;
  empleyee empleAux;
  daysOff daysAux;
  ShiftRequest srAux;
  ShiftOffRequest sorAux;
  SectionCover scAux;
  while (getline(file,linea))
  {
    if (!linea.empty())
    {
      if (linea.compare(0,14,"SECTION_HORIZON")== 0)
      {
        for (int i = 0; i < 3; i++)
        {
          getline(file,linea);
          cout << linea << endl;
          instancia.days = stoi(linea);
        }
        printf("%d",instancia.days);
      }
      else if (linea.compare(0, 13, "SECTION_STAFF") == 0)
      {
        for (int i = 0; i < 2; i++){
          getline(file, linea);
        }
        while (linea[0] != '\r')
        {
            getline(file,linea);
            empleAux = empleyee();
            size_t pos = 0;
            int tokenCount = 0;
            while ((pos = linea.find(',')) != string::npos)
            {
              string token = linea.substr(0,pos);
              linea.erase(0, pos+1);
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
            instancia.staff.push_back(empleAux);
          }    
      }
      else if (linea.compare(0, 16, "SECTION_DAYS_OFF") == 0)
      {
        for (int i = 0; i < 2; i++)
        {
          getline(file,linea);
        }
        while (linea[0] != '\r')
        {
          getline(file,linea);
          daysAux = daysOff();
          size_t pos = 0;
          while ((pos = linea.find(',')) != string::npos)
          {
            string token = linea.substr(0,pos);
            linea.erase(0, pos+1);
            if (daysAux.id.empty())
            {
              daysAux.id = token;
            }
            else
            {
              daysAux.days.push_back(stoi(token));
            }
          }
          instancia.daysOffData.push_back(daysAux);
        }
        
    }else if (linea.compare(0, 25, "SECTION_SHIFT_ON_REQUESTS") == 0)
    {
      for (int i = 0; i < 2; i++)
      {
        getline(file,linea);
      }
      while (linea[0] != '\r')
      {
        getline(file,linea);
        srAux = ShiftRequest();
        size_t pos = 0;
        int tokenCount = 0;
        while ((pos = linea.find(',')) != string::npos)
        {
          string token = linea.substr(0,pos);
          linea.erase(0, pos +1);
          switch (tokenCount)
          {
          case 0:
            srAux.id = token;
            break;
          case 1:
            srAux.day = stoi(token);
            break;
          case 2:
            srAux.shift = token;
            break;
          case 3:
            srAux.weight = stoi(token);
            break; 
          default:
            break;
          }
          tokenCount ++;
        }
        instancia.shiftRequests.push_back(srAux);
      }
      
    } else if ((linea.compare(0, 26, "SECTION_SHIFT_OFF_REQUESTS") == 0))
    {
      for (int i = 0; i < 2; i++)
      {
        getline(file, linea);
      }
      while (linea[0] != '\r')
      {
        getline(file,linea);
        sorAux = ShiftOffRequest();
        size_t pos = 0;
        int tokenCount = 0;
        while ((pos = linea.find(',')) != string::npos)
        {
          string token = linea.substr(0,pos);
          linea.erase(0, pos+1);
          switch (tokenCount)
          {
          case 0:
            sorAux.id = token;
            break;
          case 1:
            sorAux.day = stoi(token);
            break;
          case 2:
            sorAux.shift = token;
          case 3:
            sorAux.weight = stoi(token);
            break;
          default:
            break;
          }
          tokenCount++;
        }
        instancia.shiftOffRequests.push_back(sorAux);
      }
      
      
    }else if (linea.compare(0, 14, "SECTION_SHIFTS") == 0)
    {
      for (int i = 0; i < 2; i++)
      {
        getline(file,linea);
      }
      while (linea[0] != '\r')
      {
        getline(file,linea);
        shifAux = shift();
        size_t pos = 0;
        int tokenCount = 0;
        while ((pos = linea.find(',') != string::npos))
        {
          string token = linea.substr(0,pos);
          linea.erase(0, pos+1);
          switch (tokenCount)
          {
          case 0:
            shifAux.id = token;
            break;
          case 1:
            shifAux.duracion = stoi(token);
            break;
          case 2:
            shifAux.shifts = token;
            break;
          default:
            break;
          }
          tokenCount++;
        }
        instancia.shifts.push_back(shifAux);
      }
      
      
    }else if (linea.compare(0, 13, "SECTION_COVER") == 0)
    {
      getline(file,linea);
      while (linea[0] != '\r')
      {
        scAux = SectionCover();
        size_t pos = 0;
        int tokenCount = 0;
        while ((pos = linea.find(',')) != string::npos)
        {
          string token = linea.substr(0,pos);
          linea.erase(0,pos+1);
          switch (tokenCount)
          {
          case 0:
            scAux.day = stoi(token);
            break;
          case 1:
            scAux.shift = token;
            break;
          case 2:
            scAux.requiredEmployees = stoi(token);
            break;
          case 3:
            scAux.lowWeight = stoi(token);
            break;
          case 4:
            scAux.highWeight = stoi(token);
          default:
            break;
          }
          tokenCount++;
        }
        instancia.sectionCover.push_back(scAux);
        getline(file,linea);
      }
      
    }
                
    }
    
    }
    file.close();
  }

//! Función de evaluación del SSP
/**
  * La función evalúa una solución para una instancia determinada calculando la violación de varios
  * restricciones y devolución de un valor que representa la gravedad de las infracciones.
  *
  * @param instancia El parámetro "instancia" es de tipo "Instancia" y representa la instancia del
  * problema. Contiene información sobre el personal, turnos, días, solicitudes de turno, solicitudes de turno libre,
  * y requisitos de cobertura de la sección.
  * @param solucion El parámetro `solucion` es un objeto de tipo `Solución`. Representa una posible
  * solución al problema. La función `evaluar_solucion` evalúa la calidad de esta solución mediante
  * calcular un valor numérico basado en las violaciones de varias restricciones.
  *
  * @return un valor entero.
  */
int evaluar_solucion(const Instance &instancia, const Solution &solucion) {
  int valor = 0;

  // Violación de las restricciones de turnos incompatibles
  for (int empleado = 0; empleado < instancia.staff.size(); empleado++) {
    for (int turno = 0; turno < instancia.shifts.size(); turno++) {
      for (int dia = 0; dia < instancia.days; dia++) {
        if (solucion.trabajoHoy[empleado] && turno == stoi(instancia.shifts[solucion.turnoDiaAnterior[empleado]].id)) {
          for (int turno_incompatible : instancia.shifts[turno].shifts) {
            if (solucion.trabajoHoy[empleado] && turno_incompatible == stoi(instancia.shifts[solucion.turnoDiaAnterior[empleado]].id)) {
              valor += 1;
            }
          }
        }
      }
    }
  }

  // Violación de las restricciones de solicitudes de turno
  for (int i = 0; i < instancia.shiftRequests.size(); i++) {
    if (solucion.trabajoHoy[instancia.shiftRequests[i].id[0]] && stoi(instancia.shiftRequests[i].shift) != solucion.turnoDiaAnterior[instancia.shiftRequests[i].id[0]]) {
      valor += instancia.shiftRequests[i].weight;
    }
  }

  // Violación de las restricciones de solicitudes de turno libre
  for (int i = 0; i < instancia.shiftOffRequests.size(); i++) {
    if (solucion.trabajoHoy[instancia.shiftOffRequests[i].id[0]] && stoi(instancia.shiftOffRequests[i].shift) == stoi(to_string(solucion.turnoDiaAnterior[instancia.shiftOffRequests[i].id[0]]))) {
      valor += instancia.shiftOffRequests[i].weight;
    }
  }

  // Violación de las restricciones de cobertura
  for (int i = 0; i < instancia.sectionCover.size(); i++) {
    int empleados_asignados = 0;
    for (int empleado = 0; empleado < instancia.staff.size(); empleado++) {
      if (solucion.trabajoHoy[empleado] && instancia.sectionCover[i].shift == instancia.shifts[solucion.turnoDiaAnterior[empleado]].id) {
        empleados_asignados++;
      }
    }
    if (empleados_asignados < instancia.sectionCover[i].requiredEmployees) {
      valor += instancia.sectionCover[i].lowWeight;
    } else if (empleados_asignados > instancia.sectionCover[i].requiredEmployees) {
      valor += instancia.sectionCover[i].highWeight;
    }
  }

  return valor;
}



//! Función miope del greedy
/**
  * La función `greedy_move` encuentra el mejor movimiento posible para un empleado en un caso determinado y
  * solución evaluando todos los movimientos posibles y seleccionando el de mayor valor.
  *
  * @param instancia El parámetro "instancia" es un objeto de tipo "Instancia" que contiene
  * información sobre la instancia del problema. Probablemente incluya detalles como la cantidad de personal.
  * integrantes, el número de turnos, el número de días, y cualquier otro dato relevante.
  * @param solucion El parámetro "solucion" es de tipo Solución, el cual representa una posible solución
  * a la instancia del problema dada. Contiene información sobre las asignaciones de turnos a los empleados.
  * para cada día.
  *
  * @return un par de números enteros, donde el primer número entero representa el mejor empleado para la mudanza y
  * el segundo número entero representa el valor del movimiento.
  */
pair<int, int> greedy_move(const Instance &instancia, const Solution &solucion) {
  int mejor_movimiento = -1;
  int mejor_valor = -1e9;

  for (int empleado = 0; empleado < instancia.staff.size(); empleado++) {
    for (int turno = 0; turno < instancia.shifts.size(); turno++) {
      for (int dia = 0; dia < instancia.days; dia++) {
        if (!solucion.trabajoHoy[empleado] && solucion.turnoDiaAnterior[empleado] != turno) {
          int nuevo_valor = solucion.funcEval + evaluar_movimiento(instancia, solucion, empleado, turno, dia);
          if (nuevo_valor > mejor_valor) {
            mejor_movimiento = empleado;
            mejor_valor = nuevo_valor;
          }
        }
      }
    }
  }

  return make_pair(mejor_movimiento, mejor_valor);
}

//! Algoritmo greedy para resolver el SSP
/**
  * La función "codicioso" implementa un algoritmo codicioso para encontrar una solución a un caso de problema determinado.
  *
  * @param instancia El parámetro "instancia" es una instancia de un problema que necesita ser resuelto. Él
  * podría ser cualquier estructura de datos u objeto que represente el problema y contenga todo lo necesario
  *información para solucionarlo.
  *
  * @return un objeto Solución.
  */
Solution greedy(const Instance &instancia) {
  Solution solucion(instancia);

  while (true) {
    pair<int, int> movimiento = greedy_move(instancia, solucion);
    if (movimiento.first == -1) {
      break;
    }

    solucion.aplicar_movimiento(movimiento.first, movimiento.second);
  }

  return solucion;
}



int main(int argc, char const *argv[])
{
  Instance inst;
  string path = "Instancias/Instance1.txt";
  Instant_Reader(path,inst);
  cout << "Días en la instancia: " << inst.days << endl;
  return 0;
}


