#include<iostream>
#include"headers/util.h"
#include"headers/AASServerHandler.h"
#include "headers/AASModels.h"
#include "headers/RobotDescription.h"

void RobotDescription::ParseJsonToRobotDescription(const json& data)
{
    urdf_serverRelativepath = data[0]["value"].get<std::string>();
    resource_directory_name = data[1]["idShort"].get<std::string>();
    int Size = data[1]["value"].size();

    for (int i = 0; i < Size; ++i) {

        const auto resource = data[1]["value"][i];
        resources.push_back(resource);
        /*std::cout << "Element at index " << i << ": " << data[1]["value"][i]["idShort"] << std::endl;*/
    }
}

RobotDescription RobotDescription::RetrieveRobotModel(std::wstring& ServerUrl, std::wstring& AAS_ID, std::wstring& ModelName, bool WithResourceRetrieving)
{
    try {        
                json retrievedSE_json = AASServerHandler::RetrieveSubmodelElements(ServerUrl, AAS_ID, ModelName);
        
                if (!retrievedSE_json.is_null()) {
        
                    RobotDescription robot_description;
                    robot_description.WithResourceFiles = WithResourceRetrieving;
                    robot_description.ParseJsonToRobotDescription(retrievedSE_json);
                    std::wstring tempFolder = getTmpPath();
        
                    if (WithResourceRetrieving) {

                        RobotDescription rd = robot_description.robotModelWithResource(robot_description,ServerUrl,AAS_ID,tempFolder);
                        return rd;
                    }
                    else {
                        std::wstring filename = getfileWithExtension(robot_description.urdf_serverRelativepath);
                        std::wstring ConcatenatedModelPath = ServerUrl + L"/files/" + AAS_ID + stringToWstring(robot_description.urdf_serverRelativepath);
                        std::wstring new_directory_name = getRandomGUIDString();
                        std::wstring created_directory = createDirectory(tempFolder + new_directory_name + L"\\");
                        robot_description.rmodel_directory_root = created_directory;
            
                        if (!created_directory.empty()) {
        
                            //std::wstring filepathInTmp = std::wstring(tempFolder) + L"RobotDescription\\";
                            if (downloadFile(ConcatenatedModelPath, filename, created_directory)) {
        
                                std::wcout << filename + L"-> downloaded successfully to: " << created_directory << std::endl;
                            }
                            else {
                                std::wcerr << L"Failed to download: " + filename << std::endl;
                            }
                        }
                        else {
                            std::wcerr << L"Failed to create directory :" + new_directory_name + L" at :" + tempFolder << std::endl;
                        }
                    }
                    return robot_description;
                }
                else {
                    std::cout << "retrievedSE_json is null, check the retrievation process or server connection" << std::endl;
                }
        
    }
    catch (const std::exception& e) {
             std::cerr << "Failed to RetrieveRobotModel: " << e.what() << std::endl;
             
    }
	
}

RobotDescription RobotDescription::robotModelWithResource(RobotDescription robot_description, std::wstring& ServerUrl, std::wstring& AAS_ID, std::wstring tempFolder) {

    std::wstring filename = getfileWithExtension(robot_description.urdf_serverRelativepath);
    std::wstring ConcatenatedModelPath = ServerUrl + L"/files/" + AAS_ID + stringToWstring(robot_description.urdf_serverRelativepath);

    std::wstring new_directory_name = getRandomGUIDString();
    std::wstring created_directory = createDirectory(tempFolder + new_directory_name + L"\\");
    robot_description.rmodel_directory_root = created_directory;
    if (!created_directory.empty()) {

        //std::wstring filepathInTmp = std::wstring(tempFolder) + L"RobotDescription\\";
        if (downloadFile(ConcatenatedModelPath, filename, created_directory)) {

            std::wcout << filename + L"-> downloaded successfully to: " << created_directory << std::endl;
        }
        else {
            std::wcerr << L"Failed to download: " + filename << std::endl;
        }
        std::wstring resource_directory_name = stringToWstring(robot_description.resource_directory_name);
        std::wstring resource_directory = createDirectory(created_directory + resource_directory_name + L"\\");

        if (!resource_directory.empty()) {

            for (const auto& resource : robot_description.resources) {
                if (resource.has_value()) {
                    try {
                        const json& jsonObj = std::any_cast<json>(resource);
                        if (jsonObj.is_object()) {

                            SubmodelElement se = SubmodelElement::ParseJsonToSubmodelElement(jsonObj);

                            std::wstring res_name = stringToWstring(se.idShort);
                            std::wstring new_directory = createDirectory(resource_directory + res_name + L"\\");
                            if (!new_directory.empty()) {

                                const json& jsonObj = std::any_cast<json>(se.value);

                                if (jsonObj.is_array()) {

                                    for (const auto& resource : jsonObj) {

                                        const json& json_resource = std::any_cast<json>(resource);
                                        SubmodelElement individual_se = SubmodelElement::ParseJsonToSubmodelElement(json_resource);

                                        const std::string& file_value = std::any_cast<json>(individual_se.value);

                                        std::wstring filename = getfileWithExtension(file_value);
                                        std::wstring ConcatenatedModelPath = ServerUrl + L"/files/" + AAS_ID + stringToWstring(file_value);
                                        //std::wstring filepathInTmp = std::wstring(tempFolder) + ModelName + L"\\" + resource_directory + L"\\" + res_name + L"\\";

                                        if (downloadFile(ConcatenatedModelPath, filename, new_directory)) {
                                            std::wcout << filename + L"-> downloaded successfully to: " << new_directory << std::endl;

                                        }
                                        else {
                                            std::wcerr << L"Failed to download: " + filename << std::endl;
                                        }

                                    }
                                }
                                else {
                                    std::cout << "The object is of type:" << jsonObj.type_name() << "-> There is no implementation of such type,implement it at RetrieveRobotModel() method" << std::endl;
                                }
                            }
                        }
                        else {
                            std::cerr << "Error: Object in resources is not a JSON object." << std::endl;
                        }
                    }
                    catch (const std::bad_any_cast& e) {
                        std::cerr << "Error: Unable to cast JSON object from std::any: " << e.what() << std::endl;
                    }
                }
            }


        }
        else {
            std::wcerr << L"Failed to create directory " + resource_directory_name + L" at :" + created_directory << std::endl;
        }

        return robot_description;
    }
    else {
        std::wcerr << L"Failed to create directory :" + new_directory_name + L" at :" + tempFolder << std::endl;
    }

    // directory creation for robot's model resource
    // convert resource folder name from string to wstring 
    
}

std::wstring RobotDescription::getRobotModelDirectoryRoot() {

    return rmodel_directory_root;

}

bool RobotDescription::cleanUp() {
    try {
        if (fs::exists(rmodel_directory_root)) {
            fs::remove_all(rmodel_directory_root);
            std::wcout << "Deleted temporary directory " << rmodel_directory_root << std::endl;
            return 1;
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 0;
    }
}
