#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

/*CLEAN CODE HW - SOLID principle no.1 - SINGLE RESPONSIBILITY*/

/*The System class is high level class, which has only one purpose. 
Display a system monitor with al system information such as memory utilization, kernel, oper.system etc. and all processes*/

class System {
 public:
  Processor& Cpu();                  
  std::vector<Process>& Processes(); 
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif