//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_DEVICE_HPP
#define ODECL_DEVICE_HPP

namespace odecl
{

	class device
	{
		/*
		VARIABLES SECTION
		*/
	public:

		// Platform ID
		cl_device_id m_device_id;

		// Device ID
		cl_device_id m_device;

		// Device type
		cl_device_type m_device_type;

	private:

		/*
		FUNCTIONS SECTION
		*/
	public:

		// Default constructor
		device()
		{

		}

		// Default destructor
		~device()
		{

		}

		// Costructor with specific platform ID
		device(cl_device_id m_device_id)
		{
			this->m_device_id = m_device_id;

			//std::cout << "the name is " << name().c_str() << std::endl;
			//std::cout << "the version is " << version().c_str() << std::endl;
		}

		// get device name
		std::string name()
		{
			return get_info(CL_DEVICE_NAME);
		}

		// get device name
		std::string version()
		{
			return get_info(CL_DEVICE_VERSION);
		}

		// get device type
		std::string type_str()
		{
			return get_info(CL_DEVICE_TYPE);
		}

		cl_device_type type()
		{
			// print device name
			cl_device_type info;

			clGetDeviceInfo(m_device_id, CL_DEVICE_TYPE, sizeof(cl_device_type), &info, NULL);

			return info;
		}

		// get platform info based on attribute
		std::string get_info(cl_device_info cl_pi)
		{
			// print device name
			char* info;
			size_t infoSize;

			clGetDeviceInfo(m_device_id, cl_pi, 0, NULL, &infoSize);

			info = new char[infoSize];
			clGetDeviceInfo(m_device_id, cl_pi, infoSize, info, NULL);
			
			std::string str(info);

			return str;
		}

	private:

	};

}


#endif // ODECL_DEVICE_HPP