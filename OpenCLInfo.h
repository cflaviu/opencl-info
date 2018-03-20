#pragma once
#ifndef PCHDR
	#include <CL/cl.h>
	#include <vector>
	#include <string>

	typedef unsigned char       byte;
	typedef short               int2;
	typedef int                 int4;
	typedef long long           int8;
	typedef unsigned short      uint2;
	typedef unsigned int        uint4;
	typedef unsigned long long  uint8;
	typedef unsigned int        uint4;

	typedef unsigned int		uint;
	typedef unsigned long long	ulong;
#endif

namespace Info
{
	class Platform
	{
	public:
		const std::string name() const { return iName; }
		void setName( const std::string& item) { iName = item; }

		const std::string vendor() const { return iVendor; }
		void setVendor( const std::string& item) { iVendor = item; }

		const std::string version() const { return iVersion; }
		void setVersion( const std::string& item) { iVersion = item; }

	private:
		std::string iName;
		std::string iVendor;
		std::string iVersion;
	};

	typedef std::vector<std::string> StringArray;

	enum FPCapability
	{
		fpcDenorm,
		fpcInfNan,
		fpcRoundToNearest,
		fpcRoundToZero,
		fpcRoundToInf,
		fpcFMA,
		fpcCorrectlyRoundedDivideSqrt,
		fpcSoftFloat
	};

	typedef std::vector<FPCapability> FPCapabilityArray;


	enum ExecutionCapability
	{
		ecKernel,
		ecNativeKernel
	};

	typedef std::vector<ExecutionCapability> ExecutionCapabilityArray;

	typedef std::vector<std::string> ExtensionArray;

	class Memory
	{
	public:
		ulong size() const { iSize; }
		void setSize( const ulong value) { iSize = value; }

	protected:
		ulong iSize;
	};

	class Cache: public Memory
	{
	public:
		enum Type
		{
			tNone,
			tReadOnly,
			tReadWrite
		};

		Type type() const { iType; }
		void setType( const Type value) { iType = value; }

		uint lineSize() const { return iLineSize; }
		void setLineSize( const uint value) { iLineSize = value; }

	private:
		Type	iType;
		uint	iLineSize;
	};

	class GlobalMemory: public Memory
	{
	public:
		const Cache& cache() const { return iCache; }
		void setCache( const Cache& item) { iCache = item; }

	private:
		Cache iCache;
	};

	class LocalMemory: public Memory
	{
	public:
		enum Type
		{
			tLocal,
			tGlobal
		};

		Type type() const { iType; }
		void setType( const Type value) { iType = value; }

	private:
		Type iType;
	};

	class Image2DMax
	{
	public:
		size_t height() const { return iHeight; }
		void setHeight( const size_t value) { iHeight = value; }

		size_t width() const { return iWidth; }
		void setWidth( const size_t value) { iWidth = value; }

	private:
		size_t iHeight;
		size_t iWidth;
	};

	class Image3DMax: public Image2DMax
	{
	public:
		size_t depth() const { return iDepth; }
		void setDepth( const size_t value) { iDepth = value; }

	private:
		size_t iDepth;
	};

	class Image
	{
	public:
		const Image2DMax& max2D() const { return iMax2D; }
		void setMax2D( const Image2DMax& item) { iMax2D = item; }

		const Image3DMax& max3D() const { return iMax3D; }
		void setMax3D( const Image3DMax& item) { iMax3D = item; }

		const size_t maxBufferSize() const { return iMaxBufferSize; }
		void setMaxBufferSize( const size_t value) { iMaxBufferSize = value; }

		const size_t maxArraySize() const { return iMaxArraySize; }
		void setMaxArraySize( const size_t value) { iMaxArraySize = value; }

	private:
		Image2DMax	iMax2D;
		Image3DMax	iMax3D;
		size_t		iMaxBufferSize;
		size_t		iMaxArraySize;
	};

	typedef std::vector<uint> VectorWidthArray;

	enum AffinityDomain
	{
		adNuma,
		adL4Cache,
		adL3Cache,
		adL2Cache,
		adL1Cache
	};

	typedef std::vector<AffinityDomain> AffinityDomainArray;

	class Partition
	{
	public:
		uint maxSubDevices() const { return iPartitionMaxSubDevices; }
		void setMaxSubDevices( const uint value) { iPartitionMaxSubDevices = value; }

		uint properties() const { return iProperties; }
		void setProperties( const uint value) { iProperties = value; }

		const AffinityDomainArray& affinityDomains() const { return iAffinityDomains; }
		void setAffinityDomains( const AffinityDomainArray& item) { iAffinityDomains =  item; }

	private:
		AffinityDomainArray	iAffinityDomains;
		uint iPartitionMaxSubDevices;
		uint iProperties;
	};

	enum QueueProperty
	{
		qpOutOfOrder,
		qpProfiling
	};

	typedef std::vector<QueueProperty> QueuePropertyArray;

