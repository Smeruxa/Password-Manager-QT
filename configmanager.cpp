#include "ConfigManager.h"

// Реализация конструктора
ConfigManager::ConfigManager() {
    qDebug() << "Number of loaded structures: " << fillArray();
}

int ConfigManager::fillArray() {
    structArray.clear();

    // Загрузка структур из реестра
    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, createRegistryPath("").toStdWString().c_str(), 0, KEY_READ, &hKey);

    if (result == ERROR_SUCCESS) {
        DWORD subKeyCount;
        DWORD maxSubKeyLen;
        result = RegQueryInfoKey(hKey, nullptr, nullptr, nullptr, &subKeyCount, &maxSubKeyLen, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

        if (result == ERROR_SUCCESS) {
            for (DWORD i = 0; i < subKeyCount; ++i) {
                DWORD subKeyLen = maxSubKeyLen + 1;
                wchar_t* subKeyName = new wchar_t[subKeyLen];

                result = RegEnumKeyEx(hKey, i, subKeyName, &subKeyLen, nullptr, nullptr, nullptr, nullptr);

                if (result == ERROR_SUCCESS) {
                    HKEY subKey;
                    result = RegOpenKeyEx(hKey, subKeyName, 0, KEY_READ, &subKey);

                    if (result == ERROR_SUCCESS) {
                        // Прочитать тип структуры
                        DWORD type;
                        DWORD typeSize = sizeof(type);
                        result = RegQueryValueEx(subKey, L"type", nullptr, nullptr, reinterpret_cast<LPBYTE>(&type), &typeSize);

                        if (result == ERROR_SUCCESS) {
                            STRUCTURES::BaseStructure* loadedStruct = nullptr;

                            switch (type) {
                            case STRUCTURES::PASSWORD_TYPE:
                                loadedStruct = new STRUCTURES::PASSWORD();
                                break;
                            case STRUCTURES::NOTE_TYPE:
                                loadedStruct = new STRUCTURES::NOTE();
                                break;
                            case STRUCTURES::DATA_TYPE:
                                loadedStruct = new STRUCTURES::DATA();
                                break;
                            default:
                                qWarning() << "Invalid structure type: " << type;
                                break;
                            }

                            if (loadedStruct) {
                                // Пример заглушки для чтения общих полей структуры
                                wchar_t nameBuffer[MAX_PATH];
                                DWORD nameBufferSize = sizeof(nameBuffer);
                                DWORD nameType;

                                LONG result = RegQueryValueEx(subKey, L"name", 0, &nameType, reinterpret_cast<LPBYTE>(nameBuffer), &nameBufferSize);
                                if (result == ERROR_SUCCESS && nameType == REG_SZ) {
                                    loadedStruct->name = QString::fromStdWString(nameBuffer);
                                } else {
                                    qWarning() << "Failed to read 'name' from registry: " << result;
                                }

                                wchar_t additionalInfoBuffer[MAX_PATH];
                                DWORD additionalInfoBufferSize = sizeof(additionalInfoBuffer);
                                DWORD additionalInfoType;

                                result = RegQueryValueEx(subKey, L"additionalInfo", 0, &additionalInfoType, reinterpret_cast<LPBYTE>(additionalInfoBuffer), &additionalInfoBufferSize);
                                if (result == ERROR_SUCCESS && additionalInfoType == REG_SZ) {
                                    loadedStruct->additionalInfo = QString::fromStdWString(additionalInfoBuffer);
                                } else {
                                    qWarning() << "Failed to read 'additionalInfo' from registry: " << result;
                                }

                                // Ваш код для чтения дополнительных полей структуры
                                if (loadedStruct->getType() == STRUCTURES::PASSWORD_TYPE) {
                                    STRUCTURES::PASSWORD* passwordStruct = dynamic_cast<STRUCTURES::PASSWORD*>(loadedStruct);
                                    if (passwordStruct) {
                                        wchar_t siteBuffer[MAX_PATH];
                                        DWORD siteBufferSize = sizeof(siteBuffer);
                                        DWORD siteType;

                                        result = RegQueryValueEx(subKey, L"site", 0, &siteType, reinterpret_cast<LPBYTE>(siteBuffer), &siteBufferSize);
                                        if (result == ERROR_SUCCESS && siteType == REG_SZ) {
                                            passwordStruct->site = QString::fromStdWString(siteBuffer);
                                        } else {
                                            qWarning() << "Failed to read 'site' from registry: " << result;
                                        }

                                        wchar_t loginBuffer[MAX_PATH];
                                        DWORD loginBufferSize = sizeof(loginBuffer);
                                        DWORD loginType;

                                        result = RegQueryValueEx(subKey, L"login", 0, &loginType, reinterpret_cast<LPBYTE>(loginBuffer), &loginBufferSize);
                                        if (result == ERROR_SUCCESS && loginType == REG_SZ) {
                                            passwordStruct->login = QString::fromStdWString(loginBuffer);
                                        } else {
                                            qWarning() << "Failed to read 'login' from registry: " << result;
                                        }

                                        wchar_t passwordBuffer[MAX_PATH];
                                        DWORD passwordBufferSize = sizeof(passwordBuffer);
                                        DWORD passwordType;

                                        result = RegQueryValueEx(subKey, L"password", 0, &passwordType, reinterpret_cast<LPBYTE>(passwordBuffer), &passwordBufferSize);
                                        if (result == ERROR_SUCCESS && passwordType == REG_SZ) {
                                            passwordStruct->password = QString::fromStdWString(passwordBuffer);
                                        } else {
                                            qWarning() << "Failed to read 'password' from registry: " << result;
                                        }

                                        // Ваш код для чтения дополнительных полей структуры PASSWORD
                                        DWORD data;
                                        DWORD dataSize = sizeof(data);
                                        DWORD dataType;

                                        result = RegQueryValueEx(subKey, L"data", 0, &dataType, reinterpret_cast<LPBYTE>(&data), &dataSize);
                                        if (result == ERROR_SUCCESS && dataType == REG_DWORD) {
                                            passwordStruct->data = static_cast<int>(data);
                                        } else {
                                            qWarning() << "Failed to read 'data' from registry: " << result;
                                        }
                                    }
                                } else if (loadedStruct->getType() == STRUCTURES::NOTE_TYPE) {
                                    STRUCTURES::NOTE* noteStruct = dynamic_cast<STRUCTURES::NOTE*>(loadedStruct);
                                    if (noteStruct) {
                                        wchar_t noteBuffer[MAX_PATH];
                                        DWORD noteBufferSize = sizeof(noteBuffer);
                                        DWORD noteType;

                                        result = RegQueryValueEx(subKey, L"note", 0, &noteType, reinterpret_cast<LPBYTE>(noteBuffer), &noteBufferSize);
                                        if (result == ERROR_SUCCESS && noteType == REG_SZ) {
                                            noteStruct->note = QString::fromStdWString(noteBuffer);
                                        } else {
                                            qWarning() << "Failed to read 'note' from registry: " << result;
                                        }

                                        // Ваш код для чтения дополнительных полей структуры NOTE
                                        DWORD data;
                                        DWORD dataSize = sizeof(data);
                                        DWORD dataType;

                                        result = RegQueryValueEx(subKey, L"data", 0, &dataType, reinterpret_cast<LPBYTE>(&data), &dataSize);
                                        if (result == ERROR_SUCCESS && dataType == REG_DWORD) {
                                            noteStruct->data = static_cast<int>(data);
                                        } else {
                                            qWarning() << "Failed to read 'data' from registry: " << result;
                                        }
                                    }
                                } else if (loadedStruct->getType() == STRUCTURES::DATA_TYPE) {
                                    STRUCTURES::DATA* dataStruct = dynamic_cast<STRUCTURES::DATA*>(loadedStruct);
                                    if (dataStruct) {
                                        // Ваш код для чтения дополнительных полей структуры DATA
                                        DWORD dataActivity;
                                        DWORD dataActivitySize = sizeof(dataActivity);
                                        DWORD dataActivityType;

                                        result = RegQueryValueEx(subKey, L"dataActivity", 0, &dataActivityType, reinterpret_cast<LPBYTE>(&dataActivity), &dataActivitySize);
                                        if (result == ERROR_SUCCESS && dataActivityType == REG_DWORD) {
                                            dataStruct->dataActivity = static_cast<int>(dataActivity);
                                        } else {
                                            qWarning() << "Failed to read 'dataActivity' from registry: " << result;
                                        }

                                        DWORD data;
                                        DWORD dataSize = sizeof(data);
                                        DWORD dataType;

                                        result = RegQueryValueEx(subKey, L"data", 0, &dataType, reinterpret_cast<LPBYTE>(&data), &dataSize);
                                        if (result == ERROR_SUCCESS && dataType == REG_DWORD) {
                                            dataStruct->data = static_cast<int>(data);
                                        } else {
                                            qWarning() << "Failed to read 'data' from registry: " << result;
                                        }
                                    }
                                }
                                // Добавляем структуру в массив, если она успешно загружена
                                if (loadedStruct)
                                    structArray.append(loadedStruct);
                            } else {
                                qWarning() << "Failed to read 'type' from registry: " << result;
                            }
                        }

                        RegCloseKey(subKey);
                    } else {
                        qWarning() << "Failed to open registry subkey: " << result;
                    }
                } else {
                    qWarning() << "Failed to enumerate registry subkeys: " << result;
                }

                delete[] subKeyName;
            }
        } else {
            qWarning() << "Failed to query registry key info: " << result;
        }

        RegCloseKey(hKey);
    } else {
        qWarning() << "Failed to open registry key: " << result;
    }

    return structArray.size();
}

