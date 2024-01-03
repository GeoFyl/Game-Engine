#pragma once
#include "Subsystem.h"

namespace Engine::Internal {
	class ResourceSystem : public Subsystem {
	public:
		ResourceSystem() { type_ = SubsytemType::RESOURCE; }
		virtual void Update() = 0;
		virtual void SetArchive(const std::string& path) = 0;
		virtual void Mount(const std::string& real_location, const std::string& virtual_location) = 0;
		virtual void UnMount(const std::string& real_location, const std::string& virtual_location) = 0;
		virtual void Load(const std::string& path, const std::string& name, bool async = true) = 0;
		virtual void UnLoad(const std::string& name) = 0;
		virtual bool Loading() = 0;
		virtual void* GetResource(const std::string& name) = 0;
	};
}