	enum DeviceType
	{
		dtDefault,
		dtCPU,
		dtGPU,
		dtAccelerator,
		dtCustom
	};

	typedef std::vector<DeviceType> DeviceTypeArray;

	class Device
	{
	public:
		Device():
			iImage( nullptr)
		{}

		~Device()
		{
			delete iImage;
		}

		const std::string name() const { return iName; }
		void setName( const std::string& item) { iName = item; }

		const std::string version() const { return iVersion; }
		void setVersion( const std::string& item) { iVersion = item; }

		const std::string vendor() const { return iVendor; }
		void setVendor( const std::string& item) { iVendor = item; }

		const std::string driverVersion() const { return iDriverVersion; }
		void setDriverVersion( const std::string& item) { iDriverVersion = item; }

		const std::string openClVersion() const { return iOpenClVersion; }
		void setOpenClVersion( const std::string& item) { iOpenClVersion = item; }

		uint vendorId() const { return iVendorId; }
		void setVendorId( const uint value) { iVendorId = value; }

		uint addressBits()  const { return iAddressBits; }
		void setAddressBits( const uint value) { iAddressBits = value; }

		bool isAvailable() const { return iAvailable; }
		void setAvailable( const bool value) { iAvailable = value; }

		bool isCompilerAvailable() const { return iCompilerAvailable; }
		void setCompilerAvailable( const bool value) { iCompilerAvailable = value; }

		bool isLinkerAvailable() const { return iLinkerAvailable; }
		void setLinkerAvailable( const bool value) { iLinkerAvailable = value; }

		bool isLittleEndian() const { return iLittleEndian; }
		void setLittleEndian( const bool value) { iLittleEndian = value; }

		bool hasHostUnifiedMemory() const { return iHostUnifiedMemory; }
		void setHostUnifiedMemory( const bool value) { iHostUnifiedMemory = value; }

		bool hasErrorCorrectionSupport() const { return iErrorCorrectionSupport; }
		void setErrorCorrectionSupport( const bool value) { iErrorCorrectionSupport = value; }

		bool preferredInteropUserSync() const { return iPreferredInteropUserSync; }
		void setPreferredInteropUserSync( const bool value) { iPreferredInteropUserSync = value; }

		const DeviceTypeArray& type() const { return iType; }
		void setType( const DeviceTypeArray& item) { iType =  item; }

		const StringArray& kernelds() const { return iKernels; }
		void setKernels( const StringArray& item) { iKernels =  item; }

		const ExecutionCapabilityArray& executionCapabilities() const { return iExecutionCapabilities; }
		void setExecutionCapabilities( const ExecutionCapabilityArray& item) { iExecutionCapabilities =  item; }

		const FPCapabilityArray& singleFpCapabilities() const { return iSingleFPCapabilities; }
		void setSingleFpCapabilities( const FPCapabilityArray& item) { iSingleFPCapabilities =  item; }

		const FPCapabilityArray& doubleFpCapabilities() const { return iDoubleFPCapabilities; }
		void setDoubleFpCapabilities( const FPCapabilityArray& item) { iDoubleFPCapabilities =  item; }

		const FPCapabilityArray& halfFpCapabilities() const { return iHalfFPCapabilities; }
		void setHalfFpCapabilities( const FPCapabilityArray& item) { iHalfFPCapabilities =  item; }

		const ExtensionArray& extensions() const { return iExtensions; }
		void setExtensions( const ExtensionArray& item) { iExtensions =  item; }

		const GlobalMemory& globalMemory() const { return iGlobalMemory; }
		void setGlobalMemory( const GlobalMemory& item) { iGlobalMemory = item; }

		const LocalMemory& localMemory() const { return iLocalMemory; }
		void setLocalMemory( const LocalMemory& item) { iLocalMemory = item; }

		const Image* image() const { return iImage; }
		void setImage( const Image* const item) { iImage = item; }

		uint maxClockFrequency() const { return iMaxClockFrequency; }
		void setMaxClockFrequency( const uint value) { iMaxClockFrequency = value; }

		uint maxComputeUnits() const { return iMaxComputeUnits; }
		void setMaxComputeUnits( const uint value) { iMaxComputeUnits = value; }

		uint maxConstantArgs() const { return iMaxConstantArgs; }
		void setMaxConstantArgs( const uint value) { iMaxConstantArgs = value; }

		ulong maxConstantBufferSize() const { return iMaxConstantBufferSize; }
		void setMaxConstantBufferSize( const ulong value) { iMaxConstantBufferSize = value; }

		ulong maxMemoryAllocSize() const { return iMaxMemoryAllocSize; }
		void setMaxMemoryAllocSize( const ulong value) { iMaxMemoryAllocSize = value; }

		ulong maxParameterSize() const { return iMaxParameterSize; }
		void setMaxParameterSize( const ulong value) { iMaxParameterSize = value; }

		uint maxReadImageArguments() const { return iMaxReadImageArguments; }
		void setMaxReadImageArguments( const uint value) { iMaxReadImageArguments = value; }

