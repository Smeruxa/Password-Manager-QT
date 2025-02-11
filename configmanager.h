#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "libraries.h"
#include "templates.h"
#include <Windows.h>
#include <winreg.h>

class ConfigManager : public QObject
{
    Q_OBJECT
public:

    ConfigManager();
    ~ConfigManager();

    int fillArray();

    template<typename T>
    void addStruct(const T &myStruct);

    template<typename T>
    T getStruct(int index) const;

    int size() const;

    void load();
    template<typename T>
    void setStructure(int index, const T &myStruct);

    STRUCTURES::TYPES getStructType(int index) const;
    void removeStruct(int index);

private:

    void copyKeyValues(HKEY hSrcKey, HKEY hDestKey);

    QList<STRUCTURES::BaseStructure*> structArray;
    QString createRegistryPath(const QString &structureName) const;
};

#endif // CONFIGMANAGER_H
