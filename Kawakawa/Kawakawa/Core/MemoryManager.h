#pragma once

#include "Allocator.h"
#include "../Interface/IRuntimeModule.h"

// http://allenchou.net/2013/05/memory-management-part-2-of-3-c-style-interface/

namespace Kawaii::Core
{
    class MemoryManager : public IRuntimeModule
    {
    public:
        template <typename T, typename... Arguments>
        T* New(Arguments... parameters) {
            return new (Allocator(sizeof(T))) T(parameters...);
        }

        template <typename T>
        void Delete(T* p) {
            reinterpret_cast<T*>(p)->~T();
            Free(p, sizeof(T));
        }

        int  Initialize() final;
        void Finalize() final;
        void Tick() final;

        void* Allocate(size_t size);
        void* Allocate(size_t size, size_t alignment);
        void  Free(void* p, size_t size);

    private:
        inline static size_t* m_BlockSizeLookup = nullptr;
        inline static Allocator* m_Allocators = nullptr;
        static Allocator* LookUpAllocator(size_t size);
        bool                     m_Initialized = false;
    };

}  

namespace Kawaii
{
    extern std::unique_ptr<Core::MemoryManager> g_MemoryManager;
}