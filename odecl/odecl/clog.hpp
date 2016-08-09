//---------------------------------------------------------------------------//
// Copyright (c) 2015,2016 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_CLOG_HPP
#define ODECL_CLOG_HPP

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

namespace odecl
{
	class clog
	{
	private:

		static bool m_instanceExists;
		static clog* m_instance;

		std::ostringstream m_oss;

		/// <summary>
		/// Default constructor which initialises the clog object. 
		/// </summary>
		clog()
		{

		}

	public:

		static clog* getInstance()
		{
			if (!m_instanceExists)
			{
				m_instance = new clog();
				m_instanceExists = true;
				return m_instance;
			}
			else
			{
				return m_instance;
			}
		}

		~clog()
		{
			m_instanceExists = false;
		}

		/// <summary>
		/// Writes a message to the log.
		/// </summary>
		/// <param name="msg">String with the message.</param>
		void write(string msg)
		{
			m_oss << msg;
		}

		/// <summary>
		/// Writes a message to the log.
		/// </summary>
		/// <param name="msg">Double with the message.</param>
		void write(double msg)
		{
			m_oss << msg;
		}
		
		/// <summary>
		/// Writes the log messages in a text file.
		/// </summary>
		void toFile()
		{
			std::ofstream output_stream;

			output_stream.open("odecllog.txt", std::ios::binary | std::ios::app | std::ios::out);

			output_stream << m_oss.str();

			output_stream.close();

			m_oss.clear();
		}
	};

	// Initialise variables.
	bool clog::m_instanceExists=false;
	clog* clog::m_instance = NULL;
}


#endif // ODECL_CLOG_HPP