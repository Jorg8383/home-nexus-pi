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
    if (airQualityIndex <= 100)
        return "climate/thumbs-up-green.svg"
    else if (airQualityIndex <= 200)
        return "climate/thumbs-down-orange.svg"
    else
        return "climate/thumbs-down-red.svg"
}
