//---------------------------------------------------------------------------//
// Copyright (c) 2015,2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_OPENLC_SOURCE_MGR_HPP
#define SODECL_OPENLC_SOURCE_MGR_HPP

#include "sodecl.hpp"

namespace sodecl
{

/**
 * @brief Abstract class that implements the functions needed for the creation of OpenCL source files.
 * 
 */
class opencl_source_mgr
{
  private:
    /********************************************************************************************
	* GENERAL VARIABLES
	*/

    std::vector<char>				m_kernel_sources;		/**< Char vector which stores an OpenCL kernel source string. */
    size_t							m_source_size;			/**< OpenCL kernel string size */

    // Log mechanisms
    clog*                           m_log; /**< Pointer for log */

  public:
    /**
     * @brief Default constructor.
     * 
     */
    opencl_source_mgr()
    {
        
    };

    /**
     * @brief Destructor.
     * 
     */
    ~opencl_source_mgr()
    {
        
    };

    /**
    * Reads an OpenCL function or kernel
    *
    * @param	filename	OpenCL file
    */
    void read_kernel_file(char* filename)
    {
        std::ifstream t(filename);
        if (t.is_open()) {
            t.seekg(0, std::ios::end);
            size_t size = t.tellg();
            std::string str(size, ' ');
            t.seekg(0);
            t.read(&str[0], size);
            t.close();

            m_source_size = str.length();

            for (int i = 0; i < m_source_size; i++)
            {
                m_kernel_sources.push_back(str[i]);
            }
        }
        else {
            // show message:
            std::cout << "Error opening file";
        }
    }

    /**
    * Adds the string str to the vector which stores the kernel string (i.e. OpenCL kernel code) 
    *
    * @param	str		The sting to be pushed in the OpenCL kernel code vector
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull
    */
    int add_string_to_kernel_sources(string str)
    {
        // TODO: Add code to catch an exception in case the operation fails
        for (size_t i = 0; i < str.length(); i++)
        {
            m_kernel_sources.push_back(str[i]);
        }

        return 1;
    }

    /**
    * Convert a double to string.
    *
    * @param	x		Double number which will be coverted to string
    *
    * @return	A string of the converted double number
    */
    string double_to_string(double x)
    {
        // @todo Add code to catch an exception in the unlikely case of the conversion fails
        std::string s;
        {
            std::ostringstream ss;
            ss << x;
            s = ss.str();
        }
        return s;
    }

    /**
    * Form the OpenCL kernel string.
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    virtual int create_kernel_string() = 0;
};
}

#endif // SODECL_OPENLC_SOURCE_MGR_HPP