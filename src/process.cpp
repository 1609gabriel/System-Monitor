#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid){          //constructor
    long totalTime, startTime, upTime;
    string ram;
    pid_=pid;
    command_=LinuxParser::Command(pid);
    ram= LinuxParser::Ram(pid); 
    if(ram.empty()==true){
        ram_=0.;
    } else {
        long rm=stol(ram);
        ram_=rm*0.001;      //converting the memory utilization into megabytes
    }
    user_=LinuxParser::User(pid);
    upTime_=LinuxParser::UpTime(pid);

    //details see https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    upTime = LinuxParser::UpTime();
    totalTime= LinuxParser::ActiveJiffies(pid);
    startTime= LinuxParser::UpTime(pid);
    
    cpu_=100*(totalTime/(upTime-startTime));
}

// TODO: Return this process's ID
int Process::Pid(){
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    return cpu_; 
}

// TODO: Return the command that generated this process ok
string Process::Command() { 
    string command_=LinuxParser::Command(Pid());
    return command_; 
}

// TODO: Return this process's memory utilization  ok
string Process::Ram() { 
    string ram_=LinuxParser::Ram(Pid());
    return ram_; 
}

// TODO: Return the user (name) that generated this process ok
string Process::User() {
    string user_ = LinuxParser::User(Pid());
    return user_;
}

// TODO: Return the age of this process (in seconds) ok
long int Process::UpTime() { 
    return upTime_; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
   return this ->cpu_ > a.cpu_;                 /*the left part of the operation is referring the object itself*/
}

