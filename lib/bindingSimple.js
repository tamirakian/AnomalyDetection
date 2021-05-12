const addon = require('../build/Release/test2-native');

function SimpleAnomalyDetector() {

    this.LearnNormal = function (timeseries) {
        return _addonInstance.LearnNormal(timeseries);
    }

    this.Detect = function (timeseries) {
        return _addonInstance.Detect(timeseries);
    }
    var _addonInstance = new addon.SimpleAnomalyDetector();
}

module.exports = SimpleAnomalyDetector;