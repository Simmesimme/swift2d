////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STL_HELPERS_HPP
#define SWIFT2D_STL_HELPERS_HPP

// includes  -------------------------------------------------------------------
#include <vector>
#include <iostream>
#include <sstream>

#include <raknet/RakString.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T clamp(T val, T a, T b) {
  return std::min(std::max(val, a<b?a:b), a<b?b:a);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T from_string(std::string const& v) {
  std::istringstream iss(v);
  T result;
  iss >> result;
  return result;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::string to_string(T const& v) {
  std::ostringstream oss;
  oss << v;
  return oss.str();
}

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> split_string(std::string const& s, char delim);
bool string_contains(std::string const& s, char c);

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace RakNet {

std::ostream& operator<<(std::ostream& os, RakString const& v);
std::istream& operator>>(std::istream& is, RakString& v);

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace std {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {

  typename std::vector<T>::const_iterator i(v.begin());
  while (i != v.end()) {
      os << *i;

      if (++i != v.end()) {
        os << " ";
      }
  }
  return os;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
  v.clear();

  T new_one;
  while (is >> new_one) {
    v.push_back(new_one);
  }

  is.clear();

  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
#endif //SWIFT2D_STL_HELPERS_HPP
