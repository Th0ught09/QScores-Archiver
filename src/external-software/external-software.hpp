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
    \file external-software.hpp
    Header file for use of external compression programs.
*/
/*******************************************************************/

#ifndef EXTERNAL_SOFTWARE_HPP
#define EXTERNAL_SOFTWARE_HPP

#include "ExternalSoftware_Config.hpp"
#if ZLIB_FOUND
#include "zlib.h"
const bool g_USE_ZLIB = true;
#else
const bool g_USE_ZLIB = false;
#endif

#if BZIP2_FOUND
#include "bzlib.h"
const bool g_USE_BZLIB = true;
#else
const bool g_USE_BZLIB = false;
#endif


/*!
     \enum e_EXTERNAL_METHOD
     External method to use.
*/
enum e_EXTERNAL_METHOD {
  e_EXTERNAL_METHOD_UNSET,  /*!< External method not yet decided */
  e_EXTERNAL_METHOD_GZIP_ZLIB,  /*!< Gzip/Zlib method */
  e_EXTERNAL_METHOD_BZIP_BZLIB,  /*!< Bzip2/BZlib method */
  e_EXTERNAL_METHOD_REPAIR,  /*!< Re-Pair method */
  e_EXTERNAL_METHOD_LAST  /*!< Last external method */
};


/*!
    \class ExternalSoftware

    \details Class used to access external compression software.
*/
class ExternalSoftware {
  friend ostream &operator<< (ostream &os, const ExternalSoftware& es);

  public:
    //  Constructors/destructors  [external-software.cpp]
    void ShowInformation (string str, char* file, unsigned int line) const;
    ExternalSoftware (bool debug=false);
    ~ExternalSoftware ();

    //  Initialization/uninitialization  [setup.cpp]
    void Initialize (enum e_EXTERNAL_METHOD method, bool compress);
    void UnInitialize ();
    void InitializePaths ();

    //  Main processing functions  [process.cpp]
    void Process (char* buffer, unsigned int buffer_size, bool last);
    void UnProcess (char* buffer, unsigned int buffer_size, bool last);
    unsigned int RetrieveUCharBlock (unsigned char* buffer, unsigned int buffer_size, bool& last);
    unsigned int RetrieveCharBlock (char* buffer, unsigned int buffer_size, bool& last);
    unsigned char* RetrieveUChar ();
    char* RetrieveChar ();
    unsigned int GetInBufferLength () const;
    unsigned int GetOutBufferLength () const;
  
    //  Accessors  [accessors.cpp]
    bool GetDebug () const;
    bool GetInitializePaths () const;
    bool GetGzipCommand () const;
    bool GetGunzipCommand () const;
    bool GetBzipCommand () const;
    bool GetBunzipCommand () const;
    bool GetRepairCommand () const;
    bool GetDespairCommand () const;
    bool GetShuffCommand () const;
    std::string GetGzipCommandPath () const;
    std::string GetGunzipCommandPath () const;
    std::string GetBzipCommandPath () const;
    std::string GetBunzipCommandPath () const;
    std::string GetRepairCommandPath () const;
    std::string GetDespairCommandPath () const;
    std::string GetShuffCommandPath () const;
    std::string ShowSearchPaths () const;

    //  Mutators  [mutators.cpp]
    bool SetDebug ();
    bool SetInitializePaths ();
    bool SetGzipCommand (std::string cmd);
    bool SetGunzipCommand (std::string cmd);
    bool SetBzipCommand (std::string cmd);
    bool SetBunzipCommand (std::string cmd);
    bool SetRepairCommand (std::string cmd);
    bool SetDespairCommand (std::string cmd);
    bool SetShuffCommand (std::string cmd);
    void AddSearchPath (std::string path);
  private:
    //  Main zlib/gzip processing functions  [gzip-zlib.cpp]
    void ProcessZlib ();
    void UnProcessZlib ();
    void ProcessGzip ();
    void UnProcessGzip ();

    //  Main bzlib/bzip processing functions  [bzip-bzlib.cpp]
    bool InitializeBzipBZlib ();
    bool UnInitializeBzipBZlib ();
    void ProcessBZlib ();
    void UnProcessBZlib ();
    void ProcessBzip ();
    void UnProcessBzip ();

    //  Main zlib/gzip processing functions  [repair-shuff.cpp]
    void ProcessRePair ();
    void UnProcessRePair ();

    //!  Debug mode?
    bool m_Debug;

    //!  Indicate whether we are compressing or decompressing
    bool m_Compress;

    //!  The external method to used
    enum e_EXTERNAL_METHOD m_Method;

    //!  Have we initialized the paths yet?
    bool m_InitializePaths;

    //!  Set of search paths
    vector<std::string> m_SearchPaths;

    //!  Command to run gzip
    std::string m_GzipCommand;

    //!  Command to run gunzip
    std::string m_GunzipCommand;

    //!  Command to run bzip
    std::string m_BzipCommand;

    //!  Command to run bunzip
    std::string m_BunzipCommand;

    //!  Command to run Re-Pair
    std::string m_RepairCommand;

    //!  Command to run Des-Pair
    std::string m_DespairCommand;

    //!  Command to run Shuff
    std::string m_ShuffCommand;

#if ZLIB_FOUND
    //  Data structure required for using zlib
    z_stream *m_ZStream;
#endif

#if BZIP2_FOUND
    //  Data structure required for using bzlib
    bz_stream *m_BZStream;
#endif

    //!  Dictionary buffer
    char *m_DictionaryBuffer;

    //!  Pointer indicating how much of the dictionary buffer has been used
    unsigned int m_DictionaryBufferPtr;

    //!  Total size of the dictionary buffer
    unsigned int m_DictionaryBufferSize;

    //!  Input buffer
    char *m_InBuffer;

    //!  Pointer indicating how much of the input buffer has been used
    unsigned int m_InBufferPtr;

    //!  Pointer indicating how much of the input buffer has been read from
    unsigned int m_InBufferRetrieval;

    //!  Total size of the input buffer
    unsigned int m_InBufferSize;
    
    //!  Output buffer
    char *m_OutBuffer;

    //!  Pointer indicating how much of the output buffer has been used
    unsigned int m_OutBufferPtr;

    //!  Pointer indicating how much of the output buffer has been read from
    unsigned int m_OutBufferRetrieval;

    //!  Total size of the output buffer
    unsigned int m_OutBufferSize;
};

#endif

