#include "pch.h"
#include "ResourceSystem_MiniZip.h"
#include "SystemsLocator.h"
#include <thread>
#include <array>
#include "Quad.h"
#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"

using namespace Engine::Internal;

ResourceSystem_MiniZip* resource_minizip = new ResourceSystem_MiniZip;

ResourceSystem_MiniZip::ResourceSystem_MiniZip() {
	ProvideSystem(this);
	resource_minizip = nullptr;
}

ResourceSystem_MiniZip::~ResourceSystem_MiniZip() {
}

int ResourceSystem_MiniZip::Initialise()
{
	resource_lookup_["Quad"] = ResourceType::MESH;
	Quad::CreateQuad(mesh_pool_["Quad"]);

	resource_lookup_["Plane"] = ResourceType::MESH;
	Plane::CreatePlane(mesh_pool_["Plane"]);

	resource_lookup_["Cube"] = ResourceType::MESH;
	Cube::CreateCube(mesh_pool_["Cube"]);

	resource_lookup_["Sphere"] = ResourceType::MESH;
	Sphere::CreateSphere(mesh_pool_["Sphere"]);



	return true;
}

int ResourceSystem_MiniZip::Shutdown()
{
	return 0;
}

void Engine::Internal::ResourceSystem_MiniZip::Update() {
	for (auto raw = raw_image_pool_.begin(); raw != raw_image_pool_.end(); raw++) {
		if (CreateTexture(raw->first, raw->second.data(), raw->second.size())) {
			printf("Loaded texture %s\n", raw->first.c_str());
		}
		else {
			printf("Could not parse file %s\n", raw->first.c_str());
		}
	}
	raw_image_pool_.clear();
}

void ResourceSystem_MiniZip::SetArchive(const std::string& path) {
    zip_handle_ = unzOpen64(("../Game/" + path).c_str());
    int result = unzOpenCurrentFile(zip_handle_);
	if (result != UNZ_OK) {
		std::cout << "Could not set archive. Error: " << result << "\n";
		return;
	}
	unzCloseCurrentFile(zip_handle_);
	zip_path_ = path;
}

void ResourceSystem_MiniZip::Mount(const std::string& real_location, const std::string& virtual_location) {
	std::string real_path = real_location;
	std::string virtual_path = virtual_location;

	if (real_path.length()) {
		if (!(real_path.compare(real_path.length() - 1, 1, "/") == 0))
		{
			//Did we forget to end with a "/"? Add one here.
			real_path += "/";
		}
	}
	if (virtual_path.length()) {
		if (!(virtual_path.compare(virtual_path.length() - 1, 1, "/") == 0))
		{
			//Did we forget to end with a "/"? Add one here.
			virtual_path += "/";
		}
	}

	virtual_file_system_[virtual_path].push_back(std::move(std::make_unique<Folder>(zip_handle_, real_path)));
}

void ResourceSystem_MiniZip::UnMount(const std::string& real_location, const std::string& virtual_location) {
	std::string real_path = real_location;
	std::string virtual_path = virtual_location;

	if (real_path.length()) {
		if (!(real_path.compare(real_path.length() - 1, 1, "/") == 0))
		{
			//Did we forget to end with a "/"? Add one here.
			real_path += "/";
		}
	}
	if (virtual_path.length()) {
		if (!(virtual_path.compare(virtual_path.length() - 1, 1, "/") == 0))
		{
			//Did we forget to end with a "/"? Add one here.
			virtual_path += "/";
		}
	}

	std::vector<FolderPtr>* virtual_folder = &virtual_file_system_[virtual_path];
	auto it = virtual_folder->begin();
	while ((*it)->GetRoot() != real_path) {
		it++;
	}
	virtual_folder->erase(it);
}

void ResourceSystem_MiniZip::Load(const std::string& path, const std::string& name, bool async) {
	if (resource_lookup_.count(name)) {
		printf("Resource %s already exists!\n", name.c_str());
	}

	if (!async) {
		LoadFile(path, name);
		return;
	}

	load_mutex_.lock();
	load_queue_.push({path, name});
	load_mutex_.unlock();

	if (!loading_) {
		loading_ = true;
		std::thread([this] {
			while (loading_) {
				load_mutex_.lock();
				if (load_queue_.empty()) {
					loading_ = false;
					load_mutex_.unlock();
					return;
				}
				std::pair<std::string, std::string> file = std::move(load_queue_.front());
				load_queue_.pop();
				load_mutex_.unlock();
				LoadFile(file.first, file.second);
			}
		}).detach();
	}
}

