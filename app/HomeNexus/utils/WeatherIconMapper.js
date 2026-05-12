// WeatherIconMapper.js
.pragma library

function fileName(openWeatherIcon) {
    switch (openWeatherIcon) {
    case "01d":
        return "clear-day.svg";
    case "01n":
        return "clear-night.svg";
    case "02d":
        return "partly-cloudy-day.svg";
    case "02n":
        return "partly-cloudy-night.svg";
    case "03d":
    case "03n":
        return "cloudy.svg";
    case "04d":
    case "04n":
        return "overcast.svg";
    case "09d":
        return "overcast-day-rain.svg";
    case "09n":
        return "overcast-night-rain.svg";
    case "10d":
        return "partly-cloudy-day-rain.svg";
    case "10n":
        return "partly-cloudy-night-rain.svg";
    case "11d":
        return "thunderstorms-day-rain.svg";
    case "11n":
        return "thunderstorms-night-rain.svg";
    case "13d":
    case "13n":
        return "snow.svg";
    case "50d":
    case "50n":
        return "fog.svg";
    default:
        return "not-available.svg";
    }
}
