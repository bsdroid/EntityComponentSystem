/*
	Author : Tobias Stein
	Date   : 7th September, 2017
	File   : ComponentManager.cpp
	
	Manages all component container.

	All Rights Reserved. (c) Copyright 2016.
*/


#include "ComponentManager.h"

namespace ECS {

	ComponentManager::ComponentManager() : 
		ILogSubscriber("ComponentManager"), 
		m_ComponentContainerRegistry(util::StaticTypeCounter<IComponent>::Get())
	{
		LogInfo("Initialize ComponentManager!");
	}

	ComponentManager::~ComponentManager()
	{
		for (auto cc : this->m_ComponentContainerRegistry)
		{
			cc->~IComponentContainer();
			cc = nullptr;
		}


		LogInfo("Realse ComponentManager!");
	}

} // namespace ECS