void ResourceSystem_MiniZip::LoadFile(const std::string& path, const std::string& name) {
	std::string file;
	std::vector<FolderPtr>* resolved = ResolvePhysicalDir(path, file);
	if (resolved == nullptr) return;

	int end = 0;
	for (end = 0; end < file.size(); end++)	{
		if (file[end] == '.') {
			break;
		}
	}
	std::string extension = file.substr(end+1, file.length());
	if (extension == "txt") resource_lookup_[name] = ResourceType::TEXT;
	else if (extension == "wav" || extension == "mp3") resource_lookup_[name] = ResourceType::AUDIO;
	else if (extension == "obj") resource_lookup_[name] = ResourceType::MESH;
	else if (extension == "png" || extension == "jpg" || extension == "jpeg") resource_lookup_[name] = ResourceType::TEXTURE;
	else if (extension == "ttf") resource_lookup_[name] = ResourceType::FONT;
	else {
		printf("Unknown file format\n");
		return;
	}

	//Pass in the file to our physical file system, and let it get the file for us (if it exists)
	unsigned int size = 0;
	buffer_mutex_.lock();
	for (int i = 0; i < resolved->size(); i++) {
		size = resolved->at(i)->GetFile(file, read_buffer_);
		if (size > 0) {
			std::cout << "Loaded file " << file.c_str() << "\n";
			break;
		}
	}
	if (size == 0) {
		std::cout << "Could not load file " << file.c_str() << "\n";
		buffer_mutex_.unlock();
		return;
	}

	switch (resource_lookup_[name]) {
	case ResourceType::TEXT:
		text_pool_[name] = std::move(std::string(read_buffer_.data(), size));
		printf("Loaded file %s\n", name.c_str());
		break;
	case ResourceType::AUDIO:
		if(audio_pool_[name].loadMem(reinterpret_cast<unsigned char*>(read_buffer_.data()), size, true) == 3) printf("Loaded audio %s\n", name.c_str());
		else printf("Could not create audio source %s\n", name.c_str());
		break;
	case ResourceType::MESH:
		if (CreateMesh(mesh_pool_[name], read_buffer_.data(), size)) printf("Loaded mesh %s\n", name.c_str());
		else printf("Could not parse file %s\n", name.c_str());
		break;
	case ResourceType::TEXTURE:
		read_buffer_.shrink_to_fit();
		raw_image_pool_[name] = std::move(RawImage(read_buffer_.begin(), read_buffer_.begin() + size));
		break;
	case ResourceType::FONT:
		//std::string temp = name;
		//font_pool_[temp] = nullptr;
		test_ = CreateFontTTF(font_pool_[name], read_buffer_.data(), size);
		printf("hi\n");
		break;
	}
	buffer_mutex_.unlock();
}

//Adapted from my CMP203 coursework from last year
bool ResourceSystem_MiniZip::CreateMesh(Mesh& mesh, const char* buffer, unsigned int size) {
	std::vector<std::array<float, 3>> verts;
	std::vector<std::array<float, 3>> norms;
	std::vector<std::array<float, 3>> texs;
	std::vector<unsigned int> faces;

	std::stringstream file;
	std::copy(buffer, buffer + size, std::ostreambuf_iterator<char>(file));

	while (true)
	{
		std::string line_header;

		// Read first word of the line
		file >> line_header;
		if (file.eof())
		{
			break; // exit loop
		}
		else // Parse
		{
			if (line_header.compare("v") == 0) // Vertex
			{
				std::array<float, 3> vertex;
				file >> vertex[0] >> vertex[1] >> vertex[2];
				verts.push_back(vertex);
			}
			else if (line_header.compare("vt") == 0) // Tex Coord
			{
				std::array<float, 3> uv;
				file >> uv[0] >> uv[1] >> uv[2];
				texs.push_back(uv);
			}
			else if (line_header.compare("vn") == 0) // Normal
			{
				std::array<float, 3> normal;
				file >> normal[0] >> normal[1] >> normal[2];
				norms.push_back(normal);
			}
			else if (line_header.compare("f") == 0) // Face
			{
				//std::array<unsigned int, 9> face;
				char c;
				unsigned int face[9];
				if (file >> face[0] >> c >> face[1] >> c >> face[2] >> face[3] >> c >> face[4] >> c >> face[5] >> face[6] >> c >> face[7] >> c >> face[8]) {
					for (int i = 0; i < 9; i++)
					{
						faces.push_back(face[i]);
					}					
				}
				else {
					// Parser error, or not triangle faces
					return false;
				}
			}
		}
	}

#pragma region unroll_data
	// "Unroll" the loaded obj information into a list of triangles.

	for (int i = 0; i < faces.size(); i += 3) {
		mesh.vertex.push_back(std::move(verts[faces[i] - 1][0]));
		mesh.vertex.push_back(std::move(verts[faces[i] - 1][1]));
		mesh.vertex.push_back(std::move(verts[faces[i] - 1][2]));
	
		mesh.texCoords.push_back(std::move(texs[faces[i + 1] - 1][0]));
		mesh.texCoords.push_back(std::move(texs[faces[i + 1] - 1][1]));

		mesh.normals.push_back(std::move(norms[faces[i + 2] - 1][0]));
		mesh.normals.push_back(std::move(norms[faces[i + 2] - 1][1]));
		mesh.normals.push_back(std::move(norms[faces[i + 2] - 1][2]));
	}

#pragma endregion unroll_data

	// Once data has been sorted clear read data (which has been copied and are not longer needed).
	verts.clear();
	norms.clear();
	texs.clear();
	faces.clear();

	return true;
}