// Реализация метода addStruct
template<typename T>
void ConfigManager::addStruct(const T &myStruct) {
    // Преобразование указателя на объект в указатель на базовую структуру
    STRUCTURES::BaseStructure* baseStruct = new T(myStruct);
    structArray.append(baseStruct);

    QString registryPath = createRegistryPath(QString::number(structArray.size() - 1));

    HKEY hKey;
    LONG result = RegCreateKeyEx(HKEY_CURRENT_USER, registryPath.toStdWString().c_str(), 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);

    if (result == ERROR_SUCCESS) {
        // Запись типа структуры в реестр
        DWORD type = static_cast<DWORD>(baseStruct->getType());
        result = RegSetValueEx(hKey, L"type", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&type), sizeof(type));

        if (result == ERROR_SUCCESS) {
            //baseStruct
            const std::wstring name = baseStruct->name.toStdWString();
            const wchar_t* namePtr = name.c_str();
            LONG result = RegSetValueEx(hKey, L"name", 0, REG_SZ, reinterpret_cast<const BYTE*>(namePtr), static_cast<DWORD>(name.size() * sizeof(wchar_t)));
            if (result != ERROR_SUCCESS) {
                qWarning() << "Failed to write 'name' to registry: " << result;
            }

            const std::wstring additionalInfo = baseStruct->additionalInfo.toStdWString();
            const wchar_t* additionalInfoPtr = additionalInfo.c_str();
            result = RegSetValueEx(hKey, L"additionalInfo", 0, REG_SZ, reinterpret_cast<const BYTE*>(additionalInfoPtr), static_cast<DWORD>(additionalInfo.size() * sizeof(wchar_t)));
            if (result != ERROR_SUCCESS) {
                qWarning() << "Failed to write 'additionalInfo' to registry: " << result;
            }

            // Ваш код для записи дополнительных полей структуры
            if (baseStruct->getType() == STRUCTURES::PASSWORD_TYPE) {
                const STRUCTURES::PASSWORD* passwordStruct = dynamic_cast<const STRUCTURES::PASSWORD*>(baseStruct);
                if (passwordStruct) {
                    const std::wstring site = passwordStruct->site.toStdWString();
                    const wchar_t* sitePtr = site.c_str();
                    result = RegSetValueEx(hKey, L"site", 0, REG_SZ, reinterpret_cast<const BYTE*>(sitePtr), static_cast<DWORD>(site.size() * sizeof(wchar_t)));
                    if (result != ERROR_SUCCESS) {
                        qWarning() << "Failed to write 'site' to registry: " << result;
                    }

                    const std::wstring login = passwordStruct->login.toStdWString();
                    const wchar_t* loginPtr = login.c_str();
                    result = RegSetValueEx(hKey, L"login", 0, REG_SZ, reinterpret_cast<const BYTE*>(loginPtr), static_cast<DWORD>(login.size() * sizeof(wchar_t)));
                    if (result != ERROR_SUCCESS) {
                        qWarning() << "Failed to write 'login' to registry: " << result;
                    }

                    const std::wstring password = passwordStruct->password.toStdWString();
                    const wchar_t* passwordPtr = password.c_str();
                    result = RegSetValueEx(hKey, L"password", 0, REG_SZ, reinterpret_cast<const BYTE*>(passwordPtr), static_cast<DWORD>(password.size() * sizeof(wchar_t)));
                    if (result != ERROR_SUCCESS) {
                        qWarning() << "Failed to write 'password' to registry: " << result;
                    }

                    // Ваш код для записи дополнительных полей структуры PASSWORD
                    /*DWORD data = static_cast<DWORD>(passwordStruct->data);
                    result = RegSetValueEx(hKey, L"data", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));
                    if (result != ERROR_SUCCESS) {
                        qWarning() << "Failed to write 'data' to registry: " << result;
                    }*/
                    DWORD data = static_cast<DWORD>(passwordStruct->data);
                    result = RegSetValueEx(hKey, L"data", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));
                }
            } else if (baseStruct->getType() == STRUCTURES::NOTE_TYPE) {
                const STRUCTURES::NOTE* noteStruct = dynamic_cast<const STRUCTURES::NOTE*>(baseStruct);
                if (noteStruct) {
                    const std::wstring note = noteStruct->note.toStdWString();
                    const wchar_t* notePtr = note.c_str();
                    result = RegSetValueEx(hKey, L"note", 0, REG_SZ, reinterpret_cast<const BYTE*>(notePtr), static_cast<DWORD>(note.size() * sizeof(wchar_t)));
                    if (result != ERROR_SUCCESS) {
                        qWarning() << "Failed to write 'note' to registry: " << result;
                    }

                    // Ваш код для записи дополнительных полей структуры NOTE
                    DWORD data = static_cast<DWORD>(noteStruct->data);
                    result = RegSetValueEx(hKey, L"data", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));
                }
            } else if (baseStruct->getType() == STRUCTURES::DATA_TYPE) {
                const STRUCTURES::DATA* dataStruct = dynamic_cast<const STRUCTURES::DATA*>(baseStruct);
                if (dataStruct) {
                    // Ваш код для записи дополнительных полей структуры DATA
                    DWORD dataActivity = static_cast<DWORD>(dataStruct->dataActivity);
                    result = RegSetValueEx(hKey, L"dataActivity", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dataActivity), sizeof(dataActivity));
                    if (result != ERROR_SUCCESS) {
                        qWarning() << "Failed to write 'dataActivity' to registry: " << result;
                    }

                    DWORD data = static_cast<DWORD>(dataStruct->data);
                    result = RegSetValueEx(hKey, L"data", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));
                }
            }
        } else {
            qWarning() << "Failed to write 'type' to registry: " << result;
        }

        RegCloseKey(hKey);
    } else {
        qWarning() << "Failed to create registry key: " << result;
    }
}

