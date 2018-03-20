#include "stdafx.h"
#include "OpenCLInfo.h"

namespace Info
{
	template <typename T>
	T read( const cl_device_id deviceId, const uint field)
	{
		T item;
		auto error = clGetDeviceInfo( deviceId, field, sizeof(T), &item, nullptr);
		if (error)
		{
			std::cerr << "error = " << error << '\n';
			throw Exception( field, error);
		}

		return item;
	}

	void readString( char* buffer, const size_t bufferSize, const cl_device_id deviceId, const uint field)
	{
		auto error = clGetDeviceInfo( deviceId, field, bufferSize, buffer, nullptr);
		if (error)
		{
			std::cerr << "error = " << error << '\n';
			throw Exception( field, error);
		}
	}

	void readFP( FPCapabilityArray& capabilities, const cl_device_id id, const uint field)
	{
		auto value = read<cl_device_fp_config>( id, field);
		if ( value & CL_FP_DENORM)
		{
			capabilities.push_back( fpcDenorm);
		}

		if ( value & CL_FP_INF_NAN)
		{
			capabilities.push_back( fpcInfNan);
		}

		if ( value & CL_FP_ROUND_TO_NEAREST)
		{
			capabilities.push_back( fpcRoundToNearest);
		}

		if ( value & CL_FP_ROUND_TO_ZERO)
		{
			capabilities.push_back( fpcRoundToZero);
		}

		if ( value & CL_FP_ROUND_TO_INF)
		{
			capabilities.push_back( fpcRoundToInf);
		}

		if ( value & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT)
		{
			capabilities.push_back( fpcFMA);
		}

		if ( value & CL_FP_FMA)
		{
			capabilities.push_back( fpcCorrectlyRoundedDivideSqrt);
		}

		if ( value & CL_FP_SOFT_FLOAT)
		{
			capabilities.push_back( fpcSoftFloat);
		}
	}

	void readExecutionCapabilities( ExecutionCapabilityArray& capabilities, const cl_device_id id)
	{
		auto result = read<cl_device_exec_capabilities>( id, CL_DEVICE_EXECUTION_CAPABILITIES);
		if ( result & CL_EXEC_KERNEL)
		{
			capabilities.push_back( ecKernel);
		}

		if ( result & CL_EXEC_NATIVE_KERNEL)
		{
			capabilities.push_back( ecNativeKernel);
		}
	}

	void readExtensions( ExtensionArray& extensions, const cl_device_id id)
	{
		char buffer[ 1024];
		readString( buffer, 1024, id, CL_DEVICE_EXTENSIONS);
		for( char* p1 = buffer; ; )
		{
			char* p2 = strchr( p1, ' ');
			if ( p2 && p1 != p2)
			{
				*p2 = 0;
				extensions.push_back( p1);
				p1 = p2 + 1;
			}
			else
			{
				break;
			}
		}
	}

	void readGlobalMemory( GlobalMemory& memory, const cl_device_id id)
	{
		{
			Cache cache;
			cache.setSize( read<cl_ulong>( id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE));
			cache.setLineSize( read<cl_uint>( id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE));

			Cache::Type cacheType = Cache::tNone;
			switch( read<cl_device_mem_cache_type>( id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE))
			{
				case CL_READ_ONLY_CACHE: cacheType = Cache::tReadOnly; break;
				case CL_READ_WRITE_CACHE: cacheType = Cache::tReadWrite; break;
				default:;
			}

			cache.setType( cacheType);
			memory.setCache( cache);
		}

		memory.setSize( read<cl_ulong>( id, CL_DEVICE_GLOBAL_MEM_SIZE));
	}

