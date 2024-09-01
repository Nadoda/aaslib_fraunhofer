#pragma once
#include<iostream>
#include<nlohmann/json.hpp>
#include "AASModels.h"

using json = nlohmann::json;

class AASHttpClient {
public:
	static json GetAllSubmodelElements(std::wstring& serverUrl, std::wstring& AAS_ID, std::wstring& submodel_IDShort);

	static std::list<AssetAdministrationShell> GetAssetAdministrationShells(std::wstring& serverUrl);
};