		uint maxWriteImageArguments() const { return iMaxWriteImageArguments; }
		void setMaxWriteImageArguments( const uint value) { iMaxWriteImageArguments = value; }

		uint maxSamplers() const { return iMaxSamplers; }
		void setMaxSamplers( const uint value) { iMaxSamplers = value; }

		size_t maxWorkGroupSize() const { return iMaxWorkGroupSize; }
		void setMaxWorkGroupSize( const size_t value) { iMaxWorkGroupSize = value; }

		uint maxWorkItemDimensions() const { return iMaxWorkItemDimensions; }
		void setMaxWorkItemDimensions( const uint value) { iMaxWorkItemDimensions = value; }

		typedef std::vector<size_t> SizeTArray;

		const SizeTArray& maxWorkItemSizes() const { return iMaxWorkItemSizes; }
		void setiMaxWorkItemSizes( const SizeTArray& item) { iMaxWorkItemSizes = item; }

		uint memoryBaseAddressAlignment() const { return iMemoryBaseAddressAlignment; }
		void setMemoryBaseAddressAlignment( const uint value) { iMemoryBaseAddressAlignment = value; }

		uint minDataTypeAlignSize() const { return iMinDataTypeAlignSize; }
		void setMinDataTypeAlignSize( const uint value) { iMinDataTypeAlignSize = value; }

		const std::string profile() const { return iProfile; }
		void setProfile( const std::string& item) { iProfile = item; }

		size_t profilingTimerResolution() const { return iProfilingTimerResolution; }
		void setProfilingTimerResolution( const size_t value) { iProfilingTimerResolution = value; } 

		const VectorWidthArray& nativeVectorWidths() const { return iNativeVectorWidths; }
		void setNativeVectorWidths( const VectorWidthArray& item) { iNativeVectorWidths =  item; }

		const VectorWidthArray& preferredVectorWidths() const { return iPreferredVectorWidths; }
		void setPreferredVectorWidths( const VectorWidthArray& item) { iPreferredVectorWidths =  item; }

		const Partition& partition() const { return iPartition; }
		void setPartition( const Partition& item) { iPartition = item; }

		size_t printfBufferSize() const { return iPrintfBufferSize; }
		void setPrintfBufferSize( const size_t value) { iPrintfBufferSize = value; }

		const QueuePropertyArray& queueProperties() const { return iQueueProperties; }
		void setQueueProperties( const QueuePropertyArray& item) { iQueueProperties = item; }

		uint referenceCount() const { return iReferenceCount; }
		void setReferenceCount( const uint value) { iReferenceCount = value; }

	private:
		FPCapabilityArray			iDoubleFPCapabilities;
		FPCapabilityArray			iSingleFPCapabilities;
		FPCapabilityArray			iHalfFPCapabilities;
		ExecutionCapabilityArray	iExecutionCapabilities;
		ExtensionArray				iExtensions;
		StringArray					iKernels;
		GlobalMemory				iGlobalMemory;
		LocalMemory					iLocalMemory;
		SizeTArray					iMaxWorkItemSizes;
		VectorWidthArray			iNativeVectorWidths;
		VectorWidthArray			iPreferredVectorWidths;
		QueuePropertyArray			iQueueProperties;
		DeviceTypeArray				iType;
		Partition					iPartition;
		std::string					iName;
		std::string					iVersion;
		std::string					iVendor;
		std::string					iDriverVersion;
		std::string					iProfile;
		std::string					iOpenClVersion;
		const Image*				iImage;
		ulong						iMaxConstantBufferSize;
		ulong						iMaxMemoryAllocSize;
		ulong						iMaxParameterSize;
		size_t						iMaxWorkGroupSize;
		size_t						iProfilingTimerResolution;
		size_t						iPrintfBufferSize;
		uint						iAddressBits;
		uint						iMaxClockFrequency;
		uint						iMaxComputeUnits;
		uint						iMaxConstantArgs;
		uint						iMaxReadImageArguments;
		uint						iMaxWriteImageArguments;
		uint						iMaxSamplers;
		uint						iMemoryBaseAddressAlignment;
		uint						iMaxWorkItemDimensions;
		uint						iMinDataTypeAlignSize;
		uint						iVendorId;
		uint						iReferenceCount;
		bool						iAvailable;
		bool						iCompilerAvailable;
		bool						iLinkerAvailable;
		bool						iLittleEndian;
		bool						iErrorCorrectionSupport;
		bool						iHostUnifiedMemory;
		bool						iPreferredInteropUserSync;
	};

	class Exception: public std::exception
	{
	public:
		Exception( const uint field, const int error):
			std::exception( "Failed to read OpenCL info"),
			iField( field),
			iError( error)
		{}

		uint field() const { return iField; }
		int error() const { return iError; }

	private:
		uint iField;
		int iError;
	};

	void read( Device& item, const cl_device_id id);
	cl_int read( Platform& info, const cl_platform_id platformId);
}
