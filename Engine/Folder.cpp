#include "pch.h"
#include "Folder.h"
#include <iostream>
#include <fstream>

Folder::Folder(const unzFile& zip_handle, const std::string& real_root) :
    zip_handle_{ zip_handle }, real_root_{ real_root }
{
}

int Folder::GetFile(const std::string& path, std::vector<char>& buffer)
{
    // First check if the file exists locally 
    std::ifstream file(("../Game/" + real_root_ + path).c_str(), std::ios::binary | std::ios::ate);
    if (file.good()) {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        if (size > buffer.size()) buffer.resize(size);

        if (file.read(buffer.data(), size))
        {
            file.close();
            printf("READ: %d bytes \n", size);
            return size;
        }
    }
    // If not, check if it's in the archive
    else {
        int result = unzLocateFile(zip_handle_, (real_root_ + path).c_str(), 1);
        if (result == UNZ_OK) {
            unz_file_info64 info;
            unzGetCurrentFileInfo64(zip_handle_, &info, NULL, NULL, NULL, NULL, NULL, NULL);
            if (info.uncompressed_size > buffer.size()) buffer.resize(info.uncompressed_size);

            //Open the file
            result = unzOpenCurrentFile(zip_handle_);

            int totalRead = 0;
            int read = 0;
            while ((read = unzReadCurrentFile(zip_handle_, buffer.data(), buffer.size())) > 0) {
                //blocking
                //Read all the bytes!
                totalRead += read;
                printf("READ: %d bytes \n", read);
            }

            unzCloseCurrentFile(zip_handle_);

            if (read == 0) { // eof
                return totalRead;
            }
        }
    }

    return 0;
}