bool ResourceSystem_MiniZip::CreateTexture(std::string name, const char* buffer, unsigned int size)
{
	SDL_RWops* rw = SDL_RWFromConstMem(buffer, size);
	SDL_Surface* surface = IMG_Load_RW(rw, 1);

	if (surface == NULL) return false;

	Texture& texture = texture_pool_[name];
	
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 1, SDL_ISPIXELFORMAT_ALPHA(surface->format->format) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 1,  GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);
	rw->close(rw);
	return true;
}

SDL_RWops* ResourceSystem_MiniZip::CreateFontTTF(Font& font, const char* buffer, unsigned int size) {
	SDL_RWops* rw = SDL_RWFromConstMem(buffer, size);
	font = TTF_OpenFontRW(rw, 1, 12);
	/*std::string test = "bruh";
	SDL_Color colour = {  255,  255, 255 };
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, test.c_str(), colour, 203);
	printf("%s\n", SDL_GetError());*/
	return rw;
}

int Engine::Internal::ResourceSystem_MiniZip::CreateFontTexture(std::string font, std::string text, int pt_size, int& width, float r, float g, float b) {
	SDL_Color colour = { r * 255, g * 255, b * 255 };

	TTF_Font* font_file = reinterpret_cast<Font>(GetResource(font));
	if (!font_file) return -1;
	if (TTF_SetFontSize(font_file, pt_size) < 0) return -1;
	//SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font_pool_[font], text.c_str(), colour, wrap);
	
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font_file, text.c_str(), colour, width);
	printf("%s\n", SDL_GetError());
	if (surface == NULL) return -1;

	int height = surface->h;
	width = surface->w;

	Texture& texture = texture_pool_[text];
	resource_lookup_[text] = ResourceType::TEXTURE;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, surface->pitch / surface->format->BytesPerPixel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, surface->pixels);
	
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	SDL_FreeSurface(surface);

	return height;
}

void ResourceSystem_MiniZip::UnLoad(const std::string& name) {
	try {
		switch (resource_lookup_.at(name)) {
		case ResourceType::TEXTURE:
			glDeleteTextures(1, &texture_pool_[name]);
			texture_pool_.erase(texture_pool_.find(name));
			break;
		case ResourceType::TEXT:
			text_pool_.erase(text_pool_.find(name));
			break;
		case ResourceType::AUDIO:
			audio_pool_.erase(audio_pool_.find(name));
			break;
		}
	}
	catch (...) {
		printf("Resource %s does not exist\n", name.c_str());
	}
}

void* ResourceSystem_MiniZip::GetResource(const std::string& name)
{
	try {
		switch (resource_lookup_.at(name)) {
		case ResourceType::TEXT:
			return &text_pool_[name];
			break;
		case ResourceType::AUDIO:
			return &audio_pool_[name];
			break;
		case ResourceType::MESH:
			return &mesh_pool_[name];
			break;
		case ResourceType::TEXTURE:
			return &texture_pool_[name];
			break;
		case ResourceType::FONT:
			return font_pool_[name];
		}
	}
	catch (...) {
		printf("Resource %s does not exist\n", name.c_str());
		return nullptr;
	}
}

ResourcePath ResourceSystem_MiniZip::GetPathAndFile(std::string filePath)
{
	ResourcePath result;

	int endOfPathIndex = 0;
	//Here we split the filePath into the first section (the key, which helps us find the physical location) 
	//and everything else (the path to the file after we find that physical location)
	for (int c = 0; c < filePath.size(); c++)
	{
		if (filePath[c] == '/') {
			endOfPathIndex = c;
			break;
		}
	}

	result.path = filePath.substr(0, endOfPathIndex + 1);
	result.file_name = filePath.substr(result.path.length(), filePath.length());

	return result;
}

std::vector<FolderPtr>* ResourceSystem_MiniZip::ResolvePhysicalDir(const std::string& virtual_dir, std::string& file_name)
{
	std::string physical_dir;

	//Go split the path into the virtual location, and the file we want to find
	ResourcePath path_and_file = GetPathAndFile(virtual_dir);

	std::string virtual_path = path_and_file.path;
	file_name = path_and_file.file_name;

	if (virtual_file_system_.count(virtual_path)) {
		//If we found our fileSystem, return it
		return &virtual_file_system_[virtual_path];
	}
	printf("Could not load file %s - unknown virtual directory\n", file_name.c_str());
	return nullptr;
}

