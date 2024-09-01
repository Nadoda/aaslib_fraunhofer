#include "include/util.h"

std::wstring stringToWstring(std::string str) {

    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

    // Allocate a buffer for the conversion
    wchar_t* buffer = new wchar_t[size];

    // Convert std::string to std::wstring
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, size);

    // Create std::wstring from the converted buffer
    std::wstring wstr(buffer);

    // Clean up allocated buffer
    delete[] buffer;

    return wstr;
}

std::wstring createDirectory(const std::wstring directoryPath)
{

    /*std::wstring fullPath = baseDirectory + filePath;*/

    // Create a string stream to tokenize the path
    std::wstringstream ss(directoryPath);
    std::wstring directory;
    std::wstring currentPath;

    // Iterate through each directory in the path
    while (std::getline(ss, directory, L'\\')) {
        // Append the current directory to the current path
        currentPath += directory + L"\\";

        // Check if the current path or directory is already existed 
        if (currentPath.find(directoryPath) != 0) {
            continue;
        }
        else {
            CreateDirectory(currentPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
        }
    }
    std::wcout << L"Directories created successfully." << currentPath << std::endl;

    return currentPath;
}

bool downloadFile(const std::wstring& url, const std::wstring& filename, const std::wstring& directory_path)
{

    std::wstring filePath = directory_path + filename;

    HINTERNET hInternet = InternetOpen(L"FileDownloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::wcerr << L"Failed to open internet connection." << std::endl;
        return false;
    }

    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        std::wcerr << L"Failed to open URL." << std::endl;
        InternetCloseHandle(hInternet);
        return false;
    }

    // Handling the HTTP error - 404
    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    if (!HttpQueryInfo(hUrl, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, NULL) || statusCode != 200) {
        WORD defaultAttributes = getDefaultConsoleAttributes();
        setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::wcerr << L"HTTP request for " + url + L" failed with status code : " << statusCode << std::endl;
        setConsoleTextColor(defaultAttributes);
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to create file." << std::endl;
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    DWORD bytesRead = 0;
    BYTE buffer[1024];
    BOOL bRead = FALSE;
    do {
        bRead = InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead);
        if (bytesRead > 0) {
            DWORD bytesWritten = 0;
            WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
        }
    } while (bRead && bytesRead > 0);

    CloseHandle(hFile);
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return true;
}

std::wstring getTmpPath() {
    wchar_t tempFolder[MAX_PATH];
    if (GetTempPath(MAX_PATH, tempFolder) == 0) {
        std::wcerr << L"Failed to get temporary folder path." << std::endl;
        return L"";
    }
    else {
        return std::wstring(tempFolder);
    }

}

std::wstring getfileWithExtension(std::string filepath) {

    fs::path p(filepath);
    std::wstring filename = p.filename();
    /*std::wcout << L"Filename with extension: " << filename << std::endl;*/
    return filename;
}

std::wstring getRandomGUIDString() {

    GUID guid;
    HRESULT hr = CoCreateGuid(&guid);

    if (FAILED(hr)) {
        std::cerr << "Failed to create GUID" << std::endl;
    }

    // Convert GUID to string
    wchar_t guidString[39]; // GUID format: {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
    int count = StringFromGUID2(guid, guidString, sizeof(guidString) / sizeof(guidString[0]));

    if (count == 0) {
        std::cerr << "Failed to convert GUID to string" << std::endl;
    }

    // Store the GUID string in a std::wstring
    std::wstring guidWString(guidString);

    if (guidWString.length() >= 2 && guidWString.front() == L'{' && guidWString.back() == L'}') {
        guidWString = guidWString.substr(1, guidWString.length() - 2);
    }

    // Print the GUID
    std::wcout << L"Generated GUID: " << guidWString << std::endl;
    return guidWString;

}

json httpGetInvoke(std::wstring& url) {

    HINTERNET hInternet = InternetOpen(L"HTTPGET", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        std::cerr << "Failed to initialize WinINet." << std::endl;
        return 1;
    }

    // Open URL
    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hUrl == NULL) {
        std::cerr << "Failed to open URL." << std::endl;
        InternetCloseHandle(hInternet);
        return 1;
    }

    // Read response
    std::vector<char> buffer(1024);
    DWORD bytesRead;
    std::string response;
    while (InternetReadFile(hUrl, &buffer[0], buffer.size(), &bytesRead) && bytesRead > 0) {
        response.append(&buffer[0], bytesRead);
    }

    json jsonResponse;
    jsonResponse = json::parse(response);

    // Cleanup
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return jsonResponse;

}

void setConsoleTextColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

WORD getDefaultConsoleAttributes() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    return consoleInfo.wAttributes;
}