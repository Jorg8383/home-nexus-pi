#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QDebug>

namespace HomeNexusUtils
{
    class JsonReader
    {
    public:
        JsonReader() = delete; // this class is not meant to be instantiated

        static bool readObject(const QJsonObject& parent, const QString& key, QJsonObject& out, const QString& context = {});
        static bool readObjectAt(const QJsonArray& array, qsizetype index, QJsonObject& out, const QString& context = {});
        static bool readArray(const QJsonObject& parent, const QString& key, QJsonArray& out, const QString& context = {});
        static bool readDouble(const QJsonObject& parent, const QString& key, double& out, const QString& context = {});
        static bool readString(const QJsonObject& parent, const QString& key, QString& out, const QString& context = {});
        static bool readBool(const QJsonObject& parent, const QString& key, bool& out, const QString& context = {});
        static bool readInt(const QJsonObject& parent, const QString& key, int& out, const QString& context = {});
        static bool readInt64(const QJsonObject& parent, const QString& key, qint64& out, const QString& context = {});
    };
}

