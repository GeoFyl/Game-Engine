#pragma once
#include "ResourceSystem.h"
#include "Folder.h"
#include "Components.h"
#include <queue>
#include <unzip.h>
#include <boost/unordered/unordered_flat_map.hpp>
#include <atomic>
#include <mutex>

// Includes for different resource types - tightly coupled to subsystem implementations.
// If swapping out a subsytem which uses its own data structures for resources, must remember to make the necessary changes here (and the typedefs below).
#include <soloud_wav.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "RenderSystem_SDL_GL.h"

namespace Engine::Internal {
	typedef std::unique_ptr<Folder> FolderPtr;

	struct ResourcePath {
		std::string path;
		std::string file_name;
	};

	typedef std::string Text;
	typedef SoLoud::Wav Audio;
	typedef Engine::Components::Mesh Mesh;
	typedef std::vector<char> RawImage;
	typedef GLuint Texture;
	typedef TTF_Font* Font;

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
		~ResourceSystem_MiniZip();

		//Inherited from Subsystem interface

		int	Initialise() final;
		int	Shutdown() final;

		//Inherited from ResourceSystem interface
		void Update();
		void SetArchive(const std::string& path);
		void Mount(const std::string& real_location, const std::string& virtual_location);
		void UnMount(const std::string& real_location, const std::string& virtual_location);
		void Load(const std::string& path, const std::string& name, bool async = true);
		void UnLoad(const std::string& name);
		bool Loading() { return loading_; }
		void* GetResource(const std::string& name);

		int CreateFontTexture(std::string font, std::string text, int pt_size, int& wrap, float r, float g, float b);
	protected:
		// Virtual file system
		ResourcePath GetPathAndFile(std::string filePath);
		std::vector<FolderPtr>* ResolvePhysicalDir(const std::string& virtualDir, std::string& fileName);
		unzFile zip_handle_;
		std::string zip_path_;
		std::unordered_map<std::string, std::vector<FolderPtr>> virtual_file_system_;
		std::vector<char> read_buffer_ = std::vector<char>(5242880); //Allocate 5MB to begin with
		std::mutex buffer_mutex_;

		// Loading
		void LoadFile(const std::string& path, const std::string& name);
		bool CreateMesh(Mesh& mesh, const char* buffer, unsigned int size);
		bool CreateTexture(std::string name, const char* buffer, unsigned int size);
		SDL_RWops* CreateFontTTF(Font& font, const char* buffer, unsigned int size);
		SDL_RWops* test_;

		std::atomic<bool> loading_ = false;
		std::queue<std::pair<std::string, std::string>> load_queue_;
		std::mutex load_mutex_;


		// Resource management

		std::unordered_map<std::string, ResourceType> resource_lookup_;
		boost::unordered::unordered_flat_map<std::string, std::string> text_pool_;
		boost::unordered::unordered_flat_map<std::string, Audio> audio_pool_;
		boost::unordered::unordered_flat_map<std::string, Mesh> mesh_pool_;
		boost::unordered::unordered_flat_map<std::string, RawImage> raw_image_pool_;
		boost::unordered::unordered_flat_map<std::string, Texture> texture_pool_;
		boost::unordered::unordered_flat_map<std::string, Font> font_pool_;
	};

}