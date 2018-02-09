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
    ~opencl_mgr(){};

    /**
     * @brief Returns the number of OpenCL platforms available.
     * 
     * @return      int         The number of OpenCL platforms available. Returns -1 if the operation failed
     */
    int get_platform_count()
    {
        cl_uint platform_count;

        // get platform count
        cl_int err = clGetPlatformIDs(platform_count, NULL, &platform_count);

        if (err != CL_SUCCESS)
        {
            return -1;
        }

        return (int)platform_count;
    }
};
}

#endif // SODECL_OPENLC_MGR_HPP