	Image* readImageSupport( const cl_device_id id)
	{
		Image* image = nullptr;
		const auto imageSupport = read<cl_bool>( id, CL_DEVICE_IMAGE_SUPPORT) != 0;
		if ( imageSupport)
		{
			auto image = new Image();

			{
				Image2DMax img2D;
				img2D.setWidth( read<size_t>( id, CL_DEVICE_IMAGE2D_MAX_WIDTH));
				img2D.setHeight( read<size_t>( id, CL_DEVICE_IMAGE2D_MAX_HEIGHT));
				image->setMax2D( img2D);
			}

			{
				Image3DMax img3D;
				img3D.setWidth( read<size_t>( id, CL_DEVICE_IMAGE3D_MAX_WIDTH));
				img3D.setHeight( read<size_t>( id, CL_DEVICE_IMAGE3D_MAX_HEIGHT));
				img3D.setDepth( read<size_t>( id, CL_DEVICE_IMAGE3D_MAX_DEPTH));
				image->setMax3D( img3D);
			}

			image->setMaxBufferSize( read<size_t>( id, CL_DEVICE_IMAGE_MAX_BUFFER_SIZE));
			image->setMaxArraySize( read<size_t>( id, CL_DEVICE_IMAGE_MAX_ARRAY_SIZE));
		}

		return image;
	}

	void readLocalMemory( LocalMemory& memory, const cl_device_id id)
	{
		memory.setSize( read<cl_ulong>( id, CL_DEVICE_LOCAL_MEM_SIZE));
		memory.setType( read<cl_device_local_mem_type>( id, CL_DEVICE_LOCAL_MEM_TYPE) == CL_LOCAL ? LocalMemory::tLocal : LocalMemory::tGlobal);
	}

	void readWorkItemSizes( Device::SizeTArray& sizeArray, const size_t maxWorkItemDimensions, const cl_device_id id)
	{
		auto sizes = new size_t[ maxWorkItemDimensions];
		auto error = clGetDeviceInfo( id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * maxWorkItemDimensions, sizes, nullptr);
		if (error)
		{
			std::cerr << "error = " << error << '\n';
			throw Exception( CL_DEVICE_MAX_WORK_ITEM_SIZES, error);
		}

		sizeArray.assign( sizes, sizes + maxWorkItemDimensions);
		delete [] sizes;
	}

	void readNativeVectorWidths( VectorWidthArray widths, const cl_device_id id)		
	{
		widths.push_back( read<cl_uint>( id, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF));
	}

	void readPreferredVectorWidths( VectorWidthArray widths, const cl_device_id id)		
	{
		widths.push_back( read<cl_uint>( id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE));
		widths.push_back( read<cl_uint>( id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF));
	}

	void readAffinityDomains( AffinityDomainArray& affinityDomains, const cl_device_id id)
	{
		const auto value = read<cl_device_affinity_domain>( id, CL_DEVICE_PARTITION_AFFINITY_DOMAIN);
		if ( value & CL_DEVICE_AFFINITY_DOMAIN_NUMA)
		{
			affinityDomains.push_back( adNuma);
		}

		if ( value & CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE)
		{
			affinityDomains.push_back( adNuma);
		}

		if ( value & CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE)
		{
			affinityDomains.push_back( adNuma);
		}

		if ( value & CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE)
		{
			affinityDomains.push_back( adNuma);
		}

		if ( value & CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE)
		{
			affinityDomains.push_back( adNuma);
		}
	}

	void readPartition( Partition& partition, const cl_device_id id)
	{
		partition.setMaxSubDevices( read<cl_uint>( id, CL_DEVICE_PARTITION_MAX_SUB_DEVICES));

		{
			cl_device_partition_property array[ 3];
			{
				auto error = clGetDeviceInfo( id, CL_DEVICE_PARTITION_PROPERTIES, sizeof(cl_device_partition_property) * 3, array, nullptr);
				if (error)
				{
					std::cerr << "error = " << error << '\n';
					throw Exception( CL_DEVICE_PARTITION_PROPERTIES, error);
				}
			}
		}

		{
			AffinityDomainArray	affinityDomains;
			readAffinityDomains( affinityDomains, id);
			partition.setAffinityDomains( affinityDomains);
		}
	}

