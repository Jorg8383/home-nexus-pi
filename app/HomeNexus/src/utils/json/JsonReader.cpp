#include "JsonReader.hpp"

bool HomeNexusUtils::JsonReader::readObject(const QJsonObject &parent, const QString &key, QJsonObject &out, const QString &context)
{
    const QJsonValue value = parent.value(key);
    const QString path = context.isEmpty() ? key : context + "." + key;
    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isObject())
    {
        qWarning() << "Expected" << path << "to be an object";
        return false;
    }
    out = value.toObject();
    return true;
}

bool HomeNexusUtils::JsonReader::readObjectAt(const QJsonArray &array, qsizetype index, QJsonObject &out, const QString &context)
{
    const QString path = QString("%1[%2]").arg(context).arg(index);

    if (index < 0 || index >= array.size())
    {
        qWarning() << "Array index out of range:" << path;
        return false;
    }

    const QJsonValue value = array.at(index);
    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isObject())
    {
        qWarning() << "Expected" << path << "to be an array";
        return false;
    }

    out = value.toObject();
    return true;
}

bool HomeNexusUtils::JsonReader::readArray(const QJsonObject &parent, const QString &key, QJsonArray &out, const QString &context)
{
    const QJsonValue value = parent.value(key);
    const QString path = context.isEmpty() ? key : context + "." + key;

    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isArray())
    {
        qWarning() << "Expected" << path << "to be an array";
        return false;
    }

    out = value.toArray();
    return true;
}

bool HomeNexusUtils::JsonReader::readDouble(const QJsonObject &parent, const QString &key, double &out, const QString &context)
{
    const QJsonValue value = parent.value(key);
    const QString path = context.isEmpty() ? key : context + "." + key;

    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isDouble())
    {
        qWarning() << "Expected" << path << "to be a double";
        return false;
    }

    out = value.toDouble();
    return true;
}

bool HomeNexusUtils::JsonReader::readString(const QJsonObject &parent, const QString &key, QString &out, const QString &context)
{
    const QJsonValue value = parent.value(key);
    const QString path = context.isEmpty() ? key : context + "." + key;

    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isString())
    {
        qWarning() << "Expected" << path << "to be a string";
        return false;
    }

    out = value.toString();
    return true;
}

bool HomeNexusUtils::JsonReader::readBool(const QJsonObject &parent, const QString &key, bool &out, const QString &context)
{
    const QJsonValue value = parent.value(key);
    const QString path = context.isEmpty() ? key : context + "." + key;

    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isBool())
    {
        qWarning() << "Expected" << path << "to be a bool";
        return false;
    }

    out = value.toBool();
    return true;
}

bool HomeNexusUtils::JsonReader::readInt(const QJsonObject &parent, const QString &key, int &out, const QString &context)
{
    const QJsonValue value = parent.value(key);
    const QString path = context.isEmpty() ? key : context + "." + key;

    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isDouble())
    {
        qWarning() << "Expected" << path << "to be a number";
        return false;
    }

    const double number = value.toDouble();

    if (std::floor(number) != number)
    {
        qWarning() << "Expected" << path << "to be an integer, but got:" << number;
        return false;
    }

    if (number < std::numeric_limits<int>::min() || number > std::numeric_limits<int>::max())
    {
        qWarning() << "Expected" << path << "to fit into int, but got:" << number;
        return false;
    }

    out = static_cast<int>(number);
    return true;
}

bool HomeNexusUtils::JsonReader::readInt64(const QJsonObject &parent, const QString &key, qint64 &out, const QString &context)
{
    const QJsonValue value = parent.value(key);
    const QString path = context.isEmpty() ? key : context + "." + key;

    if (value.isUndefined())
    {
        qWarning() << "Expected" << path << "to exist";
        return false;
    }
    else if (!value.isDouble())
    {
        qWarning() << "Expected" << path << "to be a number";
        return false;
    }

    const double number = value.toDouble();

    if (std::floor(number) != number)
    {
        qWarning() << "Expected" << path << "to be an integer, but got:" << number;
        return false;
    }

    out = static_cast<qint64>(number);
    return true;
}
