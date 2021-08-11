#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "format.h"

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
long LinuxParser::ActiveJiffies(int pid) { 
  /*To calculate CPU usage for a specific process see: 
  https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599 */
  
  string line, value;
  vector<string>jif_vector;
  string pidConvert= to_string(pid);
  long totalTime;
  long utime=0;     /*CPU time spent in user code, measured in clock ticks*/
  long stime=0;     /*CPU time spent in kernel code, measured in clock ticks*/
  long cutime=0;    /*Waited-for children's CPU time spent in user code (in clock ticks)*/
  long cstime=0;    /*Waited-for children's CPU time spent in kernel code (in clock ticks)*/

  std::ifstream jifstream(kProcDirectory+pidConvert+kStatFilename);   // cat /proc/[pid]/stat - Linux terminal
  if(jifstream.is_open()){
    std::getline(jifstream, line);
    std::istringstream linestream(line);
    while(linestream >> value){          //see https://man7.org/linux/man-pages/man5/proc.5.html
      jif_vector.push_back(value);       //all strings from stream will be pushed in jif_vector
    }
  if(jif_vector[13].empty()){           
      utime=0;
  } else {utime = stol(jif_vector[13]);}
  if(jif_vector[14].empty()){
      stime=0;
  } else {stime = stol(jif_vector[14]);}
  if(jif_vector[15].empty()){
      cutime=0;
  } else {cutime = stol(jif_vector[15]);}
  if(jif_vector[16].empty()){
      cstime=0;
  } else {cutime = stol(jif_vector[16]);}
  
totalTime= utime + stime + cutime + cstime; 
  }
return totalTime;
}
  

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
string LinuxParser::Command(int pid) {          // [pid] - run ls /proc 
  string line;
  string pidConvert= to_string(pid);
  std::ifstream commStream(kProcDirectory+pidConvert+kCmdlineFilename);
  if(commStream.is_open()){
    std::getline(commStream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function

string LinuxParser::Ram(int pid) { 
  string line, value, key, stringVal, ramVal;
  //long ramValMB;
  string pidConvert= to_string(pid);
  std::ifstream ramStream(kProcDirectory + pidConvert + kStatusFilename);  // cat /proc/[pid]/status , [pid] - run ls /proc 
  if(ramStream.is_open()){
    while(std::getline(ramStream, line)){
      std::replace(line.begin(), line.end(),':',' ');
      std::istringstream linestream(line);
      while(linestream >> key){
        if(key=="VmSize"){
          linestream>>ramVal;
          return ramVal;
        }
      }
    }
  }
return ramVal; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
      string line, key, value;
      string pidConvert= to_string(pid);
      std::ifstream stream (kProcDirectory + pidConvert + kStatFilename);   //cat /proc/[pid]/stat - Linux terminal
      if(stream.is_open()){
        while(std::getline(stream, line)){
          std::replace(line.begin(), line.end(),':',' ');
          std::istringstream uidstream(line);
          while(uidstream >> key >> value){
          if(key == "Uid"){
            return value;
            }
          }
        }
      }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string userID= Uid(pid);
  string line, value{""}, key;
  string user;
  std::ifstream stream (kPasswordPath);
  if(stream.is_open()){
        while(std::getline(stream, line)){
          std::replace(line.begin(),line.end(),':', ' ');
          std::istringstream userstream(line);
           while(userstream >> key >> value){
            if(key == "Uid"){
              return value ;
            }
          }
        } 
  }
  return value;
}

// TODO: Read an`d return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value, time;
  vector<string> my_vector;
  long startTimeTicks;          //The time the process started after system boot
  string pidConvert= to_string(pid);
  std::ifstream jifstream(kProcDirectory+pidConvert+kStatFilename);   //cat /proc/[pid]/stat - Linux terminal
  if(jifstream.is_open()){
    std::getline(jifstream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      my_vector.push_back(value);
    }
    if(my_vector[21].empty()==true){    /* (22) starttime  %llu, since Linux 2.6, the value is expressed in clock ticks - 
                                        see https://man7.org/linux/man-pages/man5/proc.5.html */
    startTimeTicks=0;    
     } else {
    startTimeTicks= stol(my_vector[21]);
    return startTimeTicks;
    }
  }  
return startTimeTicks;
}