	void readQueueProperties( QueuePropertyArray& properties, const cl_device_id id)
	{
		auto value = read<cl_command_queue_properties>( id, CL_DEVICE_QUEUE_PROPERTIES);
		if ( value & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
		{
			properties.push_back( QueueProperty::qpOutOfOrder);
		}

		if ( value & CL_QUEUE_PROFILING_ENABLE)
		{
			properties.push_back( QueueProperty::qpProfiling);
		}
	}

	void readDeviceType( DeviceTypeArray& deviceType, const cl_device_id id)
	{
		const auto value = read<cl_device_type>( id, CL_DEVICE_TYPE);
		if ( value & CL_DEVICE_TYPE_DEFAULT)
		{
			deviceType.push_back( dtDefault);
		}

		if ( value & CL_DEVICE_TYPE_CPU)
		{
			deviceType.push_back( dtCPU);
		}

		if ( value & CL_DEVICE_TYPE_GPU)
		{
			deviceType.push_back( dtGPU);
		}

		if ( value & CL_DEVICE_TYPE_ACCELERATOR)
		{
			deviceType.push_back( dtAccelerator);
		}

		if ( value & CL_DEVICE_TYPE_CUSTOM)
		{
			deviceType.push_back( dtCustom);
		}
	}

	void read( Device& item, const cl_device_id id)
	{
		item.setAddressBits( read<cl_uint>( id, CL_DEVICE_ADDRESS_BITS));
		item.setAvailable( read<cl_bool>( id, CL_DEVICE_AVAILABLE) != 0);
		item.setCompilerAvailable( read<cl_bool>( id, CL_DEVICE_AVAILABLE) != 0);
		item.setLittleEndian( read<cl_bool>( id, CL_DEVICE_ENDIAN_LITTLE) != 0);
		item.setErrorCorrectionSupport( read<cl_bool>( id, CL_DEVICE_ERROR_CORRECTION_SUPPORT) != 0);

		{
			ExecutionCapabilityArray capabilities;
			readExecutionCapabilities( capabilities, id);
			item.setExecutionCapabilities( capabilities);
		}

		{ // extensions

			ExtensionArray extensions;
			readExtensions( extensions, id);
			item.setExtensions( extensions);
		}

		{
			GlobalMemory memory;
			readGlobalMemory( memory, id);
			item.setGlobalMemory( memory);
		}

		{
			FPCapabilityArray capabilities;
			readFP( capabilities, id, CL_DEVICE_SINGLE_FP_CONFIG);
			item.setSingleFpCapabilities( capabilities);
		}

		{
			FPCapabilityArray capabilities;
			readFP( capabilities, id, CL_DEVICE_DOUBLE_FP_CONFIG);
			item.setDoubleFpCapabilities( capabilities);
		}

		{
			#ifndef CL_DEVICE_HALF_FP_CONFIG
				#define CL_DEVICE_HALF_FP_CONFIG 0x1033
			#endif

			FPCapabilityArray capabilities;
			readFP( capabilities, id, CL_DEVICE_HALF_FP_CONFIG);
			item.setHalfFpCapabilities( capabilities);
		}

		item.setHostUnifiedMemory( read<cl_bool>( id, CL_DEVICE_HOST_UNIFIED_MEMORY) != 0);
		item.setImage( readImageSupport( id));
		item.setLinkerAvailable( read<cl_bool>( id, CL_DEVICE_LINKER_AVAILABLE) != 0);

		{
			LocalMemory memory;
			readLocalMemory( memory, id);
			item.setLocalMemory( memory);
		}

		item.setMaxClockFrequency( read<cl_uint>( id, CL_DEVICE_MAX_CLOCK_FREQUENCY));
		item.setMaxComputeUnits( read<cl_uint>( id, CL_DEVICE_MAX_COMPUTE_UNITS));
		item.setMaxConstantArgs( read<cl_uint>( id, CL_DEVICE_MAX_CONSTANT_ARGS));
		item.setMaxConstantBufferSize( read<cl_ulong>( id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE));
		item.setMaxMemoryAllocSize( read<cl_ulong>( id, CL_DEVICE_MAX_MEM_ALLOC_SIZE));
		item.setMaxParameterSize( read<size_t>( id, CL_DEVICE_MAX_PARAMETER_SIZE));
		item.setMaxReadImageArguments( read<size_t>( id, CL_DEVICE_MAX_READ_IMAGE_ARGS));
		item.setMaxWriteImageArguments( read<size_t>( id, CL_DEVICE_MAX_WRITE_IMAGE_ARGS));
		item.setMaxSamplers( read<cl_uint>( id, CL_DEVICE_MAX_SAMPLERS));
		item.setMaxWorkGroupSize( read<size_t>( id, CL_DEVICE_MAX_WORK_GROUP_SIZE));
		item.setMaxWorkItemDimensions( read<cl_uint>( id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS));

		{
			Device::SizeTArray sizeArray;
			readWorkItemSizes( sizeArray, item.maxWorkItemDimensions(), id);
			item.setiMaxWorkItemSizes( sizeArray);
		}

		item.setMemoryBaseAddressAlignment( read<cl_uint>( id, CL_DEVICE_MEM_BASE_ADDR_ALIGN));
		item.setMinDataTypeAlignSize( read<cl_uint>( id, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE));
		item.setVendorId( read<cl_uint>( id, CL_DEVICE_VENDOR_ID));

		{
			char buffer[ 64];
			readString( buffer, 64, id, CL_DEVICE_NAME);
			item.setName( buffer);

			readString( buffer, 64, id, CL_DEVICE_VERSION);
			item.setVersion( buffer);

			readString( buffer, 64, id, CL_DEVICE_VENDOR);
			item.setVendor( buffer);

			readString( buffer, 64, id, CL_DRIVER_VERSION);
			item.setDriverVersion( buffer);

			readString( buffer, 64, id, CL_DEVICE_PROFILE);
			item.setProfile( buffer);
		}

		{
			VectorWidthArray widths;
			widths.reserve( 8);
			readNativeVectorWidths( widths, id);
			item.setNativeVectorWidths( widths);

			widths.clear();
			readPreferredVectorWidths( widths, id);
			item.setPreferredVectorWidths( widths);
		}

		{
			Partition partition;
			readPartition( partition, id);	
			item.setPartition( partition);
		}

		item.setPrintfBufferSize( read<cl_uint>( id, CL_DEVICE_PRINTF_BUFFER_SIZE));
		item.setPreferredInteropUserSync( read<cl_bool>( id, CL_DEVICE_PREFERRED_INTEROP_USER_SYNC) != 0);

		{
			QueuePropertyArray properties;
			readQueueProperties( properties, id);
			item.setQueueProperties( properties);
		}

		item.setReferenceCount( read<cl_uint>( id, CL_DEVICE_REFERENCE_COUNT));

		{
			DeviceTypeArray deviceType;
			readDeviceType( deviceType, id);
			item.setType( deviceType);
		}
	}

	cl_int read( Platform& info, const cl_platform_id platformId)
	{
		char buffer[ 128];
		auto error = clGetPlatformInfo( platformId, CL_PLATFORM_VENDOR, 128, buffer, nullptr);
		if ( !error)
		{
			info.setVendor( buffer);
		}

		if ( !error)
		{
			error = clGetPlatformInfo( platformId, CL_PLATFORM_NAME, 128, buffer, nullptr);
			if ( !error)
			{
				info.setName( buffer);
			}
		}

		if ( !error)
		{
			error = clGetPlatformInfo( platformId, CL_PLATFORM_VERSION, 128, buffer, nullptr);
			if ( !error)
			{
				info.setVersion( buffer);
			}
		}

		return error;
	}
}



int run()
{
	using namespace std;
	cl_uint numPlatforms;//the NO. of platforms
	cl_platform_id platform = nullptr;//the chosen platform
	cl_int	status = clGetPlatformIDs(0, nullptr, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		cout<<"Error: Getting platforms!"<<endl;
		return 1;
	}

	/*For clarity, choose the first available platform. */
	if(numPlatforms > 0)
	{
		cl_platform_id* platforms = (cl_platform_id* )malloc(numPlatforms* sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, nullptr);
		platform = platforms[0];
		free(platforms);
	}

	using namespace Info;
	Platform info;
	read( info, platform);

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint				numDevices = 0;
	cl_device_id        *devices;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices);	
	if (numDevices == 0) //no GPU available.
	{
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, nullptr, &numDevices);	
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, devices, nullptr);
	}
	else
	{
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, nullptr);
	}


	{
		using namespace Info;
		Device device;
		read( device, devices[ 0]);
	}
	
	if (devices != nullptr)
	{
		free(devices);
		devices = nullptr;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	try
	{
		run();
	}
	catch( std::exception& ex)
	{
		const char* c = ex.what();
		c = 0;
	}
	catch(...)
	{
		int x = 0;
	}
	return 0;
}
