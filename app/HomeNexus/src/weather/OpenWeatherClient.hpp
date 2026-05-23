#pragma once

#include <QObject>

class OpenWeatherClient : public QObject
{
    Q_OBJECT
public:
    explicit OpenWeatherClient(QObject *parent = nullptr);

signals:
};
