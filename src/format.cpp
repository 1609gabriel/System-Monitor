#include <string>
#include "format.h"
#include "linux_parser.h"
#include <iomanip>  //setfill, setw

using std::string;
using std::stringstream;     //str
using std::to_string;
using std::setfill;
using std::setw;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) { 
   stringstream timeFormat;

   long hour=times/3600;
   long min=(times%3600)/60;
   long sec=(times%3600)%60;

   string hourS= to_string(hour);
   string minS= to_string(min);
   string secS= to_string(sec);
   
   timeFormat << setfill('0') << setw(2) << hour <<':'    /*https://stackoverflow.com/questions/48310746/converting-time-into-hours-minutes-and-seconds*/
              << setfill('0') << setw(2) << min << ':' 
              << setfill('0') << setw(2) << sec;

   string sysTime=timeFormat.str(); 
   return sysTime;
}