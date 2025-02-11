#ifndef TEMPLATES_H
#define TEMPLATES_H

#pragma once

#include "libraries.h"
#include "templates.h"

namespace STRUCTURES {

    enum TYPES {
        PASSWORD_TYPE,
        NOTE_TYPE,
        DATA_TYPE,
        INVALID_TYPE
    };

    class BaseStructure {
    public:
        QString name;
        QString additionalInfo;
        virtual ~BaseStructure() {}
        virtual TYPES getType() const = 0;
    };

    struct PASSWORD : public BaseStructure {
        QString site;
        QString login;
        QString password;
        int data;

        PASSWORD(const QString& name, const QString& site, const QString& login, const QString& password,
                 const QString& additionalInfo, int data)
            : site(site), login(login), password(password), data(data) {
            this->name = name;
            this->additionalInfo = additionalInfo;
        }

        TYPES getType() const override {
            return PASSWORD_TYPE;
        }

        PASSWORD() = default;
    };

    struct NOTE : public BaseStructure {
        QString note;
        int data;

        NOTE(const QString& name, const QString& note, const QString& additionalInfo, int data)
            : note(note), data(data) {
            this->name = name;
            this->additionalInfo = additionalInfo;
        }

        TYPES getType() const override {
            return NOTE_TYPE;
        }

        NOTE() = default;
    };

    struct DATA : public BaseStructure {
        int dataActivity;
        int data;

        DATA(const QString& name, const QString& additionalInfo, int dataActivity, int data)
            : dataActivity(dataActivity), data(data) {
            this->name = name;
            this->additionalInfo = additionalInfo;
        }

        TYPES getType() const override {
            return DATA_TYPE;
        }

        DATA() = default;
    };
}

#endif // TEMPLATES_H
