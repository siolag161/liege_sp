/****************************************************************************************
 * File: chrono_measure.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 13/06/2014

 ***************************************************************************************/
#ifndef TEST_CHRONO_MEASURE_HPP
#define TEST_CHRONO_MEASURE_HPP

#include <chrono>

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
  template<typename F, typename ...Args>
  static typename TimeT::rep execution(F func, Args&&... args)
  {
    auto start = std::chrono::system_clock::now();

    // Now call the function with all the parameters you need.
    func(std::forward<Args>(args)...);

    auto duration = std::chrono::duration_cast< TimeT> 
        (std::chrono::system_clock::now() - start);

    return duration.count();
  }
};

#endif // TEST_CHRONO_MEASURE_HPP
