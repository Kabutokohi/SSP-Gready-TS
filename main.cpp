#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

struct ShiftSchedulingData {
  int horizonLength;
  std::vector<int> shiftIds;
  std::vector<EmployeeData> staff;
  std::vector<std::pair<int, int>> daysOff;
  std::vector<ShiftOnRequest> shiftOnRequests;
  std::vector<ShiftOffRequest> shiftOffRequests;
  std::vector<ShiftCoverage> cover;
};

struct EmployeeData {
  int id;
  int maxShifts;
  int maxTotalMinutes;
  int minTotalMinutes;
  int maxConsecutiveShifts;
  int minConsecutiveShifts;
  int minConsecutiveDaysOff;
  int maxWeekends;
};

struct ShiftOnRequest {
  int employeeId;
  int day;
  int shiftId;
  int weight;
};

struct ShiftOffRequest {
  int employeeId;
  int day;
  int shiftId;
  int weight;
};

struct ShiftCoverage {
  int day;
  int shiftId;
  int requirement;
  int underWeight;
  int overWeight;
};

ShiftSchedulingData readShiftSchedulingData(std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    exit(1);
  }

  ShiftSchedulingData data;

  // Read the horizon length.
  file >> data.horizonLength;

  // Read the shift IDs.
  std::string line;
  std::getline(file, line);
  while (std::getline(file, line)) {
    int shiftId;
    std::stringstream ss(line);
    ss >> shiftId;
    data.shiftIds.push_back(shiftId);
  }

  // Read the staff data.
  while (std::getline(file, line)) {
    EmployeeData employee;
    std::stringstream ss(line);
    ss >> employee.id;
    ss >> employee.maxShifts;
    ss >> employee.maxTotalMinutes;
    ss >> employee.minTotalMinutes;
    ss >> employee.maxConsecutiveShifts;
    ss >> employee.minConsecutiveShifts;
    ss >> employee.minConsecutiveDaysOff;
    ss >> employee.maxWeekends;
    data.staff.push_back(employee);
  }

  // Read the days off data.
  while (std::getline(file, line)) {
    int employeeId;
    int dayIndex;
    std::stringstream ss(line);
    ss >> employeeId;
    ss >> dayIndex;
    data.daysOff.emplace_back(employeeId, dayIndex);
  }

  // Read the shift on request data.
  while (std::getline(file, line)) {
    ShiftOnRequest request;
    std::stringstream ss(line);
    ss >> request.employeeId;
    ss >> request.day;
    ss >> request.shiftId;
    ss >> request.weight;
    data.shiftOnRequests.push_back(request);
  }

  // Read the shift off request data.
  while (std::getline(file, line)) {
    ShiftOffRequest request;
    std::stringstream ss(line);
    ss >> request.employeeId;
    ss >> request.day;
    ss >> request.shiftId;
    ss >> request.weight;
    data.shiftOffRequests.push_back(request);
  }

  // Read the shift coverage data.
  while (std::getline(file, line)) {
    ShiftCoverage coverage;
    std::stringstream ss(line);
    ss >> coverage.day;
    ss >> coverage.shiftId;
    ss >> coverage.requirement;
    ss >> coverage.underWeight;
    ss >> coverage.overWeight;
    data.cover.push_back(coverage);
  }

  file.close();

  return data;
}

int main(int argc, char const *argv[])
{
    ShiftSchedulingData informacion;
    informacion = readShiftSchedulingData("Instancias/Instance1.txt");
    return 0;
}
