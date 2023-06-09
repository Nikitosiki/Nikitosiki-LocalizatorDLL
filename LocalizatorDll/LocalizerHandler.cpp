#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "LocalizerHandler.h"
#include "DllFunctions.h"
#include "XMLReader.h"


LocalizerHandler::LocalizerHandler()
{
    // ������������� ������������ ��������
    dictionary = new std::unordered_map<std::string, std::string>();
    languages = new std::vector<language>();

    // �������� ���� ������� ����������
    const std::string directoryPath = GetDllFolderPath();

    // ��������� �������� ������ �� ����� ��������
    if (!XMLReader::ReadLanguagesFromFile(directoryPath + "\\language\\settings.xml", *this->languages))
        MessageError(NULL, "Error. Reading settings file!", "Localizer Error", MB_ICONERROR | MB_OK);

    // ��������� ��� ����� ������ ����������
    if (!CheckAllTranslationsExist(directoryPath + "\\language\\dictionaries\\", *this->languages))
        MessageError(NULL, "Error. Not all dictionary files exist!", "Localizer Error", MB_ICONERROR | MB_OK);

    // ��������� ���� �� ����� �������
    std::vector<std::string> keys;
    if (!XMLReader::ReadKeysFromFile(directoryPath + "\\language\\keys.xml", keys))
        MessageError(NULL, "Error. Reading keys file!", "Localizer Error", MB_ICONERROR | MB_OK);

    // ��������� ��� ��� ����� ����������
    std::string duplicateKey;
    if (isDuplicates(keys, duplicateKey))
        MessageError(NULL, ("Error. Key file contains a duplicate: " + duplicateKey).c_str(), "Localizer Error", MB_ICONERROR | MB_OK);

    // ��������� �������� ����� ���������� �����, � ����� ��������
    std::string fileLanguageName;
    if (!XMLReader::ReadSelectLangFile_FromFile(directoryPath + "\\language\\settings.xml", fileLanguageName))
        MessageError(NULL, "Error. Reading settings file!", "Localizer Error", MB_ICONERROR | MB_OK);

    // ��������� ������� � ������ ���������
    if (!XMLReader::ReadTranslationsFromFile(directoryPath + "\\language\\dictionaries\\" + fileLanguageName, *this->dictionary))
        MessageError(NULL, ("Error. Reading dictionary file: " + fileLanguageName).c_str(), "Localizer Error", MB_ICONERROR | MB_OK);

    // ��������� ������� �� ������������, ��������� ���� �� ����� �������
    std::string ErrorMess;
    if (!CheckCorrectDictionary(keys, *this->dictionary, ErrorMess))
        MessageError(NULL, ("Error. Incorrect keys in dictionary, file: " + fileLanguageName + "\n" + ErrorMess).c_str(), "Localizer Error", MB_ICONERROR | MB_OK);
}

LocalizerHandler::~LocalizerHandler()
{
    delete dictionary;
    delete languages;
}


// ����� ��� �������� �������� �� ����� �� unordered_map
const std::string& LocalizerHandler::GetValue(const std::string& key) const
{
    if (dictionary->empty())
        MessageError(NULL, "Error. Dictionary file is empty!", "Localizer Error", MB_ICONERROR | MB_OK);

    auto iter = dictionary->find(key);
    if (iter != dictionary->end())
    {
        return iter->second;
    }
    else
    {
        MessageError(NULL, ("Error. No key: \"" + key + "\", in dictionary!").c_str(), "Localizer Error", MB_ICONERROR | MB_OK);
    }
}

const void LocalizerHandler::SetLanguage(const std::string& nameLanguage) const
{
    if (languages->empty())
        MessageError(NULL, "Error. Languages file is empty!", "Localizer Error", MB_ICONERROR | MB_OK);

    if (!XMLReader::ModifySelectLangFile(GetDllFolderPath() + "\\language\\settings.xml", nameLanguage))
        MessageError(NULL, ("Error. Modify select language! Input Language: " + nameLanguage).c_str(), "Localizer Error", MB_ICONERROR | MB_OK);
}

const std::string LocalizerHandler::GetSelectLanguageName() const
{
    if (languages->empty())
        MessageError(NULL, "Error. Languages file is empty!", "Localizer Error", MB_ICONERROR | MB_OK);

    std::string nameSelectLang;
    if (!XMLReader::ReadSelectLangName_FromFile(GetDllFolderPath() + "\\language\\settings.xml", nameSelectLang))
        MessageError(NULL, "Error. Reading settings file!", "Localizer Error", MB_ICONERROR | MB_OK);

    return nameSelectLang;
}

const std::string LocalizerHandler::GetLanguageNames() const
{
    if (languages->empty())
        MessageError(NULL, "Error. Languages file is empty!", "Localizer Error", MB_ICONERROR | MB_OK);

    std::string names;
    for (const language& languageThis : *languages) {
        if (!names.empty()) {
            names += " ";
        }
        names += languageThis.name;
    }
    return names;
}

const char** LocalizerHandler::GetLanguageNames(int& size) const
{
    if (languages->empty())
        MessageError(NULL, "Error. Languages file is empty!", "Localizer Error", MB_ICONERROR | MB_OK);

    size = languages->size();
    const char** languageNames = new const char* [size];

    for (int i = 0; i < size; ++i) {
        languageNames[i] = (*languages)[i].name.c_str();
    }

    return languageNames;
}


// ----------------------------------
// ---------- Helper class ----------
// ----------------------------------

const void LocalizerHandler::MessageError(const HWND& hWnd, const LPCSTR& lpText, const LPCSTR& lpCaption, const UINT& uType) const
{
    MessageBoxA(hWnd, lpText, lpCaption, uType);
    Stop();
    ExitProcess(0);
}

const bool LocalizerHandler::isDuplicates(const std::vector<std::string>& values, std::string& duplicateElement) const
{
    std::vector<std::string> sortedValues = values;      // ������� ����� �������
    std::sort(sortedValues.begin(), sortedValues.end()); // ��������� �����

    // ���������, ���� �� �������� �������� � ��������������� �������, ������� �����
    for (std::size_t i = 1; i < sortedValues.size(); ++i)
    {
        if (sortedValues[i] == sortedValues[i - 1])
        {
            duplicateElement = sortedValues[i];
            return true; // ���� ���������
        }
    }

    return false; // ���������� ���
}

const std::string LocalizerHandler::GetDllFolderPath() const
{
    char path[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);

    if (hModule != NULL)
    {
        GetModuleFileNameA(hModule, path, sizeof(path));

        std::string fullPath(path);
        size_t found = fullPath.find_last_of("\\");
        return fullPath.substr(0, found);
    }

    MessageError(NULL, "Error. Failed to get project directory!", "Localizer Error", MB_ICONERROR | MB_OK);
}

const bool LocalizerHandler::CheckCorrectDictionary(const std::vector<std::string>& keys, const std::unordered_map<std::string, std::string>& dictionary, std::string& errorMess) const
{
    if (keys.size() != dictionary.size())
    {
        errorMess = "Incorrect number of keys!";
        return false;
    }

    for (const auto& key : keys)
    {
        if (dictionary.find(key) == dictionary.end())
        {
            errorMess = "Key not found: " + key;
            return false;
        }
    }

    return true;
}

const bool LocalizerHandler::CheckAllTranslationsExist(const std::string& folderPath, const std::vector<language>& languages) const
{
    for (const auto& language : languages)
    {
        std::ifstream file(folderPath + language.file);
        if (!file)
        {
            return false;
        }
    }

    return true;
}