//  ###########################################################################
//  Copyright 2011-2015, 2024 by Raymond Wan (rwan.work@gmail.com)
//    https://github.com/rwanwork/QScores-Archiver
//
//  This file is part of QScores-Archiver.
//
//  QScores-Archiver is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public License
//  as published by the Free Software Foundation; either version
//  3 of the License, or (at your option) any later version.
//
//  QScores-Archiver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with QScores-Archiver; if not, see
//  <http://www.gnu.org/licenses/>.
//  ###########################################################################


/*******************************************************************/
/*!
    \file sort.cpp
    Header file for sorting functions for Huffman coding class definition .
*/
/*******************************************************************/

#ifndef SORT_HPP
#define SORT_HPP

class SortByFrequency {
  public:
    Huffman *huffman;
    SortByFrequency (Huffman *x) : huffman (x) {}
    
    bool operator () (const unsigned int lhs, const unsigned int rhs) {
      if (huffman -> GetTableValue (lhs) == huffman -> GetTableValue (rhs)) {
        return (lhs < rhs);
      }
      else {
        return ((huffman -> GetTableValue (lhs)) > (huffman -> GetTableValue (rhs)));
      }
    }
};

#endif

