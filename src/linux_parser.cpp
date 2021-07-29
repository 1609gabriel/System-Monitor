#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>


#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization

/* Memory Utilization() based on the formula --> Total used memory = MemTotal - MemFree/ MemTotal
more information concerning memory utilization 
see https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290from */

float LinuxParser::MemoryUtilization() { 
  string line, value, key, memTot, memFree; 
  float memUtil, memFreeVal, memTotalVal;
  std::ifstream memstream(kProcDirectory + kMeminfoFilename);
  if(memstream.is_open()){
    while(std::getline(memstream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream (line);
      stream >> key >> value;
        if(key == "MemTotal"){ 
          memTotalVal= stof(value);
          }
        if(key == "MemFree"){
          memFreeVal= stof(value);
          }   
    memUtil= (memTotalVal- memFreeVal)/memTotalVal;
    }
  }
  return memUtil;
}
   
// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, value, key;
  long time_sys, time_idle;
  std::ifstream ustream (kProcDirectory + kUptimeFilename);
  if(ustream.is_open()){  
    while(std::getline(ustream, line)){
      std::istringstream linestream(line);
      linestream >> time_sys >> time_idle;
    } 
  }
  return time_sys; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long numSystTicks;  //number of ticks the system made since the last boot
  long sysTime = UpTime();
  long sysFrq= sysconf(_SC_CLK_TCK);  /*system frequency which is the number of ticks per second 
                                  (you can get it using the following function ( sysconf(_SC_CLK_TCK) ).
                                  https://knowledge.udacity.com/questions/129844*/

  numSystTicks= sysTime* sysFrq;
  return numSystTicks;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::vector<string> jif= CpuUtilization();
  long a, b, c, d, e, f, g, h;
 
  if((jif[CPUStates::kUser_]).empty()) {
    a=0;
  } else a= stol(jif[CPUStates::kUser_]);

  if((jif[CPUStates::kNice_]).empty()) {
    b=0; 
  } else b= stol(jif[CPUStates::kNice_]); 

  if((jif[CPUStates::kSystem_]).empty()) {
    c=0;
  } else c= stol(jif[CPUStates::kSystem_]);

  if((jif[CPUStates::kIRQ_]).empty()) {
   d=0; 
  } else d= stol(jif[CPUStates::kIRQ_]);

  if ((jif[CPUStates::kSoftIRQ_]).empty()) {
    e=0;
  } else e= stol(jif[CPUStates::kSoftIRQ_]);

  if((jif[CPUStates::kSteal_]).empty())  {
    f=0;
  } else f= stol(jif[CPUStates::kSteal_]);

  if((jif[CPUStates::kGuest_]).empty()){
    g=0;
  } else g= stol(jif[CPUStates::kGuest_]);

  if((jif[CPUStates::kGuestNice_]).empty()) {
   h=0;
  } else h= stol(jif[CPUStates::kGuestNice_]);

  return a + b + c + d + e + f + g + h;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::vector<string>cpu= CpuUtilization();
  long i, j;

  if((cpu[CPUStates::kIdle_]).empty()){
    i=0;
  } else i= stol(cpu[CPUStates::kIdle_]);
  
 if((cpu[CPUStates::kIOwait_]).empty()){
    j=0;
  }else j= stol(cpu[CPUStates::kIOwait_]);

  return i + j;  
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string>cpu;
  string key, value, line, n;
  std::ifstream CPUstream(kProcDirectory + kStatFilename);
  if(CPUstream.is_open()){
    std::getline(CPUstream, line);
    std::istringstream linestream(line);
    while(linestream >> value){      
      cpu.push_back(value);
    }
    cpu[0].erase();
  }
  return cpu; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, val, key;
  int totalProcNum;
  std::ifstream stream (kProcDirectory + kStatFilename);
  if(stream.is_open()){
      while(std::getline(stream, line)){
        std::istringstream totstream(line); 
        totstream >> key >> val;
        if(key== "processes"){
          totalProcNum = stoi(val);
          return totalProcNum;
          }
      }
  }
  return totalProcNum; 
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses(){  
  string line, val, key;
  int runProcNum;
  std::ifstream runstream (kProcDirectory + kStatFilename);
  if(runstream.is_open()){
    while(std::getline(runstream, line)){
      std::istringstream runstream(line);
      runstream >> key >> val;
      if(key == "procs_running"){
          runProcNum = stoi(val);
          return runProcNum;
        }
      }
    }
 return runProcNum; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; 
  }
  
