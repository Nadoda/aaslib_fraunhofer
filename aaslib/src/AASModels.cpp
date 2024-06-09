#include "headers/AASModels.h"

SubmodelElement SubmodelElement::ParseJsonToSubmodelElement(const json& jsonObject) {
    SubmodelElement submodelElement;

    submodelElement.idShort = jsonObject["idShort"];
    submodelElement.modelType.name = jsonObject["modelType"]["name"];
    submodelElement.value = jsonObject["value"];
    /*submodelElement.valueType = jsonObject["mimeType"];*/
    return submodelElement;
}

void Submodel::ParseJsonToSubmodel(const json& data) {

    idShort = data["idShort"].get<std::string>();
   /* identification.id = data["identification"]["id"].get<std::string>();
    identification.idType = data["identification"]["idType"].get<std::string>();*/
}

void AssetAdministrationShell::ParseJsonToAAS(const json& data)
{
    idShort = data["idShort"].get<std::string>();
    identification.id = data["identification"]["id"].get<std::string>();
    identification.idType = data["identification"]["idType"].get<std::string>();
    if (data["submodels"].is_array()) {

        int Size = data["submodels"].size();
        for (int i = 0; i < Size; ++i) {

            const auto submodel_json = data["submodels"][i];
            Submodel sm;
            sm.ParseJsonToSubmodel(submodel_json);
            submodels.push_back(sm);
        }
    }
}