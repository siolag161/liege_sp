/****************************************************************************************
 * File: CSVParser.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 25/03/2014

 ***************************************************************************************/
#ifndef GRAPH_CSVPARSER_HPP
#define GRAPH_CSVPARSER_HPP

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace utility
{

template<class T>
class CSVRow
{
 public:
  inline T const& operator[](std::size_t index) const
  {
    return m_data[index];
  }

  inline T const at(std::size_t index) const
  {
    return m_data[index];
  }
  
  std::size_t size() const
  {
    return m_data.size();
  }

  inline void readNextRow(std::istream& str)
  {
    std::string line;
    std::getline(str,line);

    std::stringstream lineStream(line);
    T cell;

    m_data.clear();
    while (lineStream >> cell)
    {
      m_data.push_back(cell);
      if ( lineStream.peek() == ',')
        lineStream.ignore();
    }
    
  }
 private:
  std::vector<T> m_data;
};

template< typename T>
inline std::istream& operator>>(std::istream& str, CSVRow<T>& data)
{
  data.readNextRow(str);
  return str;
}

template<typename T>
class CSVIterator
{    
 public: 
  typedef std::input_iterator_tag iterator_category;
  typedef CSVRow<T> value_type;
  typedef std::size_t difference_type;
  typedef CSVRow<T>* pointer;
  typedef CSVRow<T>& reference;

  CSVIterator(std::istream& str): m_str(str.good()?&str:NULL) { ++(*this); }
  CSVIterator(): m_str(NULL) {}

  // Pre Increment
  CSVIterator& operator++() { if (m_str) { (*m_str) >> m_row;m_str = m_str->good()?m_str:NULL;}return *this;}
  // Post increment
  CSVIterator operator++(int) { CSVIterator tmp(*this);++(*this);return tmp;}
  CSVRow<T> const& operator*() const { return m_row;}
  CSVRow<T> const* operator->() const  { return &m_row;}

  bool operator==(CSVIterator const& rhs) { return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));}
  bool operator!=(CSVIterator const& rhs) { return !((*this) == rhs);}
 private:
  std::istream* m_str;
  CSVRow<T> m_row;
};

} // namespace graphends here. graph

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace utility
{



template<>
class CSVRow<std::string>
{
 public:
  inline std::string const& operator[](std::size_t index) const
  {
    return m_data[index];
  }

  inline std::string const at(std::size_t index) const
  {
    return m_data[index];
  }
  
  std::size_t size() const
  {
    return m_data.size();
  }

  inline void readNextRow(std::istream& str)
  {
    std::string line;
    std::getline(str,line);

    std::stringstream lineStream(line);
    std::string cell;

    m_data.clear();
    while(std::getline(lineStream, cell, ','))
    {
      m_data.push_back(cell);
    }
    
  }
 private:
  std::vector<std::string> m_data;
};


} // namespace graphends here. graph

/****************************************************************************************/
#endif // GRAPH_CSVPARSER_HPP