template<typename T>
void ConfigManager::setStructure(int index, const T &myStruct) {
    if (index >= 0 && index < structArray.size()) {
        qDebug() << index;
        QString registryPath = createRegistryPath(QString::number(index));

        HKEY hKey;
        LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.toStdWString().c_str(), 0, KEY_SET_VALUE, &hKey);

        if (result == ERROR_SUCCESS) {
            const STRUCTURES::BaseStructure& baseStruct = myStruct;

            // Запись общих полей в реестр
            DWORD dataSize = static_cast<DWORD>((baseStruct.name.length() + 1) * sizeof(wchar_t));
            RegSetValueEx(hKey, L"name", 0, REG_SZ, reinterpret_cast<const BYTE*>(baseStruct.name.toStdWString().c_str()), dataSize);

            dataSize = static_cast<DWORD>((baseStruct.additionalInfo.length() + 1) * sizeof(wchar_t));
            RegSetValueEx(hKey, L"additionalInfo", 0, REG_SZ, reinterpret_cast<const BYTE*>(baseStruct.additionalInfo.toStdWString().c_str()), dataSize);

            // Запись полей, специфичных для каждой структуры
            if constexpr (std::is_same_v<T, STRUCTURES::PASSWORD>) {
                const STRUCTURES::PASSWORD& passwordStruct = dynamic_cast<const STRUCTURES::PASSWORD&>(myStruct);

                dataSize = static_cast<DWORD>((passwordStruct.site.length() + 1) * sizeof(wchar_t));
                RegSetValueEx(hKey, L"site", 0, REG_SZ, reinterpret_cast<const BYTE*>(passwordStruct.site.toStdWString().c_str()), dataSize);

                dataSize = static_cast<DWORD>((passwordStruct.login.length() + 1) * sizeof(wchar_t));
                RegSetValueEx(hKey, L"login", 0, REG_SZ, reinterpret_cast<const BYTE*>(passwordStruct.login.toStdWString().c_str()), dataSize);

                dataSize = static_cast<DWORD>((passwordStruct.password.length() + 1) * sizeof(wchar_t));
                RegSetValueEx(hKey, L"password", 0, REG_SZ, reinterpret_cast<const BYTE*>(passwordStruct.password.toStdWString().c_str()), dataSize);
            } else if constexpr (std::is_same_v<T, STRUCTURES::NOTE>) {
                const STRUCTURES::NOTE& noteStruct = dynamic_cast<const STRUCTURES::NOTE&>(myStruct);

                dataSize = static_cast<DWORD>((noteStruct.note.length() + 1) * sizeof(wchar_t));
                RegSetValueEx(hKey, L"note", 0, REG_SZ, reinterpret_cast<const BYTE*>(noteStruct.note.toStdWString().c_str()), dataSize);
            } else if constexpr (std::is_same_v<T, STRUCTURES::DATA>) {
                const STRUCTURES::DATA& dataStruct = dynamic_cast<const STRUCTURES::DATA&>(myStruct);

                // Преобразование int в wchar_t и запись в реестр
                std::wstring dataActivityStr = std::to_wstring(dataStruct.dataActivity);
                dataSize = static_cast<DWORD>((dataActivityStr.length() + 1) * sizeof(wchar_t));
                RegSetValueEx(hKey, L"dataActivity", 0, REG_SZ, reinterpret_cast<const BYTE*>(dataActivityStr.c_str()), dataSize);
            }

            RegCloseKey(hKey);
        } else {
            qWarning() << "Failed to open registry key: " << result;
        }
    } else {
        qWarning() << "Invalid index";
    }
}

