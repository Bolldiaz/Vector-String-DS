#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include "vl_vector.h"
#include <cstring>


#define DEFAULT_STR_SIZE 1
#define DEFAULT_STR_SIZE 1
#define STRING_INIT_CHAR '\0'

template<size_t CAP = DEFAULT_STAT_CAP>
class vl_string : public vl_vector<char, CAP>
{
 public:
  typedef vl_vector<char, CAP> this_vec;
  
  /**
   * default Ctor
   */
  vl_string () : this_vec ()
  {
    this_vec::_cur[0] = STRING_INIT_CHAR;
    this_vec::_size = DEFAULT_STR_SIZE;
  }

  /**
   * copy Ctor
   * @param oth another vl_string object
   */
  vl_string (const vl_string &oth) : vl_vector<char, CAP> (oth)
  {}

  /**
   * implicit Ctor
   * @param c points the begining of a string
   */
  vl_string(const char *c) : vl_vector<char, CAP> (c, c + std::strlen (c) + 1)
  {}

  /**
   * * @return the length of the vl_string
   */
  size_t size () const
  { return this_vec::_size - 1; }

  /**
   * clear the vl_string from all it's elements
   */
  void clear ()
  {
    delete[] this_vec::_dyn;
    this_vec::_size = DEFAULT_STR_SIZE;
    this_vec::_DynamicCapacity = DEFAULT_DYN_CAP;
    this_vec::_dyn = nullptr;
    this_vec::_cur = this_vec::_stat;
    this_vec::_cur[0] = STRING_INIT_CHAR;
  }

  /**
   * check if buf is a substring of the array of vl_string
   * @param buf the substring
   * @return True if is, else false
   */
  bool contains (const char* buf) const
  {
    size_t len = std::strlen(buf);
    if (!len)
      return true;
    auto start =  this_vec::begin();
    auto last = this_vec::end();
    auto res = std::find_end(start, last, buf, buf + len);
    return last != res;
  }

  // operators:
  /**
   * += operator concatenate the given char to the vl_string
   * @param c the char to concatenate
   * @return this = this + char
   */
  vl_string<CAP>& operator+= (const char c)
  {
    this_vec::push_back(c);
    return *this;
  }

  /**
   * += operator calculate the concatenation of the array and the given string
   * @param buf the string to concatenate
   * @return this = this + buf
   */
  vl_string<CAP>& operator+= (const char* buf)
  {
    size_t len = std::strlen(buf);
    this_vec::insert(this_vec::begin() + this_vec::size() , buf, buf + len);
    return *this;
  }

  /**
   * += operator concatenate rhs string to this string
   * @param rhs another vl_string object
   * @return this = this + rhs
   */
  vl_string<CAP>& operator+= (const vl_string<CAP>& rhs)
  {
    this_vec::insert(this_vec::begin() + this_vec::size(), rhs.begin(), rhs
    .begin() + rhs.size());
    return *this;
  }

  /**
   * + operator concatenate the given char to the vl_string
   * @param c the char to concatenate
   * @return new vl_string = this + char
   */
  vl_string<CAP> operator+ (const char c) const
  {
    vl_string<CAP> new_str(*this);
    return new_str += c;
  }

  /**
   * + operator calculate the concatenation of the array and the given string
   * @param buf the string to concatenate
   * @return new vl_string = this + buf
   */
  vl_string<CAP> operator+ (const char* buf) const
  {
    vl_string<CAP> new_str(*this);
    return new_str += buf;
  }

  /**
   * + operator concatenate rhs string to this string
   * @param rhs another vl_string object
   * @return new vl_string = this + rhs
   */
  vl_string<CAP> operator+ (const vl_string<CAP>& rhs) const
  {
    vl_string<CAP> new_str(*this);
    return new_str += rhs;
  }

  /**
   * = operator
   * @param rhs another vl_string object
   * @return reference to this
   */
  vl_string<CAP>& operator= (const vl_string<CAP>& rhs)
  {
    if (this == &rhs)
      return *this;
    clear();
    this_vec::insert(this_vec::begin() ,rhs.begin(), rhs.begin() + rhs.size());
    return *this;
  }

  /**
   * implicit casting to const char*
   * @return pointer to the start of the string
   */
  operator const char* () const
  {
    return this_vec::data();
  }

};

#endif //_VL_STRING_H
