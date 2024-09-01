#pragma once
#include<nlohmann/json.hpp>
namespace fs = std::filesystem;
using json = nlohmann::json;



class RobotDescription {
private:
    std::string urdf_serverRelativepath;
    std::string resource_directory_name;
    std::list<std::any> resources;
    std::wstring rmodel_directory_path;

public:

    ~RobotDescription() {
         // CleanUp(); // Commented because of the testing purposes 
    }
    

    bool WithResourceFiles = true;
    std::wstring GetRobotModelDirectoryPath();
    void ParseJsonToRobotDescription(const json& data);
    static RobotDescription RetrieveRobotModel(std::wstring& ServerUrl, std::wstring& AAS_ID, std::wstring& ModelName, bool WithResourceRetrieving);
    bool CleanUp();

private:
    RobotDescription RobotModelWithResource(RobotDescription robot_description, std::wstring& ServerUrl, std::wstring& AAS_ID, std::wstring tempFolder);

};