// Реализация метода getStruct
template <typename T>
T ConfigManager::getStruct(int index) const {
    if (index >= 0 && index < structArray.size()) {
        STRUCTURES::BaseStructure* baseStruct = structArray.at(index);

        if constexpr (std::is_same_v<T, STRUCTURES::PASSWORD>) {
            if (baseStruct->getType() == STRUCTURES::PASSWORD_TYPE) {
                STRUCTURES::PASSWORD passwordStruct;

                // Определяем subKey по индексу из реестра
                HKEY subKey;
                QString subKeyName = createRegistryPath(QString::number(index));

                if (RegOpenKeyEx(HKEY_CURRENT_USER, subKeyName.toStdWString().c_str(), 0, KEY_READ, &subKey) == ERROR_SUCCESS) {
                    // Чтение значений из реестра и заполнение структуры PASSWORD

                    DWORD nameSize = MAX_PATH;
                    wchar_t name[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"name", 0, NULL, reinterpret_cast<LPBYTE>(name), &nameSize) == ERROR_SUCCESS) {
                        passwordStruct.name = QString::fromWCharArray(name);
                    }

                    DWORD additionalInfoSize = MAX_PATH;
                    wchar_t additionalInfo[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"additionalInfo", 0, NULL, reinterpret_cast<LPBYTE>(additionalInfo), &additionalInfoSize) == ERROR_SUCCESS) {
                        passwordStruct.additionalInfo = QString::fromWCharArray(additionalInfo);
                    }

                    DWORD siteSize = MAX_PATH;
                    wchar_t site[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"site", 0, NULL, reinterpret_cast<LPBYTE>(site), &siteSize) == ERROR_SUCCESS) {
                        passwordStruct.site = QString::fromWCharArray(site);
                    }

                    DWORD loginSize = MAX_PATH;
                    wchar_t login[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"login", 0, NULL, reinterpret_cast<LPBYTE>(login), &loginSize) == ERROR_SUCCESS) {
                        passwordStruct.login = QString::fromWCharArray(login);
                    }

                    DWORD passwordSize = MAX_PATH;
                    wchar_t password[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"password", 0, NULL, reinterpret_cast<LPBYTE>(password), &passwordSize) == ERROR_SUCCESS) {
                        passwordStruct.password = QString::fromWCharArray(password);
                    }

                    /*DWORD dataSize = sizeof(DWORD);
                    DWORD data;
                    if (RegQueryValueEx(subKey, L"data", 0, NULL, reinterpret_cast<LPBYTE>(&data), &dataSize) == ERROR_SUCCESS) {
                        passwordStruct.data = static_cast<int>(data);
                    }*/

                    DWORD dataSize = sizeof(DWORD);
                    DWORD data;
                    if (RegQueryValueEx(subKey, L"data", 0, NULL, reinterpret_cast<LPBYTE>(&data), &dataSize) == ERROR_SUCCESS) {
                        passwordStruct.data = static_cast<int>(data);
                    }

                    RegCloseKey(subKey);
                } else {
                    qWarning() << "Failed to open registry key for index: " << index;
                }

                return passwordStruct;
            }
        } else if constexpr (std::is_same_v<T, STRUCTURES::NOTE>) {
            if (baseStruct->getType() == STRUCTURES::NOTE_TYPE) {
                STRUCTURES::NOTE noteStruct;

                // Аналогично для структуры NOTE
                HKEY subKey;
                QString subKeyName = createRegistryPath(QString::number(index));

                if (RegOpenKeyEx(HKEY_CURRENT_USER, subKeyName.toStdWString().c_str(), 0, KEY_READ, &subKey) == ERROR_SUCCESS) {
                    // Чтение значений из реестра и заполнение структуры NOTE

                    DWORD nameSize = MAX_PATH;
                    wchar_t name[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"name", 0, NULL, reinterpret_cast<LPBYTE>(name), &nameSize) == ERROR_SUCCESS) {
                        noteStruct.name = QString::fromWCharArray(name);
                    }

                    DWORD additionalInfoSize = MAX_PATH;
                    wchar_t additionalInfo[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"additionalInfo", 0, NULL, reinterpret_cast<LPBYTE>(additionalInfo), &additionalInfoSize) == ERROR_SUCCESS) {
                        noteStruct.additionalInfo = QString::fromWCharArray(additionalInfo);
                    }

                    DWORD noteSize = MAX_PATH;
                    wchar_t note[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"note", 0, NULL, reinterpret_cast<LPBYTE>(note), &noteSize) == ERROR_SUCCESS) {
                        noteStruct.note = QString::fromWCharArray(note);
                    }

                    DWORD dataSize = sizeof(DWORD);
                    DWORD data;
                    if (RegQueryValueEx(subKey, L"data", 0, NULL, reinterpret_cast<LPBYTE>(&data), &dataSize) == ERROR_SUCCESS) {
                        noteStruct.data = static_cast<int>(data);
                    }

                    RegCloseKey(subKey);
                } else {
                    qWarning() << "Failed to open registry key for index: " << index;
                }

                return noteStruct;
            }
        } else if constexpr (std::is_same_v<T, STRUCTURES::DATA>) {
            if (baseStruct->getType() == STRUCTURES::DATA_TYPE) {
                STRUCTURES::DATA dataStruct;

                // Аналогично для структуры DATA
                HKEY subKey;
                QString subKeyName = createRegistryPath(QString::number(index));

                if (RegOpenKeyEx(HKEY_CURRENT_USER, subKeyName.toStdWString().c_str(), 0, KEY_READ, &subKey) == ERROR_SUCCESS) {
                    // Чтение значений из реестра и заполнение структуры DATA

                    DWORD nameSize = MAX_PATH;
                    wchar_t name[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"name", 0, NULL, reinterpret_cast<LPBYTE>(name), &nameSize) == ERROR_SUCCESS) {
                        dataStruct.name = QString::fromWCharArray(name);
                    }

                    DWORD additionalInfoSize = MAX_PATH;
                    wchar_t additionalInfo[MAX_PATH];
                    if (RegQueryValueEx(subKey, L"additionalInfo", 0, NULL, reinterpret_cast<LPBYTE>(additionalInfo), &additionalInfoSize) == ERROR_SUCCESS) {
                        dataStruct.additionalInfo = QString::fromWCharArray(additionalInfo);
                    }

                    DWORD dataActivitySize = sizeof(DWORD);
                    DWORD dataActivity;
                    if (RegQueryValueEx(subKey, L"dataActivity", 0, NULL, reinterpret_cast<LPBYTE>(&dataActivity), &dataActivitySize) == ERROR_SUCCESS) {
                        dataStruct.dataActivity = static_cast<int>(dataActivity);
                    }

                    DWORD dataSize = sizeof(DWORD);
                    DWORD data;
                    if (RegQueryValueEx(subKey, L"data", 0, NULL, reinterpret_cast<LPBYTE>(&data), &dataSize) == ERROR_SUCCESS) {
                        dataStruct.data = static_cast<int>(data);
                    }

                    RegCloseKey(subKey);
                } else {
                    qWarning() << "Failed to open registry key for index: " << index;
                }

                return dataStruct;
            }
        }
    }

    qWarning() << "Invalid index or mismatched type";
    return T();
}

