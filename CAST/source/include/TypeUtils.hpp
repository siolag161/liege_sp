/*********************************************************************
 * File: EnumUtils.hpp
 * Description: temporary groups helper method related to enum manipulation
 * @date: 18/10/2013
 * @author: dtphan89 (thanh.phan@outlook.com)
 *********************************************************************/

#ifndef LATENT_TREE_MODEL_ENUMUTILS_HPP
#define LATENT_TREE_MODEL_ENUMUTILS_HPP

#include<sstream> // stringstream
#include<string> // basic_string

namespace utility
{    

/**
 * construct enum from index
 */
template<typename EnumType>
EnumType EnumOfIndex(const int& i); 

/**
 * Integer to Type trick (from Modern C++ Design)
 */
template<int I>
struct Int2Type
{
  enum {value = I};
};

      
///////////////////////// generic from string and to string ///////////////////////////////
/**
 * generic functor to convert the string (normal, wide string)... to T
 */
template<typename CharType, typename ConvertType> 
struct fromString: public std::unary_function<CharType, ConvertType>
{
  ConvertType operator()(const std::basic_string<CharType>& str);
 private:
  std::stringstream ifs;
};


/**
 * handles from string to string 
 */
template<typename CharType> 
struct fromString<CharType, std::string >
    : public std::unary_function<CharType, std::string >
{
  std::string operator()(const std::string& str);
};


/**
 * generic functor to convert toString (normal, wide string)... to T
 */
template<typename CharType, typename ConvertType> 
struct toString: public std::unary_function<ConvertType, std::basic_string<CharType> >
{
  std::basic_string<CharType> operator()(const ConvertType&);
 private:
  std::stringstream ifs;
};


/**
 * handles from string to string 
 */
template<typename CharType> 
struct toString<CharType, std::string >
    : std::unary_function<CharType, std::string >
{
  std::string operator()(const std::string&);

};



}
/************************************************** IMPLEMENTATION **************************************************/

namespace utility
{
   
template<typename EnumType>
EnumType EnumOfIndex(const int& i)
{
  return static_cast<EnumType>(i);
}

////////////////////// FROM STRING AND TO STRING //////////////////////
/**
 * generic functor to convert the string (normal, wide string)... to T
 */
template<typename CharT, typename ConvertT> 
ConvertT fromString<CharT, ConvertT>::operator()(const std::basic_string<CharT>& str)
{
  ConvertT result(0);

  if (!(ifs<<str))
  {
    //throw std::runtime_error("cannot convert")
    ifs.clear();
  }
  if (!(ifs>>result)) 
  {
    //throw std::runtime_error("cannot convert")
    ifs.clear();
  }
  ifs.clear();
  return result;
}

template<typename CharT> 
std::string fromString<CharT, std::string >::operator()
    (const std::string& str)
{
  return str;
} 

//////////////////////////////////////////////////////////////////
template<typename CharT, typename ConvertT> 
std::basic_string<CharT> toString<CharT, ConvertT>::operator()(const ConvertT& t)
{
  std::basic_string<CharT> result;

  if (!(ifs<<t))
  {
    //throw std::runtime_error("cannot convert")
    ifs.clear();
  }
  if (!(ifs>>result)) 
  {
    //throw std::runtime_error("cannot convert")
    ifs.clear();
  }
  ifs.clear();
  return result;
}

template<typename CharT> 
std::string toString<CharT, std::string >::operator()
    (const std::string& str)
{
  return str;
}

}

#endif // LATENT_TREE_MODEL_ENUMUTILS_HPP
