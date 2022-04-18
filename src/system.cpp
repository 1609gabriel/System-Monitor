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

// Return the system's CPU - already implemented
Processor& System::Cpu() { 
    return cpu_; }

// Return a container composed of the system's processes

    vector<Process>& System::Processes(){
        const vector<int>&pids=LinuxParser::Pids();
        for(const int& pid_:pids){
            Process new_process(pid_);
            if(new_process.Command()==""|| new_process.Ram()==""){
                continue;
            }else {
                processes_.emplace_back(new_process);
            }
        }
        return processes_;
    }

// Return the system's kernel identifier (string)
string System::Kernel() { 
    string ke= LinuxParser::Kernel();
    return ke; 
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    float mu= LinuxParser::MemoryUtilization();
    return mu; 
}

// Return the operating system name
string System::OperatingSystem() { 
    string os= LinuxParser::OperatingSystem();
    return os; 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    int rp= LinuxParser::RunningProcesses();
    return rp; 
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    int tp= LinuxParser::TotalProcesses();    
    return tp; 
}

// Return the number of seconds since the system started running
long System::UpTime() { 
    long ut= LinuxParser::UpTime();
    return ut; 
}
/*
long SystemESP32::UpTimeS()const{
    long it = LinuxParser::UpTimeSub();
    return it;
} */