// WeatherIconMapper.js
.pragma library

function fileName(openWeatherIcon) {
    switch (openWeatherIcon) {
    case "01d":
        return "weather/clear-day.svg";
    case "01n":
        return "weather/clear-night.svg";
    case "02d":
        return "weather/partly-cloudy-day.svg";
    case "02n":
        return "weather/partly-cloudy-night.svg";
    case "03d":
    case "03n":
        return "weather/cloudy.svg";
    case "04d":
    case "04n":
        return "weather/overcast.svg";
    case "09d":
        return "weather/overcast-day-rain.svg";
    case "09n":
        return "weather/overcast-night-rain.svg";
    case "10d":
        return "weather/partly-cloudy-day-rain.svg";
    case "10n":
        return "weather/partly-cloudy-night-rain.svg";
    case "11d":
        return "weather/thunderstorms-day-rain.svg";
    case "11n":
        return "weather/thunderstorms-night-rain.svg";
    case "13d":
    case "13n":
        return "weather/snow.svg";
    case "50d":
    case "50n":
        return "weather/fog.svg";
    default:
        return "weather/not-available.svg";
    }
}
