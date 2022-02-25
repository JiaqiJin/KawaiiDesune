#pragma once

#include "Allocator.h"
#include "../../Interface/IModule.h"
#include "../../Utils/Singleton.h"

namespace Excalibur
{

	class MemoryManager : public IModule
	{
	public:
		template<class T, typename... Arguments>
		T* New(Arguments... parameters)
		{
			return new (Allocate(sizeof(T))) T(parameters...);
		}

		template<class T>
		void Delete(T* p)
		{
			p->~T();
			Free(p, sizeof(T));
		}
		MemoryManager() = default;
		virtual ~MemoryManager() = default;
		virtual int Initialize()  ;
		virtual void Finalize()  ;
		void* Allocate(size_t size);

		void Free(void* p, size_t size);

	private:
		MemoryManager(const MemoryManager& m) = default;
		MemoryManager& operator=(const MemoryManager&) = default;

		static size_t* m_pBlockSizeLookup;
		static Allocator * m_pAllocators;
		static Allocator* LookupAllocator(size_t size);
		
	};

}
