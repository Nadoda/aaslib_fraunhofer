#pragma once
#include<nlohmann/json.hpp>
namespace fs = std::filesystem;
using json = nlohmann::json;



class RobotDescription {
private:
    std::string urdf_serverRelativepath;
    std::string resource_directory_name;
    std::list<std::any> resources;
    std::wstring rmodel_directory_root;

public:
   /* ~RobotDescription() {
        try {
            if (fs::exists(rmodel_directory_root)) {
                fs::remove_all(rmodel_directory_root);
                std::wcout << "Deleted temporary directory " << rmodel_directory_root << std::endl;
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }
    }*/

    bool WithResourceFiles = true;
    std::wstring getRobotModelDirectoryRoot();
    void ParseJsonToRobotDescription(const json& data);
    static RobotDescription RetrieveRobotModel(std::wstring& ServerUrl, std::wstring& AAS_ID, std::wstring& ModelName, bool WithResourceRetrieving);
    bool cleanUp();

private:
    RobotDescription robotModelWithResource(RobotDescription robot_description, std::wstring& ServerUrl, std::wstring& AAS_ID, std::wstring tempFolder);

};

