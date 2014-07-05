
#include "TimerUtils.hpp"

#include <string>

namespace utility 
{

void Timer::start()
{
  stopped = 0; // reset stop flag
#ifdef WIN32
  QueryPerformanceCounter(&startCount);
#else
  gettimeofday(&startCount, NULL);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// stop the timer.
// endCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer::stop()
{
  stopped = 1; // set timer stopped flag

#ifdef WIN32
  QueryPerformanceCounter(&endCount);
#else
  gettimeofday(&endCount, NULL);
#endif
}


///////////////////////////////////////////////////////////////////////////////
// compute elapsed time in micro-second resolution.
// other getElapsedTime will call this first, then convert to correspond resolution.
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMicroSec()
{
#ifdef WIN32
  if(!stopped)
    QueryPerformanceCounter(&endCount);

  startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
  endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
  if(!stopped)
    gettimeofday(&endCount, NULL);

  startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
  endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

  return endTimeInMicroSec - startTimeInMicroSec;
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMilliSec()
{
  return this->getElapsedTimeInMicroSec() * 0.001;
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000000
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInSec()
{
  return this->getElapsedTimeInMicroSec() * 0.000001;
}

///////////////////////////////////////////////////////////////////////////////
// same as getElapsedTimeInSec()
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTime()
{
  return this->getElapsedTimeInSec();
}

void Timer::restart()
{
  stop();
  start();
}

std::string Timer::display() { 
  double seconds = getElapsedTimeInSec();
  return timeDisplay(seconds);
}
 
}
