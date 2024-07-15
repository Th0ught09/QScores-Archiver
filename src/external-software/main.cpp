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
    \file main.cpp
    Test driver for ExternalSoftware class.
*/
/*******************************************************************/

#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE
#include <string>
#include <vector>
#include <iostream>
#include <cstring>  //  strcmp
#include <cstdio>  //  printf

using namespace std;

#include "external-software-local.hpp"
#include "external-software.hpp"
#include "testing.hpp"


/*!
     Main driver

     \param[in] argc Number of arguments
     \param[in] argv Arguments to program
     \return Returns 0 on success, 1 otherwise.
*/
int main(int argc, char **argv) {
  bool last = false;

  //  Need at least one argument
  if (argc < 2) {
    return (EXIT_FAILURE);
  }

  if (strcmp (argv[1], "1") == 0) {
    cout << "External Software version " << EXTERNAL_SOFTWARE_PROGRAM_VERSION << " compiled on:  " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
    cout << "Git hash:  " << EXTERNAL_SOFTWARE_GIT_HASH << endl;
  }
  else if (strcmp (argv[1], "2") == 0) {
    ExternalSoftware external_software (true);
    cout << external_software << endl;
  }
  else if ((strcmp (argv[1], "3") == 0) || (strcmp (argv[1], "5") == 0)) {
    enum e_EXTERNAL_METHOD method;
    if (strcmp (argv[1], "3") == 0) {
      method = e_EXTERNAL_METHOD_GZIP_ZLIB;
    }
    if (strcmp (argv[1], "5") == 0) {
      method = e_EXTERNAL_METHOD_BZIP_BZLIB;
    }
    
    unsigned int size = 100;
    char tmp1[32] = "zenzizenzizenzizenzizenzizenzic";
    char *tmp2 = (char*) (malloc (sizeof (char) * size));
    char *tmp3 = (char*) (malloc (sizeof (char) * size));

    ExternalSoftware external_software1 (true);
    external_software1.Initialize (method, true);
    cout << external_software1 << endl;
    external_software1.Process (tmp1, 31, true);
    unsigned int inbuffer_size = external_software1.GetInBufferLength ();
    unsigned int outbuffer_size = external_software1.GetOutBufferLength ();
    unsigned int retrieved_size = external_software1.RetrieveCharBlock (tmp2, size, last);
    
    ExternalSoftware external_software2 (true);
    external_software2.Initialize (method, false);
    external_software2.UnProcess (tmp2, outbuffer_size, true);
    inbuffer_size = external_software2.GetInBufferLength ();
    outbuffer_size = external_software2.GetOutBufferLength ();
    retrieved_size = external_software2.RetrieveCharBlock (tmp3, size, last);
    DisplayChar (tmp3, outbuffer_size);
    if (!CompareChar (tmp1, tmp3, outbuffer_size)) {
      cerr << "EE\tStrings failed to match." << endl;
      return (EXIT_FAILURE);
    }
    free (tmp2);
    free (tmp3);
  }
  else if ((strcmp (argv[1], "4") == 0) || (strcmp (argv[1], "6") == 0)) {
    enum e_EXTERNAL_METHOD method;
    if (strcmp (argv[1], "4") == 0) {
      method = e_EXTERNAL_METHOD_GZIP_ZLIB;
    }
    if (strcmp (argv[1], "6") == 0) {
      method = e_EXTERNAL_METHOD_BZIP_BZLIB;
    }
    
    unsigned int size = 0;
    char* tmp1 = ReadFile (string (argv[2]), size);
    if (tmp1 == NULL) {
      return (EXIT_FAILURE);
    }
    char* tmp2 = (char*) calloc (size * 2, sizeof (char));
    char* tmp3 = (char*) calloc (size * 2, sizeof (char));

    ExternalSoftware external_software1 (true);
    external_software1.Initialize (method, true);
    cout << external_software1 << endl;

    unsigned int pos = 0;
    while ((size - (pos * g_BLOCK_SIZE)) > g_BLOCK_SIZE) {
      external_software1.Process (&tmp1[pos * g_BLOCK_SIZE], g_BLOCK_SIZE, false);
      pos++;
    }

    //  Even if size can be evenly divided by g_BLOCK_SIZE, we still need to call Process () with "true"
    external_software1.Process (&tmp1[pos * g_BLOCK_SIZE], (size - (pos * g_BLOCK_SIZE)), true);
        
    unsigned int inbuffer_size = external_software1.GetInBufferLength ();
    unsigned int outbuffer_size = external_software1.GetOutBufferLength ();
    unsigned int retrieved_size = external_software1.RetrieveCharBlock (tmp2, outbuffer_size, last);

    ExternalSoftware external_software2 (true);
    external_software2.Initialize (method, false);
    last = false;
    pos = 0;
    while ((retrieved_size - (pos * g_BLOCK_SIZE)) > g_BLOCK_SIZE) {
      external_software2.UnProcess (&tmp2[pos * g_BLOCK_SIZE], g_BLOCK_SIZE, false);
      pos++;
    }
    external_software2.UnProcess (&tmp2[pos * g_BLOCK_SIZE], (retrieved_size - (pos * g_BLOCK_SIZE)), true);
    
    inbuffer_size = external_software2.GetInBufferLength ();
    outbuffer_size = external_software2.GetOutBufferLength ();
    retrieved_size = external_software2.RetrieveCharBlock (tmp3, outbuffer_size, last);
    
//     DisplayChar (tmp3, outbuffer_size);
    if (!CompareChar (tmp1, tmp3, outbuffer_size)) {
      cerr << "EE\tStrings failed to match." << endl;
      return (EXIT_FAILURE);
    }
    
    free (tmp1);
    free (tmp2);
    free (tmp3);
  }
  cerr << "Hello" << endl;
  return (EXIT_SUCCESS);
}


