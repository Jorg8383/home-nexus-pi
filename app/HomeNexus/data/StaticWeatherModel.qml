import QtQuick

ListModel {
    ListElement {
        // Location
        cityName: "Mehrstetten"
        country: "DE"
        longitude: 9.5639
        latitude: 48.3739
        timezone: 7200

        // Weather condition
        weatherId: 804
        weatherMain: "Clouds"
        weatherDescription: "overcast clouds"
        weatherIcon: "04d"

        // Temperature
        temp: 6.45
        feelsLike: 3.65
        tempMin: 5.09
        tempMax: 6.45

        // Atmosphere
        pressure: 1009
        humidity: 77
        seaLevel: 1009
        groundLevel: 930
        visibility: 10000

        // Wind
        windSpeed: 3.98
        windDeg: 224
        windGust: 7.12

        // Clouds
        cloudiness: 91

        // Time
        dt: 1778652926
        sunrise: 1778643884
        sunset: 1778698294
    }
}
