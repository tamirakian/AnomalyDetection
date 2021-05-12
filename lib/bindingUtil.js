const addon = require('../build/Release/test2-native');

function anomaly_detection_util() {
    /*
    this.Avg = function (x, size) {
        return _addonInstance.Avg(x, size);
    }
    */
    this.Var = function (x, size) {
        return _addonInstance.Var(x, size);
    }

    this.Cov = function (x, y, size) {
        return _addonInstance.Cov(x, y, size);
    }

    this.Pearson = function (x, y, size) {
        return _addonInstance.Pearson(x, y, size);
    }

    this.Linear_reg = function (points, size) {
        return _addonInstance.Linear_reg(points, size);
    }

    this.Dev = function (p, points, size) {
        return _addonInstance.Dev(p, points, size);
    }

    var _addonInstance = new addon.anomaly_detection_util();
}

module.exports = anomaly_detection_util;