// Реализация метода size
int ConfigManager::size() const {
    return structArray.size();
}

// Реализация метода createRegistryPath
QString ConfigManager::createRegistryPath(const QString &structureName) const {
    return "SOFTWARE\\PINACC\\" + structureName;
}

// Реализация метода getStructType
STRUCTURES::TYPES ConfigManager::getStructType(int index) const {
    if (index >= 0 && index < structArray.size()) {
        HKEY hKey;
        QString registryPath = createRegistryPath(QString::number(index));

        LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.toStdWString().c_str(), 0, KEY_READ, &hKey);
        if (result == ERROR_SUCCESS) {
            DWORD type;
            DWORD typeSize = sizeof(type);
            result = RegQueryValueEx(hKey, L"type", nullptr, nullptr, reinterpret_cast<LPBYTE>(&type), &typeSize);

            RegCloseKey(hKey);

            if (result == ERROR_SUCCESS) {
                return static_cast<STRUCTURES::TYPES>(type);
            } else {
                qWarning() << "Failed to read type from registry: " << result;
            }
        } else {
            qWarning() << "Failed to open registry key: " << result;
        }
    } else {
        qWarning() << "Invalid index";
    }

    return STRUCTURES::INVALID_TYPE;
}

void ConfigManager::removeStruct(int index) {
    if (index >= 0 && index < structArray.size()) {
        QString registryPath = createRegistryPath(QString::number(index));

        LONG result = RegDeleteTree(HKEY_CURRENT_USER, registryPath.toStdWString().c_str());

        if (result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND) {
            // Успешно удалили старый ключ или ключ не существует

            // Сдвигаем индексы для всех структур после удаленной
            for (int i = index + 1; i < structArray.size(); ++i) {
                QString oldPath = createRegistryPath(QString::number(i));
                QString newPath = createRegistryPath(QString::number(i - 1));

                HKEY hOldKey;
                if (RegOpenKeyEx(HKEY_CURRENT_USER, oldPath.toStdWString().c_str(), 0, KEY_READ, &hOldKey) == ERROR_SUCCESS) {
                    HKEY hNewKey;
                    if (RegCreateKeyEx(HKEY_CURRENT_USER, newPath.toStdWString().c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hNewKey, NULL) == ERROR_SUCCESS) {
                        // Копируем значения из старого ключа в новый
                        copyKeyValues(hOldKey, hNewKey);

                        // Закрываем новый ключ
                        RegCloseKey(hNewKey);
                    } else {
                        qWarning() << "Failed to create registry key: " << result;
                    }

                    // Закрываем старый ключ
                    RegCloseKey(hOldKey);
                } else {
                    qWarning() << "Failed to open registry key for copying: " << result;
                }
            }

            // Удаляем последний ключ, который стал дублироваться после сдвига
            QString lastPath = createRegistryPath(QString::number(structArray.size() - 1));
            RegDeleteTree(HKEY_CURRENT_USER, lastPath.toStdWString().c_str());

            // Удаляем последний элемент в массиве
            //structArray.pop_back();
            fillArray();
        } else {
            qWarning() << "Failed to delete registry key: " << result;
        }
    } else {
        qWarning() << "Invalid index";
    }
}

