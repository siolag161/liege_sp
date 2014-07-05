/*********************************************************************
 * File: EnumUtils.hpp
 * Description: temporary groups helper method related to enum manipulation
 * @date: 18/10/2013
 * @author: Song Ho Ahn (song.ahn@gmail.com)
 *********************************************************************/

#ifndef LATENT_TREE_MODEL_TIMER__HPP
#define LATENT_TREE_MODEL_TIMER__HPP


#ifdef WIN32   // Windows system specific
#include <windows.h>
#else      // Unix based system specific
#include <sys/time.h>
#endif
#include <sstream>

#include <stdlib.h>
#include <string>

namespace utility 
{

class Timer
{
 public:
  Timer() {
#ifdef WIN32
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;
#else
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
#endif
    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
  }
  // default constructor
  ~Timer() {}                                   // default destructor
  
   void start();                             // start timer
   void stop();                              // stop the timer
   double getElapsedTime();                    // get elapsed time in second
   double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
   double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
   double getElapsedTimeInMicroSec();          // get elapsed time in micro-second
   void restart();

  std::string display();
  
 protected:
 private:
  double startTimeInMicroSec;                 // starting time in micro-second
  double endTimeInMicroSec;                   // ending time in micro-second
  int    stopped;                             // stop flag 
#ifdef WIN32
  LARGE_INTEGER frequency;                    // ticks per second
  LARGE_INTEGER startCount;                   //
  LARGE_INTEGER endCount; 

  //
#else
  timeval startCount;                         //
  timeval endCount;                           //
#endif
};


inline std::string timeDisplay( double seconds ) {
  std::ostringstream result;
  long secs = seconds / (long)1;
  int hours = (secs / 3600);
  int minutes = (secs / 60) % 60;
  int s = secs % 60;

  if ( secs < 60 )  {
    result << seconds << " (seconds).";

  } else if (secs < 3600) {
    result << minutes << " (minutes) " << s << " (seconds).";

  } else {
    result << hours << " (hours), " << minutes << " (minutes) " << s << " (seconds)";
  }  

  return result.str();
}


} //namespace internal_tools


#endif // LATENT_TREE_MODEL_ENUMUTILS_HPP
