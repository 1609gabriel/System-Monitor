#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include<iomanip>

#include "process.h"
#include "linux_parser.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid){          //constructor
    long totalTime;
    long startTime;
    string ram;
    pid_=pid;
    command_=LinuxParser::Command(pid);
    ram= LinuxParser::Ram(pid); 
    if(ram.empty()==true){
        ram_=0;
    } else {
        ram_=stof(ram);      
    }
    user_=LinuxParser::User(pid);
    upTime_=LinuxParser::UpTime(pid);

    //details see https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    totalTime= LinuxParser::ActiveJiffies(pid);
    startTime= LinuxParser::UpTime(pid);

    if(startTime==0){
        cpu_= totalTime/sysconf(_SC_CLK_TCK);       //cpu value expressed in %, multiplying by 100 already done in ncurses_display.cpp (line no.75)
    } else if (totalTime==0){
       cpu_= sysconf(_SC_CLK_TCK)/startTime;  
    } else {
       cpu_= totalTime/sysconf(_SC_CLK_TCK)/startTime; 
    }
}

// TODO: Return this process's ID
int Process::Pid(){
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    return cpu_; 
}

// TODO: Return the command that generated this process 
string Process::Command() { 
    string comm=LinuxParser::Command(Pid());
    if(comm.size()>40){
        command_=comm.substr(0,40);
    }
    return command_; 
}

// TODO: Return this process's memory utilization  
string Process::Ram() { 
    float ramVal;
    string ram;
    string ramm;
    string ram_;
    ram=LinuxParser::Ram(Pid());
    ramVal= stof(ram);
    ramVal= ramVal/1024;          //converting the memory utilization from kilobyte to megabytes
    ramm= to_string(ramVal);
    ram_=ramm.substr(0,5);
    return ram_; 
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    string user_ = LinuxParser::User(Pid());
    return user_;
}

// TODO: Return the age of this process (in seconds) 
long int Process::UpTime() { 
    return upTime_; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
   return this ->cpu_ > a.cpu_;                 /*the left part of the operation is referring the object itself*/
}