// Функция для копирования значений из одного ключа в другой
void ConfigManager::copyKeyValues(HKEY hSrcKey, HKEY hDestKey) {
    DWORD index = 0;
    WCHAR valueName[256];
    DWORD valueNameSize = sizeof(valueName) / sizeof(valueName[0]);
    DWORD valueType;
    BYTE valueData[1024];
    DWORD valueDataSize = sizeof(valueData);

    // Перебираем все значения в старом ключе
    while (RegEnumValue(hSrcKey, index++, valueName, &valueNameSize, NULL, &valueType, valueData, &valueDataSize) == ERROR_SUCCESS) {
        // Записываем значения в новый ключ
        RegSetValueEx(hDestKey, valueName, 0, valueType, valueData, valueDataSize);

        // Сбрасываем размеры для следующей итерации
        valueNameSize = sizeof(valueName) / sizeof(valueName[0]);
        valueDataSize = sizeof(valueData);
    }
}

// Реализация деструктора
ConfigManager::~ConfigManager() {
    qDeleteAll(structArray);
    structArray.clear();
}

template void ConfigManager::addStruct<STRUCTURES::PASSWORD>(const STRUCTURES::PASSWORD &);
template void ConfigManager::addStruct<STRUCTURES::NOTE>(const STRUCTURES::NOTE &);
template void ConfigManager::addStruct<STRUCTURES::DATA>(const STRUCTURES::DATA &);

template void ConfigManager::setStructure<STRUCTURES::PASSWORD>(int, const STRUCTURES::PASSWORD &);
template void ConfigManager::setStructure<STRUCTURES::NOTE>(int, const STRUCTURES::NOTE &);
template void ConfigManager::setStructure<STRUCTURES::DATA>(int, const STRUCTURES::DATA &);

template STRUCTURES::PASSWORD ConfigManager::getStruct<STRUCTURES::PASSWORD>(int) const;
template STRUCTURES::NOTE ConfigManager::getStruct<STRUCTURES::NOTE>(int) const;
template STRUCTURES::DATA ConfigManager::getStruct<STRUCTURES::DATA>(int) const;
