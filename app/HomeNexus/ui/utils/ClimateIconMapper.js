// ClimateIconMapper.js
.pragma library

function temperatureIcon(temperature)
{
    if (temperature < 16)
        return "climate/temperature-low.svg"
    else if (temperature < 26)
        return "climate/temperature-medium.svg"
    else
        return "climate/temperature-high.svg"
}

function airQualityIcon(airQualityIndex)
{
    if (airQualityIndex <= 150)
        return "climate/emoji-smiley-white.svg"
    else
        return "climate/emoji-sad-white.svg"
}
