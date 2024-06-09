#pragma once
#include<iostream>
#include<nlohmann/json.hpp>
#include "AASModels.h"

using json = nlohmann::json;

class AASServerHandler {
public:
	static json RetrieveSubmodelElements(std::wstring& serverUrl, std::wstring& AAS_ID, std::wstring& submodel_IDShort);

	static std::list<AssetAdministrationShell> getAAS(std::wstring& serverUrl);
};
