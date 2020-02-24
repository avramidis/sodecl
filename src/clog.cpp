//---------------------------------------------------------------------------//
// Copyright (c) 2015,2016 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <string>
#include <sstream>
#include <fstream>

#include "clog.hpp"

using namespace std;

namespace sodecl {

    // Initialise variables.
    bool clog::m_instanceExists = false;
    clog* clog::m_instance = NULL;

    clog::clog()
    {
        ofstream newFileLog("sodecllog.txt", std::ios::binary | std::ios::out);
        newFileLog.close();

        ofstream newFileExitStatus("sodeclexitstatus.txt", std::ios::binary | std::ios::out);
        newFileExitStatus.close();
    }

    clog*
    clog::getInstance()
    {
        if (!m_instanceExists) {
            m_instance = new clog();
            m_instanceExists = true;
            return m_instance;
        }
        else {
            return m_instance;
        }
    }

    clog::~clog()
    {
        m_instanceExists = false;
    }

    void
    clog::write(const char* msg)
    {
        m_oss << msg;
    }

    void
    clog::write(double msg)
    {
        m_oss << msg;
    }

    void
    clog::toFile()
    {
        std::ofstream output_stream;

        output_stream.open("sodecllog.txt", std::ios::binary | std::ios::app | std::ios::out);

        output_stream << m_oss.str();

        output_stream.close();

        m_oss.clear();
    }

    void
    clog::writeExitStatusFile(size_t exittype, string msg)
    {
        std::ostringstream oss;
        if (exittype==0) {
            oss << "1 ";
        }
        else {
            oss << "0 ";
        }

        oss << msg;

        std::ofstream output_stream;
        output_stream.open("sodeclexitstatus.txt", std::ios::binary | std::ios::app | std::ios::out);
        output_stream << oss.str();
        output_stream.close();

        oss.clear();
    }

}
