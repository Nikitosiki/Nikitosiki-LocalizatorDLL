#pragma once
#include <string>
#include <unordered_map>

class XMLReader
{
private:
    std::unordered_map<std::string, std::string> translations;

public:
    // ����� ��� ���������� ����� XML � ���������� ��� � unordered_map
    bool LoadFile(const std::string&);

    // ����� ��� �������� �������� �� ����� �� unordered_map
    const std::string& GetValue(const std::string&) const;
};