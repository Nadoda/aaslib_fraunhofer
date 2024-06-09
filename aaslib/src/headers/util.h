#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <filesystem>
#include<nlohmann/json.hpp>
#include <sstream>
#include <objbase.h>  // For CoCreateGuid and StringFromGUID2
#include <combaseapi.h> // For CoCreateGuid and StringFromGUID2
#pragma comment(lib, "wininet.lib")
using json = nlohmann::json;
namespace fs = std::filesystem;


 std::wstring stringToWstring(std::string st);

 std::wstring createDirectory(const std::wstring directoryPath);

 bool downloadFile(const std::wstring& url, const std::wstring& filename, const std::wstring& directory_path);

 std::wstring getTmpPath();

 std::wstring getfileWithExtension(std::string filepath);

 std::wstring getRandomGUIDString();

 json httpGetInvoke(std::wstring& url);