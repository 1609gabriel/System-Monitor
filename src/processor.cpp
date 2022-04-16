#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>

#include"linux_parser.h"

using std::stol;
using std::stof;
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization

float Processor::Utilization() { 
  float totaldFloat; 
  float idledFloat; 
  float utilization;
  long idle;
  long nonIdle; 
  long totald; 
  long idled; 
  long prevTotal; 
  long total;

  utilization=0;
  nonIdle= LinuxParser::ActiveJiffies();
  idle= LinuxParser::IdleJiffies();

  total= idle + nonIdle;
  prevTotal= prevIdle + prevNonidle;

  totald= total - prevTotal;
  idled= idle - prevIdle;

  totaldFloat= totald;
  idledFloat= idled;

  utilization= (totaldFloat - idledFloat)/ totaldFloat; 
  
  prevIdle= idle;
  prevNonidle= nonIdle;

  return utilization;
}

/* bash: cat /proc/stat - Processor Utilization Data 
   cpu (aggregates in all of other "cpuN" lines) 
   user time(1.col.),nice t.(2.col.),system t.(3.col.),idletime(4.col.) + iowait(5.col.),
   irq(6.column) + softirq(7.column) + steal(8.column)*/
