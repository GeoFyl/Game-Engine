#pragma once
#include "ResourceSystem.h"
#include "Folder.h"
#include "Components.h"
#include <queue>
#include <unzip.h>
#include <boost/unordered/unordered_flat_map.hpp>
#include <atomic>
#include <mutex>

// Includes for different resource types - quite tightly coupled to specific subsystem implementations.
// If swapping out a subsytem which uses its own data structures for resources, must remember to make the necessary changes here (and the typedefs below).
#include <soloud_wav.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "RenderSystem_SDL_GL.h"

namespace Toffee::Internal {
	typedef std::unique_ptr<Folder> FolderPtr;

	// A directory and file name
	struct ResourcePath {
		std::string path;
		std::string file_name;
	};

	
	typedef std::string Text;
	typedef SoLoud::Wav Audio;
	typedef Toffee::Components::Mesh Mesh;
	typedef std::vector<char> RawFile;
	typedef GLuint Texture;
	struct Font {
		RawFile data;
		TTF_Font* font;
	};

	enum class ResourceType {
		TEXT,
		AUDIO,
		MESH,
		TEXTURE,
		FONT
	};

	class ResourceSystem_MiniZip : public ResourceSystem {
	public:
		ResourceSystem_MiniZip();

	private:
		//Inherited from Subsystem interface

		int	Initialise() final;
		int	Shutdown() final;

		//Inherited from ResourceSystem interface

		void Update() final;
		void SetArchive(const std::string& path) final;
		void Mount(const std::string& real_location, const std::string& virtual_location) final;
		void UnMount(const std::string& real_location, const std::string& virtual_location) final;
		void Load(const std::string& path, const std::string& name, bool async = true) final;
		void UnLoad(const std::string& name) final;
		bool Loading() final { return loading_; }
		void* GetResource(const std::string& name) final;


		// --Virtual file system-- adapted (quite heavily) from the lab example

		// Split a path into a virtual directory and file name.
		ResourcePath GetPathAndFile(std::string filePath);
		// Returns a vector of possible real locations.
		std::vector<FolderPtr>* ResolvePhysicalDir(const std::string& virtualDir, std::string& fileName);
		// Handle to the current archive
		unzFile zip_handle_;
		std::string zip_path_;
		std::unordered_map<std::string, std::vector<FolderPtr>> virtual_file_system_;
		std::vector<char> read_buffer_ = std::vector<char>(5242880); //Allocate 5MB to begin with
		std::mutex buffer_mutex_;

		// --Loading--

		// Load a file from a real location.
		void LoadFile(const std::string& path, const std::string& name);
		// Create a mesh from the provided buffer.
		bool CreateMesh(Mesh& mesh, const char* buffer, unsigned int size);
		// Create a texture from the provided buffer.
		bool CreateTexture(std::string name, const char* buffer, unsigned int size);
		// Create a font file from the provided buffer.
		SDL_RWops* CreateFontTTF(Font& font, const char* buffer, unsigned int size);
		// Create a texture using the provided input data.
		int CreateFontTexture(std::string font, std::string text, int pt_size, int& wrap, float r, float g, float b);

		std::atomic<bool> loading_ = false;
		std::queue<std::pair<std::string, std::string>> load_queue_;
		std::mutex load_mutex_;


		// --Resource management--

		// Resources are pooled in flat maps (preallocated buckets),
		// with a lookup table to figure out which pool a given resource is in.
		std::unordered_map<std::string, ResourceType> resource_lookup_;
		boost::unordered::unordered_flat_map<std::string, std::string> text_pool_;
		boost::unordered::unordered_flat_map<std::string, Audio> audio_pool_;
		boost::unordered::unordered_flat_map<std::string, Mesh> mesh_pool_;
		boost::unordered::unordered_flat_map<std::string, Texture> texture_pool_;
		boost::unordered::unordered_flat_map<std::string, Font> font_pool_;

		// OpenGl textures must be created on the same thread as is running the OpenGL context,
		// so the raw data is pushed to a queue for later processing
		std::queue<std::pair<std::string, RawFile>> raw_image_queue_; 
		std::mutex queue_mutex_;
	};

}