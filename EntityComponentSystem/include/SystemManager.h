/*
	Author : Tobias Stein
	Date   : 13th July, 2016
	File   : SystemManager.h
	
	Manager class for systems

	All Rights Reserved. (c) Copyright 2016.
*/

#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__

#define ECS_SYSTEM_MEMORY_CAPACITY	8192 // 8Mb

#include <vector>
#include <assert.h>

#include "ECS.h"

#include "Memory/ECSMM.h"
#include "Log/ILogSubscriber.h"

#include "Memory/Allocator/LinearAllocator.h"
#include "util/StaticTypeCounter.h"



namespace ECS
{

	// forward declaration
	class ISystem;

	class ECS_API SystemManager : protected Log::ILogSubscriber, protected Memory::Internal::GlobalMemoryUser
	{

	private:

		using SystemArray		= std::vector<ISystem*>;
		using SystemAllocator	= Memory::Allocator::LinearAllocator;

		static const size_t SYSTEM_MEMORY_CAPACITY = ECS_SYSTEM_MEMORY_CAPACITY;

		SystemAllocator*	m_SystemAllocator;

		SystemArray			m_Systems;

		// This class is not inteeded to be initialized
		SystemManager();
		SystemManager(const SystemManager&) = delete;
		SystemManager& operator=(SystemManager&) = delete;	

	public:

		static inline SystemManager& GetInstance()
		{
			static SystemManager INSTANCE;
			return INSTANCE;
		}

		~SystemManager();
		
		template<class T, class... ARGS>
		inline T* AddSystem(ARGS&&... systemArgs)
		{
			T* system = nullptr;

			void* pSystemMem = this->m_SystemAllocator->allocate(sizeof(T), alignof(T));
			if (pSystemMem != nullptr)
			{
				// create new system
				system = new (pSystemMem)T(std::forward<ARGS>(systemArgs)...);
				m_Systems[T::STATIC_SYSTEM_TYPE_ID] = system;

				LogInfo("System \'%s\' (%d bytes) created.", typeid(T).name(), sizeof(T));
			}
			else
			{
				LogError("Unable to create system \'%s\' (%d bytes).", typeid(T).name(), sizeof(T));
				assert(true);
			}

			return system;
		}

		template<class T>
		inline T* GetSystem() const
		{
			assert(T::STATIC_SYSTEM_TYPE_ID < util::StaticTypeCounter<ISystem>::Get() && "Unknown system!");
			return reinterpret_cast<T*>(this->m_Systems[T::STATIC_SYSTEM_TYPE_ID]);
		}
	};

} // namespace ECS

#endif // __SYSTEM_MANAGER_H__