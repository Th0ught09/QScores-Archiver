///////////////////////////////////////////////////////////////////////////
//  QScores-Archiver
//  Software and library for transforming and compressing next generation
//    sequencing data in FASTQ format.
//  
//  Version 1.00 -- November 6, 2011
//  
//  Copyright (C) 2011 by Raymond Wan, All rights reserved.
//  Contact:  rwan@cuhk.edu.hk
//  Organization:  Department of Computational Biology, Graduate School of
//                 Frontier Science, University of Tokyo and
//                 Computational Biology Research Center, AIST,
//                 Japan
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
///////////////////////////////////////////////////////////////////////////


/*******************************************************************/
/*!
    \file parameters-test.cpp
    Member functions of main-test for checking parameters .
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: parameters-test.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>  //  setw
#include <cmath>  //  floor
#include <climits>  //  UINT_MAX

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>   // includes all needed Boost.Filesystem declarations

using namespace std;
using namespace boost;
namespace po = boost::program_options;

using namespace std;
namespace bfs = boost::filesystem;

//  Pull the configuration file in
#include "BitIOConfig.hpp"

#include "test_sequential.hpp"
#include "test_random.hpp"
#include "test_showlengths.hpp"
#include "parameters-test.hpp"


/*!
     Show basic information about the program

     \return The program exit condition
*/
void ShowInfo () {
  cout << sizeof (unsigned int) << endl;
  cout << sizeof (unsigned long long int) << endl;
  cout << sizeof (double) << endl;

  cout << "BitIO version " << BitIO_VERSION_MAJOR << "." << BitIO_VERSION_MINOR << ":  " << __DATE__ <<  " (" << __TIME__ << ")" << endl;

  cerr << "==\tShowInfo successful!" << endl;

  return;
}


/*!
     This function makes use of Boost's program_options for handling
     arguments on the command line and in options in a configuration
     file whose format resembles .ini files.

     Initially, boolean and enumerated values are given default values.
     Then, the available options are set up, with default values for
     string and numeric types.  The description of the options are
     recorded.

     Next, the command line options are read, followed by the configuration
     file options.  The command line options take priority over the
     configuration file ones.  Then, the options are processed, one-by-one.

     All of this is encapsulated within a try...catch block.
*/
bool ProcessOptions (int argc, char *argv[]) {
  bool random = false;
  unsigned int showlengths = UINT_MAX;
  string method = "";
  unsigned int param = 5;
  bool result = false;
  bool save_file = false;

  try {
    //  Options that are allowed only on the command line
    po::options_description program_only ("Program options");
    program_only.add_options()
      ("version,v", "Print version")
      ("help,h", "This help message")
      ("showinfo", "Show simple information.")
      ("random", "Employ random tests.")
      ("showlengths", po::value<int>() -> default_value (UINT_MAX), "Employ tests to show bit lengths up to the value given [Default:  -1, do not run test].")
      ("method", po::value<string> (), "Method to use.  No default; choose from [unary, binary, gamma, delta, golomb, rice].")
      ("param", po::value<unsigned int> (), "Parameter for Golomb/Rice coding.")
      ("savefile", "Save the output file (i.e., do not delete it).")
      ;

    po::options_description cmdline_options;
    cmdline_options.add (program_only);

    po::options_description visible;
    visible.add (program_only);

    po::variables_map vm;
    store (po::command_line_parser (argc, argv).options (cmdline_options).run (), vm);

    if (vm.count ("version")) {
      cout << "BitIO Test version " << BitIO_VERSION_MAJOR << "." << BitIO_VERSION_MINOR << ":  " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
      return false;
    }

    if (vm.count ("help")) {
      cout << visible << endl;
      return (true);
    }

    if (vm.count ("showinfo")) {
      ShowInfo ();
      return (true);
    }
    
    //  Booleans
    if (vm.count ("random")) {
      random = true;
    }

    //  Integers
    if (vm.count ("showlengths")) {
      showlengths = vm["showlengths"].as<int>();
    }
    
    if (vm.count ("param")) {
      param = vm["param"].as<unsigned int>();
    }

    //  Strings
    if (vm.count ("method")) {
      method = vm["method"].as<string>();
    }
    
    if (vm.count ("savefile")) {
      save_file = true;
    }
  }
  catch (std::exception& e) {
    cout << e.what() << "\n";
    return false;
  }

  if (random) {
    if (method == "unary") {
      result = TestUnaryRandom ();
    }
    else if (method == "binary") {
      result = TestBinaryRandom ();
    }
    else if (method == "gamma") {
      result = TestGammaRandom ();
    }
    else if (method == "delta") {
      result = TestDeltaRandom ();
    }
    else if (method == "golomb") {
      result = TestGolombRandom (param);
    }
    else if (method == "rice") {
      result = TestRiceRandom (param);
    }
    else {
      cerr << "==\t* Error:  Test case unknown:   sequential " << method << endl;
      return (false);
    }
  }
  else if (showlengths != UINT_MAX) {
    if (method == "unary") {
      result = TestUnaryShowLengths (showlengths);
    }
    else if (method == "binary") {
      result = TestBinaryShowLengths (showlengths);
    }
    else if (method == "gamma") {
      result = TestGammaShowLengths (showlengths);
    }
    else if (method == "delta") {
      result = TestDeltaShowLengths (showlengths);
    }
    else if (method == "golomb") {
      result = TestGolombShowLengths (param, showlengths);
    }
    else if (method == "rice") {
      result = TestRiceShowLengths (param, showlengths);
    }
    else {
      cerr << "==\t* Error:  Test case unknown:   showlengths " << method << endl;
      return (false);
    }
  }
  else {
    if (method == "unary") {
      result = TestUnarySequential ();
    }
    else if (method == "binary") {
      result = TestBinarySequential ();
    }
    else if (method == "gamma") {
      result = TestGammaSequential ();
    }
    else if (method == "delta") {
      result = TestDeltaSequential ();
    }
    else if (method == "golomb") {
      result = TestGolombSequential (param);
    }
    else if (method == "rice") {
      result = TestRiceSequential (param);
    }
    else {
      cerr << "==\t* Error:  Test case unknown:   sequential " << method << endl;
      return (false);
    }
  }

  //  Erase the temporary file; assumes it is called "tmp.data"
  if (!save_file) {
    bfs::path tmp_path ("tmp.data");
    remove (tmp_path);
  }
  
  return (result);
}

