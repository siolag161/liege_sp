/****************************************************************************************
 * File: CurrentTime.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 21/04/2014

 ***************************************************************************************/
#ifndef INTERNAL_UTILS_CURRENTTIME_HPP
#define INTERNAL_UTILS_CURRENTTIME_HPP

#include <ctime>
namespace internal_tools
{

static inline long CurrentTimeNS() {
  struct timespec tm;
  clock_gettime( CLOCK_REALTIME, &tm);
  return tm.tv_nsec;
}

}

/****************************************************************************************/
#endif // INTERNAL_UTILS_CURRENTTIME_HPP
