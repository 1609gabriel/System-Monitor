#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;

/* Basic class for Process representation
It contains relevant attributes as shown below */

class Process {
 public:
  Process(int pid);
  int Pid();                               
  string User();                          
  string Command();                        
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
   std::string user_, command_;
   long upTime_;
   double cpu_; 
   float ram_;
   int pid_;
};

#endif