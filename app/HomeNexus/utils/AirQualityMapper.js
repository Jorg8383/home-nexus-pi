// AirQualityMapper.js
.pragma library

function iaqText(iaq) {
    if (iaq === undefined || iaq === null) {
        return qsTr("Unknown")
    } else if (iaq <= 50) {
        return qsTr("Excellent")
    } else if (iaq <= 100) {
        return qsTr("Good")
    } else if (iaq <= 150) {
        return qsTr("Okay")
    } else if (iaq <= 200) {
        return qsTr("Poor")
    } else if (iaq <= 250) {
        return qsTr("Bad")
    } else if (iaq <= 350) {
        return qsTr("Very bad")
    } else {
        return qsTr("Critical")
    }
}
