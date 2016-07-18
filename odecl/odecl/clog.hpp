//---------------------------------------------------------------------------//
// Copyright (c) 2015,2016 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_CLOG_HPP
#define ODECL_CLOG_HPP

#include <string>
#include <vector>

using namespace std;

namespace odecl
{
	class clog
	{
	private:

		// Vector with the output messages
		std::vector<string> m_msgs;

	public:

		/// <summary>
		/// Default constructor which initialises the clog object. 
		/// </summary>
		clog()
		{

		}

		/// <summary>
		/// Writes a message to the log.
		/// </summary>
		/// <param name="msg">String with the message.</param>
		void write(string msg)
		{
			m_msgs.push_back(msg);
		}

		/// <summary>
		/// Writes the log messages in a text file.
		/// </summary>
		void toFile()
		{

		}
	};
}


#endif // ODECL_CLOG_HPP