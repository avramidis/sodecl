//---------------------------------------------------------------------------//
// Copyright (c) 2015,2016 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef SODECL_CLOG_HPP
#define SODECL_CLOG_HPP

#include "sodecl_export.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace sodecl {
    class SODECL_EXPORT clog {
    private:

        static bool m_instanceExists;
        static clog *m_instance;

        std::ostringstream m_oss;

        /// <summary>
        /// Default constructor which initialises the clog object.
        /// </summary>
        clog();

    public:

        static clog *getInstance();

        ~clog();

        /// <summary>
        /// Writes a message to the log.
        /// </summary>
        /// <param name="msg">String with the message.</param>
        void write(const char* msg);

        /// <summary>
        /// Writes a message to the log.
        /// </summary>
        /// <param name="msg">Double with the message.</param>
        void write(double msg);

        /// <summary>
        /// Writes the log messages in a text file.
        /// </summary>
        void toFile();

        /// <summary>
        /// Writes the SODECL exit status file.
        /// </summary>
        void writeExitStatusFile(size_t exittype, string msg);
    };
}


#endif // SODECL_CLOG_HPP
