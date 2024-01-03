#pragma once
#include <unzip.h>

class Folder {
public:
	Folder(const unzFile& zip_handle, const std::string& real_root);
	int GetFile(const std::string& path, std::vector<char>& buffer);
	const std::string& GetRoot() { return real_root_; }
	//void Initialise();
protected:
	unzFile zip_handle_;
	std::string real_root_;


};

