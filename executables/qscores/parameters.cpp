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
    \file parameters.cpp
    Member functions of QScores for checking parameters .
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: parameters.cpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>  //  setw
#include <climits>  //  UINT_MAX

#include <boost/program_options.hpp>

using namespace std;
using namespace boost;
namespace po = boost::program_options;

//  Pull the configuration file in
#include "QScoresConfig.hpp"

#if ZLIB_FOUND
#include "zlib.h"
#endif

#if BZIP2_FOUND
#include "bzlib.h"
#endif

#include "systemcfg.hpp"
#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-local.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"
#include "parameters.hpp"


/*!
     This function makes use of Boost's program_options for handling
     arguments on the command line and in options in a configuration
     file whose format resembles .ini files [Filename:  g_CFG_FILENAME].

     Initially, boolean and enumerated values are given default values.
     Then, the available options are set up, with default values for
     string and numeric types.  The description of the options are
     recorded.

     Next, the command line options are read, followed by the configuration
     file options.  The command line options take priority over the
     configuration file ones.  Then, the options are processed, one-by-one.

     All of this is encapsulated within a try...catch block.

     \param[in] argc Number of arguments
     \param[in] argv Arguments to program
     \return Returns true on success, false on failure.
*/
bool QScores::ProcessOptions (int argc, char *argv[]) {
  try {
    //  Options that are allowed only on the command line
    po::options_description program_only ("Program options");
    program_only.add_options ()
      ("version,v", "Print version")
      ("help,h", "This help message")
      ("checkextern", "Check the status of external programs.")      
      ("checkbin", "Check the bins created.  Either --qsbinning or --epbinning is required.")
      ("checkunbin", "Check the bins created (in reverse).  Either --qsbinning or --epbinning is required.")
      ("addpath", po::value<string>(), "Add search path for external programs; separate paths with commas.")
      ("debug", "Turn debugging on.")
      ("verbose", "Turn verbose output on.")
      ;

    //  Options that are allowed on the command line and in the configuration file
    po::options_description general ("General options");
    general.add_options ()
      ("encode", "Encoding mode")
      ("decode", "Decoding mode")
      ("input", po::value<string>(), "Input filename.")
      ("output", po::value<string>(), "Output filename.")
      ("mapping", po::value<string>(), "Quality scores mapping [sanger* | solexa | illumina].")
      ("blocksize", po::value<int>() -> default_value (INT_MAX), "Block size [Infinite size*].")
      ;

    po::options_description lossy ("Lossy transformation options");
    lossy.add_options ()
      ("trunc", po::value<int>() -> default_value (-1), "Truncation")
      ("logbin", po::value<int>() -> default_value (-1), "Logarithmic binning")
      ("unibin", po::value<int>() -> default_value (-1), "Uniform binning")
      ;
      
    po::options_description lossless ("Lossless transformation options");
    lossless.add_options ()
      ("gaptrans", "Apply gap transformation")
      ("minshift", "Shift vectors by the minimum value in the block")
      ("freqorder", "Reorder alphabet based on symbol frequencies")
      ;

    po::options_description compression ("Internal compression options");
    compression.add_options ()
      ("nocompress", "No compression performed (useful if testing the lossy/lossless transformations only).")
      ("binary", "Binary coding")
      ("gamma", "Gamma coding")
      ("delta", "Delta coding")
      ("golomb", "Golomb coding")
      ("rice", "Rice coding")
      ("interp", "Interpolative coding")
      ("huffman", "Huffman coding")
      ("arithmetic", "Arithmetic coding (unavailable)")
      ("param", po::value<unsigned int>() -> default_value (UINT_MAX), "Global parameter for Golomb or Rice coding [Default:  Use block-based parameters.]")
      ;

    po::options_description ext_compression ("External compression options");
    ext_compression.add_options ()
      ("gzip", "gzip")
      ("bzip", "bzip2")
      ("repair", "Re-Pair (unavailable)")
      ("ppm", "PPM (unavailable)")
      ;

    //  Command line options
    po::options_description cmdline_options;
    cmdline_options.add (program_only).add (general).add (lossy).add (lossless).add (compression).add (ext_compression);

    //  Configuration file options
    po::options_description config_file_options;
    config_file_options.add (general).add (lossy).add (lossless).add (compression).add (ext_compression);

    po::options_description visible ("Quality Scores Compressor\n\nOptions");
    visible.add (program_only).add (general).add (lossy).add (lossless).add (compression).add (ext_compression);

    po::variables_map vm;
    store (po::command_line_parser (argc, argv).options (cmdline_options).run (), vm);

    //  -----------------------------------------------------------------
    //  Handle the configuration file first so that command line arguments override it
    //  -----------------------------------------------------------------
    ifstream cfg_fp (g_CFG_FILENAME, ios::in);
    if (cfg_fp) {
      cerr << "II\tReading from configuration file " << g_CFG_FILENAME << "." << endl;
      store (parse_config_file (cfg_fp, config_file_options), vm);
      notify (vm);
    }

    //  -----------------------------------------------------------------
    //  Program options and handle the configuration file
    //  -----------------------------------------------------------------
    if (vm.count ("version")) {
      cout << "QScores version " << QScores_VERSION_MAJOR << "." << QScores_VERSION_MINOR << ":  " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
      return false;
    }

    if (vm.count ("help")) {
      cout << visible << endl;
      cout << "Input file must be provided without any flags.  Only one of the compression" << endl;
      cout << "options (internal/external) can be selected at any one time." << endl;
      cout << "* Indicates default values." << endl;
      return false;
    }

    if (vm.count ("checkextern")) {
      SetExternalSoftwareCheck ();
    }

    if (vm.count ("checkbin")) {
      SetBinningCheck ();
    }

    if (vm.count ("checkunbin")) {
      SetUnbinningCheck ();
    }

    if (vm.count ("addpath")) {
      m_ExternalSoftware.AddSearchPath (vm["addpath"].as<string>());
    }

    if (vm.count ("debug")) {
      SetDebug ();
    }

    if (vm.count ("verbose")) {
      SetVerbose ();
    }

    //  -----------------------------------------------------------------
    //  General options
    //  -----------------------------------------------------------------
    if (vm.count ("encode")) {
      SetEncode ();
    }

    if (vm.count ("decode")) {
      SetDecode ();
    }

    //  Strings
    if (vm.count ("input")) {
      m_QScoresSettings.SetInputFn (vm["input"].as<string>());
    }

    if (vm.count ("output")) {
      m_QScoresSettings.SetOutputFn (vm["output"].as<string>());
    }

    if (vm.count ("mapping")) {
      m_QScoresSettings.SetQScoresMapping (vm["mapping"].as<string>());
    }

    if (vm.count ("blocksize")) {
      SetBlocksize (vm["blocksize"].as<int>());
    }

    //  -----------------------------------------------------------------
    //  Lossy transformation options
    //  -----------------------------------------------------------------
    if (vm.count ("trunc")) {
      if (vm["trunc"].as<int> () != -1) {
        m_QScoresSettings.SetLossyMaxTruncation ();
        m_QScoresSettings.SetLossyMaxTruncationParameter (vm["trunc"].as<int>());
      }
    }

    if (vm.count ("logbin")) {
      if (vm["logbin"].as<int> () != -1) {
        m_QScoresSettings.SetLossyLogBinning ();
        m_QScoresSettings.SetLossyLogBinningParameter (vm["logbin"].as<int>());
      }
    }

    if (vm.count ("unibin")) {
      if (vm["unibin"].as<int> () != -1) {
        m_QScoresSettings.SetLossyUniBinning ();
        m_QScoresSettings.SetLossyUniBinningParameter (vm["unibin"].as<int>());
      }
    }

    //  -----------------------------------------------------------------
    //  Lossless transformation options
    //  -----------------------------------------------------------------
    if (vm.count ("gaptrans")) {
      m_QScoresSettings.SetTransformGapTrans ();
    }

    if (vm.count ("minshift")) {
      m_QScoresSettings.SetTransformMinShift ();
    }

    if (vm.count ("freqorder")) {
      m_QScoresSettings.SetTransformFreqOrder ();
    }

    //  -----------------------------------------------------------------
    //  Compression options
    //  -----------------------------------------------------------------
    if (vm.count ("binary")) {
      m_QScoresSettings.SetCompressionBinary ();
    }

    if (vm.count ("gamma")) {
      m_QScoresSettings.SetCompressionGamma ();
    }

    if (vm.count ("delta")) {
      m_QScoresSettings.SetCompressionDelta ();
    }

    if (vm.count ("interp")) {
      m_QScoresSettings.SetCompressionInterP ();
    }

    if (vm.count ("golomb")) {
      m_QScoresSettings.SetCompressionGolomb ();
    }

    if (vm.count ("rice")) {
      m_QScoresSettings.SetCompressionRice ();
    }

    if (vm.count ("huffman")) {
      m_QScoresSettings.SetCompressionHuffman ();
    }

    if (vm.count ("arithmetic")) {
      m_QScoresSettings.SetCompressionArithmetic ();
      cerr << "EE\t--arithmetic has not been implemented yet." << endl;
      exit (EXIT_FAILURE);
    }

    if (vm.count ("gzip")) {
      m_QScoresSettings.SetCompressionGzip ();
    }

    if (vm.count ("bzip")) {
      m_QScoresSettings.SetCompressionBzip ();
    }

    if (vm.count ("repair")) {
      m_QScoresSettings.SetCompressionRepair ();
      cerr << "EE\t--repair has not been implemented yet." << endl;
      exit (EXIT_FAILURE);
    }

    if (vm.count ("ppm")) {
      m_QScoresSettings.SetCompressionPPM ();
      cerr << "EE\t--ppm has not been implemented yet." << endl;
      exit (EXIT_FAILURE);
    }
    
    if (vm.count ("nocompress")) {
      m_QScoresSettings.SetCompressionNone ();
    }

    if (vm.count ("param")) {
      m_QScoresSettings.SetCompressionGlobalParameter (vm["param"].as<unsigned int>());
    }
  }
  catch (std::exception& e) {
    cout << e.what() << "\n";
    return false;
  }


  return true;
}


