#include <iostream>
#include <windows.h>
#include <wininet.h>
#include "headers/AASServerHandler.h"
#include "headers/util.h"
#pragma comment(lib, "wininet.lib")

json AASServerHandler::RetrieveSubmodelElements(std::wstring& serverUrl, std::wstring& AAS_ID, std::wstring& submodel_IDShort)
{
    std::wstring ConcatenatedSubModelPath = serverUrl + L"/shells/" + AAS_ID + L"/aas/submodels/" + submodel_IDShort + L"/submodel/submodelElements";

    try {
        return httpGetInvoke(ConcatenatedSubModelPath);
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
        return 1;
    }
   
}

std::list<AssetAdministrationShell> AASServerHandler::getAAS(std::wstring& serverUrl)
{
    try {
        std::wstring ConcatenatedShellPath = serverUrl + L"/shells";

        json aasInjson = httpGetInvoke(ConcatenatedShellPath);

        std::list<AssetAdministrationShell> aaslist;
        if (aasInjson.is_array()) {

            int Size = aasInjson.size();
            for (int i = 0; i < Size; ++i) {

                const auto aas_json = aasInjson[i];
                AssetAdministrationShell aas;
                aas.ParseJsonToAAS(aas_json);
                aaslist.push_back(aas);
            }
        }
        else {
            AssetAdministrationShell aas;
            aas.ParseJsonToAAS(aasInjson);
            aaslist.push_back(aas);
        }
        return aaslist;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load AAS: " << e.what() << std::endl;
    }
    

    
}
