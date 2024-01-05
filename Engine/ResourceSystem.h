#pragma once
#include "Subsystem.h"

namespace Toffee::Internal {

	// Interface base class to resource management system.
	class ResourceSystem : public Subsystem {
	public:
		ResourceSystem() { type_ = SubsytemType::RESOURCE; }
		// Set the current archive for loading resources from.
		virtual void SetArchive(const std::string& path) = 0;
		// Mount a real location to a virtual location.
		// The real location may be a local folder or in the archive.
		virtual void Mount(const std::string& real_location, const std::string& virtual_location) = 0;
		// Unmount a real location from a virtual location.
		virtual void UnMount(const std::string& real_location, const std::string& virtual_location) = 0;
		///<summary>Load and store a resource.</summary>
		///<param name="path">The virtual file path.</param>
		///<param name="name">Give the resource a name.</param>
		///<param name="async">Whether to load asynchronously or not.</param>
		virtual void Load(const std::string& path, const std::string& name, bool async = true) = 0;
		// Unload a resource
		virtual void UnLoad(const std::string& name) = 0;
		// Returns whether any resources are currently loading.
		virtual bool Loading() = 0;
		// Retrieve a resource.
		virtual void* GetResource(const std::string& name) = 0;

	protected:
		friend class Core;
		friend class UserInterfaceSystem;
		virtual void Update() = 0;
		virtual int CreateFontTexture(std::string font, std::string text, int pt_size, int& wrap, float r, float g, float b) = 0;
	};
}

