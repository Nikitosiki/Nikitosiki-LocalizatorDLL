#include <iostream>
#include <string>
#include <unordered_map>
#include "pch.h"
#include "XMLReader.h"
#include "pugixml.hpp"
#include "language.h"

using namespace pugi;


const bool XMLReader::ReadLanguagesFromFile(const std::string& fileFullPath, std::vector<language>& languages)
{
    xml_document doc;
    if (!doc.load_file(fileFullPath.c_str()))
    {
        return false;
    }

    xml_node availableLanguagesNode = doc.select_node("/settings/available_languages").node();
    if (availableLanguagesNode.empty()) {
        return false;
    }

    // ������� ������� unordered_map ����� ��������� ����� ������
    languages.clear();


    for (xml_node languageNode : availableLanguagesNode.children("language")) {
        language newlanguage;
        newlanguage.code = languageNode.attribute("code").as_string();
        newlanguage.name = languageNode.attribute("name").as_string();
        newlanguage.file = languageNode.attribute("file").as_string();
        languages.push_back(newlanguage);
    }

    return true;
}


const bool XMLReader::ReadFromFile(const std::string& fileFullPath, const std::string& selectNode, const std::string& selectChildren, const std::string& selectAttribute, std::string& value)
{
    xml_document doc;
    if (!doc.load_file(fileFullPath.c_str()))
    {
        return false;
    }

    xml_node node = doc.select_node(selectNode.c_str()).node();
    if (node.empty()) {
        return false;
    }

    xml_node entry = node.child(selectChildren.c_str());
    if (entry.empty()) {
        return false;
    }

    value = entry.attribute(selectAttribute.c_str()).as_string();
    if (value.empty()) {
        return false;
    }

    return true;
}

//// ����� ��� ���������� ����� XML � ���������� ��� � unordered_map
//const std::string XMLReader::LoadFile(const std::string& filePath)
//{
//    xml_document doc;
//    if (!doc.load_file(filePath.c_str()))
//    {
//        return false;
//    }
//
//    // ������� ������� unordered_map ����� ��������� ����� ������
//    translations.clear();
//
//    xml_node root = doc.child("translations");
//    if (!root)
//    {
//        return false;
//    }
//
//    for (xml_node entry = root.child("entry"); entry; entry = entry.next_sibling("entry"))
//    {
//        std::string key = entry.attribute("key").as_string();
//        std::string value = entry.attribute("value").as_string();
//        translations[key] = value;
//    }
//
//    return true;
//}