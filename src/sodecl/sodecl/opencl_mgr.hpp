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
    /********************************************************************************************
	* OPENCL HARDWARE SECTION VARIABLES
	*/

    cl_uint                             m_opencl_platform_count;            /**< Number of OpenCL platforms */
    std::vector<sodecl::platform *>     m_opencl_platforms;                 /**< Vector which stores the sodecl::platform objects. One object for each OpenCL platform */
    cl_uint                             m_selected_opencl_platform;         /**< The index of the selected sodecl::platform object in the m_platforms vector */
    cl_uint                             m_selected_opencl_device;           /**< The index of the selected sodecl::device object in m_devices vector of selected platform */
    device_Type                         m_selected_opencl_device_type;      /**< Selected OpenCL device type */

    /********************************************************************************************
    OPENCL SOFTWARE SECTION VARIABLES
    */

    cl_context                          m_context;              /**< OpenCL context */
    cl_command_queue                    m_command_queue;        /**< OpenCL command queue */
    std::vector<char*>                  m_kernel_sources;       /**< Char vector which stores the OpenCL kernel source string. @todo store multiple kernel source strings */
    std::string                         m_build_options_str;    /**< Char vector which stores the OpenCL build options string */
    std::vector<build_Option>           m_build_options;        /**< build_Option vector which stores the OpenCL build options selection */
    string                              m_kernel_path_str;      /**< OpenCL kernels solvers path */
    size_t                              m_source_size;          /**< OpenCL kernel string size */
    std::vector<cl_program>             m_programs;             /**< OpenCL programs vector */
    std::vector<cl_kernel>              m_kernels;              /**< OpenCL kernels vector */
    int                                 m_local_group_size;     /**< OpenCL device local group size */
	cl_double*	                        m_rcounter;		        /**< Counter that counts the number of calls to the random number generator */ 

    // Log mechanisms
    clog*                               m_log; /**< Pointer for log */

  public:
    /**
     * @brief Default constructor.
     * 
     */
    opencl_mgr()
    {
        // Initialise the clog object
        m_log = clog::getInstance();

        get_opencl_platform_count();
        create_opencl_platforms();
    };

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
        // get platform count
        cl_int err = clGetPlatformIDs(0, nullptr, &m_opencl_platform_count);

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

        return (int)m_opencl_platform_count;
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

    /**
     * @brief Sets the sodecl object to use the selected OpenCL device for the integration of the ODE model.
     * 
     * @param	platform_num	Index of selected OpenCL platform
     * @param	device_type		OpenCL device type
     * @param	device_num		Index of selected OpenCL device in the selected OpenCL platform
     * @return  int             Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int choose_opencl_device(cl_uint platform_num, device_Type device_type, cl_uint device_num)
    {
        // Check if selected platform exist
        if (platform_num < 0 || platform_num > m_opencl_platform_count)
        {
            //cerr << "Selected platform number is out of bounds." << std::endl;
            m_log->write("Selected platform number is out of bounds.\n");
            return 0;
        }

        // Check if selected device exist
        if (device_num < 0 || device_num > m_opencl_platforms[platform_num]->get_device_count())
        {
            //cerr << "Selected device number is out of bounds." << std::endl;
            m_log->write("Selected device number is out of bounds.\n");
            return 0;
        }

        // If selected device type is not ALL (OpenCL type) check if selected device type exist
        if ((cl_device_type)device_type != (cl_device_type)device_Type::ALL)
        {
            if (m_opencl_platforms[platform_num]->m_devices[device_num]->type() != (cl_device_type)device_type)
            {
                //cerr << "Selected device is not of the type selected." << std::endl;
                m_log->write("Selected device is not of the type selected.\n");
                return 0;
            }
        }

        //std::cout << "Selected platform: " << m_platforms[platform_num]->name().c_str() << std::endl;
        //std::cout << "Device name: " << m_platforms[platform_num]->m_devices[device_num]->name().c_str() << std::endl;
        //std::cout << "Device type: " << m_platforms[platform_num]->m_devices[device_num]->type() << std::endl;

        m_log->write("Selected platform name: ");
        m_log->write(m_opencl_platforms[platform_num]->name().c_str());
        m_log->write("\n");

        m_log->write("Selected device name: ");
        m_log->write(m_opencl_platforms[platform_num]->m_devices[device_num]->name().c_str());
        m_log->write("\n");

        m_log->write("Selected device OpenCL version: ");
        m_log->write(m_opencl_platforms[platform_num]->m_devices[device_num]->version().c_str());
        m_log->write("\n");

        m_selected_opencl_platform = platform_num;
        m_selected_opencl_device = device_num;
        m_selected_opencl_device_type = device_type;

        return 1;
    }

    /**
     * @brief Create OpenCL context for the selected OpenCL platform and OpenCL device.
     * 
     * @return  int  Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int create_opencl_context()
    {
        cl_int err;
        m_context = clCreateContext(NULL, 1, &(m_opencl_platforms[m_selected_opencl_platform]->m_devices[m_selected_opencl_device]->m_device_id), NULL, NULL, &err);
        if (err != CL_SUCCESS)
        {
            std::cerr << "Error: Failed to create context! " << err << std::endl;
            return 0;
        }

        return 1;
    }

    /**
    * Create OpenCL command queue for the selected OpenCL platform and OpenCL device. 
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    int create_command_queue()
    {
        cl_context context = m_context;

        cl_device_id device = m_opencl_platforms[m_selected_opencl_platform]->m_devices[m_selected_opencl_device]->m_device_id;

        cl_int err;
        m_command_queue = clCreateCommandQueue(context, device, NULL, &err);
        if (err != CL_SUCCESS)
        {
            std::cout << "Error: Failed to create command queue!" << std::endl;
            return 0;
        }

        return 1;
    }

    /**
    * Create OpenCL program for the selected OpenCL platform, OpenCL device and created OpenCL context.
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    cl_program create_program(std::string source)
    {
        const char *srcptr[] = {source.c_str()};
        m_source_size = source.size();

        cl_int err;
        cl_program program = clCreateProgramWithSource(m_context, 1, srcptr, (const size_t *)&m_source_size, &err);
        if (err != CL_SUCCESS)
        {
            return NULL;
        }

        return program;
    }

    /// <summary>
    /// Create the OpenCL kernel.
    /// </summary>
    /// <param name="kernelname">Name of the OpenCL kernel.</param>
    /// <param name="program">OpenCL program.</param>
    /// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
    cl_kernel create_kernel(char* kernelname, cl_program program)
    {
        cl_int err_code;

        cl_kernel kernel = clCreateKernel(program, kernelname, &err_code);
        if (!kernel || err_code != CL_SUCCESS)
        {
            cerr << "Error: Failed to create compute kernel!" << std::endl;
            return NULL;
        }

        return kernel;
    }

    /**
     * @brief Creates an OpenCL buffer.
     * 
     * @param	context		OpenCL cl_context.
     * @param	list_size	Size of the buffer in number of double values.
     * @return  cl_mem      Returns the cl_mem buffer if the operation was succcessful or 0 if they were unsuccessful.
     */
    cl_mem create_buffer(cl_context context, int list_size, cl_map_flags flags)
    {
        cl_int err_code;
        cl_mem buffer = clCreateBuffer(context, flags, list_size * sizeof(cl_double), NULL, &err_code);
        if (err_code != CL_SUCCESS)
        {
            return NULL;
        }

        return buffer;
    }

    /* @brief Writes data in the OpenCL memory buffers.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int write_to_buffer(cl_mem buffer, double* source, int list_size)
    {
        int err = 0;
        err = clEnqueueWriteBuffer(m_command_queue, buffer, CL_TRUE, 0, list_size * sizeof(cl_double), source, 0, NULL, NULL);
        if (err != CL_SUCCESS)
        {
            std::cout << "Error: Failed to write to source array!" << std::endl;
            return 1;
        }
        return 0;
    }

    /**
    * Sets OpenCL kernel argument.
    *
    * @param	commands	OpenCL command queue
    * @param	list_size	Number of data points to be written in the OpenCL memory buffers
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    int set_kernel_arg(cl_kernel kernel, cl_mem buffer, int pos)
    {
        // Set the arguments to the compute kernel
        cl_int err = 0;
        err = clSetKernelArg(kernel, pos, sizeof(cl_mem), &buffer);
        if (err != CL_SUCCESS)
        {
            std::cout << "Error: Failed to set kernel argument! " << err << std::endl;
            return 1;
        }
        return 0;
    }

    /**
    * Adds a selected OpenCL build option in the vector which stores these OpenCL build options.
    *
    * @param	build_option	build_Option which will be stored in the vector
    *
    * @todo		This function should be expanded to perform all the operations for adding the OpenCL options in the char vector
    */
    void add_option_to_build_options(build_Option build_option)
    {
        m_build_options.push_back(build_option);
    }

    /// <summary>
    /// Adds the characters of a string in the vector which stores the OpenCL build options.
    /// </summary>
    /// <param name="str">String with the OpenCL build option.</param>

    /**
    * Adds the characters of a string in the vector which stores the OpenCL build options.
    *
    * @param	str		String with the OpenCL build option
    */
    void add_string_to_build_options_str(string str)
    {
        for (size_t i = 0; i < str.length(); i++)
        {
            m_build_options_str.push_back(str[i]);
        }
    }

    /**
    * Builds OpenCL program for the selected OpenCL device.
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    int build_program(cl_program program)
    {
        cl_device_id device_id = m_opencl_platforms[m_selected_opencl_platform]->m_devices[m_selected_opencl_device]->m_device_id;

        add_string_to_build_options_str("-I. ");

        for (build_Option option : m_build_options)
        {
            switch (option)
            {
            case build_Option::FastRelaxedMath:
                add_string_to_build_options_str("-cl-fast-relaxed-math ");
                break;
            case build_Option::stdCL20:
                add_string_to_build_options_str("-cl-std=CL2.0 ");
                break;
            case build_Option::stdCL21:
                add_string_to_build_options_str("-cl-std=CL2.1 ");
                break;
            }
        }

        const char *options = &m_build_options_str[0];
        //cerr << "Build options string: " << options << endl;
        m_log->write("Build options string: ");
        m_log->write(options);
        m_log->write("\n");

        //const char * options = "-D KHR_DP_EXTENSION -x clc++ -cl-mad-enable";
        //const char * options = "-D KHR_DP_EXTENSION -cl-opt-disable -cl-mad-enable";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-unsafe-math-optimizations";
        //const char * options = "-D KHR_DP_EXTENSION -cl-opt-disable";
        //const char * options = "-D KHR_DP_EXTENSION -cl-std=CL2.0 -cl-uniform-work-group-size";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-no-signed-zeros -cl-finite-math-only";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-std=CL2.0";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math";
        //const char * options = "-D KHR_DP_EXTENSION -cl-std=CL2.0";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-std=CL2.0";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-std=CL2.0";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-strict-aliasing -cl-single-precision-constant -cl-no-signed-zeros -cl-denorms-are-zero";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-maxrregcount=200 -cl-nv-verbose -cl-mad-enable -cl-fast-relaxed-math -cl-no-signed-zeros -cl-strict-aliasing";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-maxrregcount=200 -cl-nv-verbose -cl-mad-enable -cl-no-signed-zeros -cl-strict-aliasing";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math  -cl-nv-maxrregcount=90";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-opt-level=2";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-allow-expensive-optimizations";
        //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math";
        //const char * options = "";
        //const char * options = "-cl-fast-relaxed-math -cl-std=CL2.0";

        cl_int err = clBuildProgram(program, 1, &device_id, options, NULL, NULL);
        if (err != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];

            std::cout << "Error: " << err << std::endl;

            std::cout << "Error: Failed to build program executable!" << std::endl;

            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
                                  sizeof(buffer), buffer, &len);

            std::cout << buffer << std::endl;
            return 0;
        }

        return 1;
    }
};
}

#endif // SODECL_OPENLC_MGR_HPP