/*!
     Check the settings to ensure they are valid.

     \return Returns true on success, false on failure.
*/
bool QScores::CheckSettings () {
  if ((GetEncode ()) && (GetDecode ())) {
    cerr << "EE\tChoose from either --encode or --decode, but not both." << endl;
    exit (EXIT_FAILURE);
  }
  
  if ((!GetEncode ()) && (!GetDecode ())) {
    cerr << "EE\tOne of --encode or --decode must be selected." << endl;
    exit (EXIT_FAILURE);
  }

  if (GetEncode ()) {
    if (GetBlocksize () == 0) {
      cerr << "EE\tThe block size accompanying --blocksize cannot be 0." << endl;
      exit (EXIT_FAILURE);
    }
  }

  if (m_QScoresSettings.GetQScoresMapping () == e_QSCORES_MAP_UNSET) {
    m_QScoresSettings.SetQScoresMapping ("sanger");  //  Sanger is the default method
  }

  if (m_QScoresSettings.GetCompressionNone () && (!GetEncode())) {
    cerr << "EE\tThe 'no compression' option is only valid with encoding.  Please use the --encode option." << endl;
    exit (EXIT_FAILURE);
  }

  //  Pass debug parameters to other objects here
  if (GetDebug ()) {
    m_QScoresSettings.SetDebug ();
  }

  //  Summarize the settings that were selected
  if (GetVerbose ()) {
    cerr << left << setw (g_VERBOSE_WIDTH) << "II\tDebug mode:" << (GetDebug () == true ? "Yes" : "No") << endl;
    cerr << left << setw (g_VERBOSE_WIDTH) << "II\tVerbose mode:" << (GetVerbose () == true ? "Yes" : "No") << endl;
    cerr << left << setw (g_VERBOSE_WIDTH) << "II\tProgram mode:" << (GetEncode () == true ? "Encoding" : "Decoding") << endl;
    if (GetEncode ()) {
      cerr << left << setw (g_VERBOSE_WIDTH) << "II\tBlocksize:" << GetBlocksize () << endl;
    }
  }

  //  Any output that follows is not related to parameters; so add some space
  cerr << endl;
  
  return true;
}


