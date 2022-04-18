#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

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
   
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

class SystemMicro {      /* base class - cannot be instantiated*/
 public:
  virtual long UpTimeS()const = 0; /* pure virtual function --> interface */
};

class SystemESP32 : public SystemMicro{  /* derived class */
 public:
  long UpTimeS()const override;
};

#endif