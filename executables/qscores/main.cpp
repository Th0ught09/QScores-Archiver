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
    \file main.cpp
    Test driver for QScores class.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
    $Id: main.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <climits>  //  UINT_MAX

using namespace std;

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-defn.hpp"
#include "qscores-settings.hpp"
#include "qscores.hpp"
#include "parameters.hpp"


/*!
     Main driver

     \param[in] argc Number of arguments
     \param[in] argv Arguments to program
     \return Returns 0 on success, 1 otherwise.
*/
int main(int argc, char **argv) {
  QScores qscores;

  try {
    //  Read the configuration file and then the command line parameters
    if (!qscores.ProcessOptions (argc, argv)) {
      return (EXIT_FAILURE);
    }

    //  See if the user just wants to check external software
    if (qscores.GetExternalSoftwareCheck ()) {
      //  Initialize the paths; the values passed into the function aren't important
      qscores.Initialize ();
      qscores.PerformExternalSoftwareCheck ();
      return (EXIT_SUCCESS);
    }

    //  See if the user just wants to check the binning size
    if (qscores.GetBinningCheck ()) {
      qscores.PerformBinningCheck ();
      return (EXIT_SUCCESS);
    }
    else if (qscores.GetUnbinningCheck ()) {
      qscores.PerformUnbinningCheck ();
      return (EXIT_SUCCESS);
    }

    //  If the parameters check out, run it
    if (!qscores.CheckSettings ()) {
      return (EXIT_FAILURE);
    }

    if (!qscores.Run ()) {
      return (EXIT_FAILURE);
    }
    
    if (qscores.GetVerbose ()) {
      if (qscores.GetEncode ()) {
        cerr << "II\tEncoding finished!" << endl << endl;
      }
      else if (qscores.GetDecode ()) {
        cerr << "II\tDecoding finished!" << endl << endl;
      }
    }
  }
  catch (...) {
    cerr << "EE\tUnknown exception thrown..." << endl;
  }
  
  return (EXIT_SUCCESS);
}

