#include <string>
#include <ctime>

#include "format.h"
#include "linux_parser.h"

using std::string;
using std::ifstream;
using std::istringstream;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time_sys) { 
    string line, day_, month_, nb_, time_, year;
    time_t t= time_sys;             /*convert time_sys[sec.] to time_t (inherited from C), 
                                    time_t enables to represents the system time*/
    string time_string= ctime(&t);  //convert time_t value to string(https://www.cplusplus.com/reference/ctime/ctime/)
                                    //output time_string - "Thu Jan 1 05:02:03 1970" */
    istringstream stream(time_string);
    if(stream){
        getline(stream, line);
        istringstream linestream(line);
        linestream >> day_ >> month_ >> nb_ >> time_ >> year;
  }
    return time_; 
}