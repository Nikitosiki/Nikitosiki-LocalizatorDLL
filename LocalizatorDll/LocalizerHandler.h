#pragma once
#include <string>
#include <unordered_map>
#include "language.h"

class LocalizerHandler
{
private:
	std::unordered_map<std::string, std::string> dictionary;
	std::vector<language> languages;

	const std::string GetDllFolderPath() const;


public:
	LocalizerHandler();

	// ����� ��� �������� �������� �� ����� �� unordered_map
	const std::string& GetValue(const std::string&) const;

	const std::string GetLanguageNames() const;

	const char** GetLanguageNames(int&) const;
};

