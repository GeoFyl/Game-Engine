#pragma once
#include <unzip.h>

// Represents a real directory.
class Folder {
public:
	Folder(const unzFile& zip_handle, const std::string& real_root);
	// Reads file into supplied buffer.
	int GetFile(const std::string& path, std::vector<char>& buffer);
	const std::string& GetRoot() { return real_root_; }

private:
	unzFile zip_handle_;
	std::string real_root_;
};

