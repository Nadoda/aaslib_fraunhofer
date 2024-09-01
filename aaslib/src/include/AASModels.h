#pragma once
#include<iostream>
#include<any>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

class Identification {
public:
    std::string  id;
    std::string  idType;
};

class Description {
public:
    std::string language;
    std::string  text;
};

class Endpoint {
public:
    std::string address;
    std::string type;
};

class ModelType {
public:
    std::string  name;
};

class Asset {
public:
    std::string idShort;
    std::string kind;
    ModelType modelType;
    Identification identification;
    std::list<Description> description;
};

class Key {
public:
    std::string type;
    std::string idType;
    std::string value;
    bool local;
};


class SemanticId {
public:
    std::list<Key> keys;
};

class SubmodelElement {
public:
    std::string  idShort;
    ModelType  modelType;
    std::any value;
    std::string valueType;
    std::any description;

    static SubmodelElement ParseJsonToSubmodelElement(const json& jsonObject);
};

class Submodel {
public:
    std::string idShort;
    /*Identification identification;
    std::list<Description> description;
    SemanticId semanticId;
    std::list<Endpoint> endpoints;
    ModelType modelType;
    std::list<SubmodelElement> submodelElements;*/

    void ParseJsonToSubmodel(const json& data);
};


class AssetAdministrationShell{
    public:
     std::string  idShort;
     Identification identification;
    /* std::list<Description> description;
     std::list<Endpoint> endpoints;
     ModelType modelType;
     Asset asset;*/
     std::list<Submodel> submodels;
     void ParseJsonToAAS(const json& data);
};

















