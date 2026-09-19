#pragma once

#include <QQmlEngine>

#include "WeatherViewModel.hpp"

struct ForecastListModelForeign
{
    Q_GADGET
    QML_FOREIGN(ForecastListModel)
    QML_NAMED_ELEMENT(ForecastListModel)
    QML_UNCREATABLE("ForecastListModel is provided by WeatherViewModel")
};

struct WeatherViewModelForeign
{
    Q_GADGET
    QML_FOREIGN(WeatherViewModel)
    QML_SINGLETON
    QML_NAMED_ELEMENT(WeatherViewModel)

public:
    static WeatherViewModel *create(QQmlEngine *, QJSEngine *)
    {
        Q_ASSERT(s_instance);
        QQmlEngine::setObjectOwnership(s_instance, QQmlEngine::CppOwnership);
        return s_instance;
    }

    static void setInstance(WeatherViewModel *instance) { s_instance = instance; }

private:
    static inline WeatherViewModel *s_instance = nullptr;
};