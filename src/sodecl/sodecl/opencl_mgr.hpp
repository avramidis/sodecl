#ifndef SODECL_OPENLC_MGR_HPP
#define SODECL_OPENLC_MGR_HPP

#include "sodecl.hpp"

namespace sodecl
{

/**
 * @brief Class that implements the OpenCL functions needed by the ODE and SDE solvers.
 * 
 */
class opencl_mgr
{
  private:

    /********************************************************************************************
	* OPENCL HARDWARE SECTION VARIABLES
	*/

    cl_uint m_opencl_platform_count;                    /**< Number of OpenCL platforms */
    std::vector<sodecl::platform *> m_opencl_platforms; /**< Vector which stores the sodecl::platform objects. One object for each OpenCL platform */
    cl_uint m_selected_platform;                        /**< The index of the selected sodecl::platform object in the m_platforms vector */
    cl_uint m_selected_device;                          /**< The index of the selected sodecl::device object in m_devices vector of selected platform */
    device_Type m_selected_device_type;                 /**< Selected OpenCL device type */

  public:
    /**
     * @brief Default constructor.
     * 
     */
    opencl_mgr(){};

    /**
     * @brief Destructor.
     * 
     */
    ~opencl_mgr()
    {
        for (auto i : m_opencl_platforms)
        {
            delete i;
        }
        m_opencl_platforms.clear();
    };

    /**
     * @brief Returns the number of OpenCL platforms available.
     * 
     * @return      int         The number of OpenCL platforms available. Returns -1 if the operation failed.
     */
    int get_opencl_platform_count()
    {
        cl_uint opencl_platform_count;

        // get platform count
        cl_int err = clGetPlatformIDs(0, nullptr, &opencl_platform_count);
        
        if (err == CL_INVALID_VALUE)
        {
            std::cerr << "Supplied values to the function for getting the OpenCL platform IDs are invalid." << std::endl;
            return -1;
        }

        if (err == CL_OUT_OF_HOST_MEMORY)
        {
            std::cerr << "There was a failure to allocate resources required by the OpenCL implementation on the host." << std::endl;
            return -1;
        }

        return (int)opencl_platform_count;
    }

    /**
     * @brief Creates all sodecl::platform objects.
     * 
     * @return      int         The number of OpenCL platforms available. Returns -1 if the operation failed.
     */
    int create_opencl_platforms()
    {
        cl_platform_id *cpPlatform = new cl_platform_id[m_opencl_platform_count];

        // get all OpenCL platforms
        cl_int err = clGetPlatformIDs(m_opencl_platform_count, cpPlatform, nullptr);

        if (err == CL_INVALID_VALUE)
        {
            std::cerr << "Supplied values to the function for getting the OpenCL platform IDs are invalid." << std::endl;
            delete[] cpPlatform;
            return -1;
        }

        if (err == CL_OUT_OF_HOST_MEMORY)
        {
            std::cerr << "There was a failure to allocate resources required by the OpenCL implementation on the host." << std::endl;
            delete[] cpPlatform;
            return -1;
        }

        for (cl_uint i = 0; i < m_opencl_platform_count; i++)
        {
            m_opencl_platforms.push_back(new platform(cpPlatform[i]));
        }
        delete[] cpPlatform;

        return 1;
    }
};
}

#endif // SODECL_OPENLC_MGR_HPP