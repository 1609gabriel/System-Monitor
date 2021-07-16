#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
string System::Kernel() { 
    string a= LinuxParser::Kernel();
    return a; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// TODO: Return the operating system name
string System::OperatingSystem() { 
    string b= LinuxParser::OperatingSystem();
    return b; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    int c= LinuxParser::RunningProcesses();
    return c; 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long System::UpTime() { 
    float t= LinuxParser::UpTime();
    return t; 
}