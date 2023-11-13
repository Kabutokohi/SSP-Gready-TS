#ifndef File_RW
#define File_RW

//! Librerias utilizadas
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

//! Declaracion de los struct


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
struct shift;

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
struct empleyee;

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
struct daysOff;

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
struct ShiftRequest;

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
struct ShiftOffRequest;

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
struct SectionCover;

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
struct Instance;

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
struct Solution;

//! Declaracion de las funciones

/**
  * La función "print_Horizonte" imprime el número de días en el objeto "inst".
  *
  * @param inst El parámetro "inst" es de tipo "Instancia".
  */
void print_Horizonte(Instance inst);


/**
  * La función "print_shifts" imprime los valores de la estructura de datos "shifts" en la instancia "inst".
  *
  * @param inst El parámetro "inst" es de tipo "Instancia".
  */
void print_shifts(Instance inst);


/**
  * La función "print_staff" itera a través de un vector de empleados e imprime su información.
  *
  * @param inst El parámetro "inst" es una instancia de una clase o estructura que contiene un vector llamado
  * "personal". Este vector contiene objetos que representan empleados, cada uno con varios atributos, como
  *DNI, número de turnos, minutos máximo y mínimo, turnos consecutivos máximo y mínimo, mínimo
  * días libres consecutivos, y máximo fines de semana.
  */
void print_staff(Instance inst);


/**
  * La función "print_DaysOff" imprime los valores en la estructura de datos "inst.daysOffData", que
  * contiene información sobre días libres para diferentes identificaciones.
  *
  * @param inst El parámetro "inst" es de tipo "Instancia".
  */
void print_DaysOff(Instance inst);


/**
  * La función "print_SectionCover" imprime los detalles de las portadas de las secciones, incluido el día, turno,
  * empleados requeridos y peso para cobertura insuficiente y excesiva.
  *
  * @param inst El parámetro "inst" es de tipo "Instancia", que es un tipo de datos definido por el usuario. Es
  * pasado por referencia a la función "print_SectionCover".
  */
void print_SectionCover(const Instance& inst);


/**
  * La función imprime las solicitudes de turno, incluyendo su ID, día, turno y peso.
  *
  * @param inst El parámetro `inst` es de tipo `Instancia`, que es una estructura de datos definida por el usuario. Él
  * probablemente contenga información sobre solicitudes de turno, como su identificación, día, turno y peso.
  */
void print_ShiftRequests(const Instance& inst);

/**
  * La función imprime las solicitudes de cambio de marcha de una instancia.
  *
  * @param inst El parámetro "inst" es de tipo "Instancia", que es una estructura de datos definida por el usuario. Él
  * probablemente contenga información sobre las solicitudes de turnos libres, como su identificación, día, turno y peso.
  */
void print_ShiftOffRequests(const Instance& inst);

/**
  * La función `Instant_Reader` lee datos de un archivo y devuelve un objeto `Instance` que contiene el
  * datos analizados.
  *
  * @param ruta El parámetro ruta es una cadena que representa la ruta del archivo de entrada que
  * contiene los datos a leer.
  *
  * @return una instancia de la estructura "Instancia".
  */
Instance Instant_Reader(const std::string& path);

#endif //File_RW