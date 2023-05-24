#include "pch.h"
#include "XMLReader.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include "pugixml.hpp"

using namespace pugi;


// ����� ��� ���������� ����� XML � ���������� ��� � unordered_map
bool XMLReader::LoadFile(const std::string& filePath)
{
    xml_document doc;
    if (!doc.load_file(filePath.c_str()))
    {
        return false;
    }

    // ������� ������� unordered_map ����� ��������� ����� ������
    translations.clear();

    xml_node root = doc.child("translations");
    if (!root)
    {
        return false;
    }

    for (xml_node entry = root.child("entry"); entry; entry = entry.next_sibling("entry"))
    {
        std::string key = entry.attribute("key").as_string();
        std::string value = entry.attribute("value").as_string();
        translations[key] = value;
    }

    return true;
}

// ����� ��� �������� �������� �� ����� �� unordered_map
const std::string& XMLReader::GetValue(const std::string& key) const
{
    auto iter = translations.find(key);
    if (iter != translations.end())
    {
        return iter->second;
    }
    else
    {
        return 0;
    }
}


