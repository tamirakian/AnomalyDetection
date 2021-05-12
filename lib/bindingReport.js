const addon = require('../build/Release/test2-native');

function AnomalyReport(name, num) {

    var _addonInstance = new addon.AnomalyReport(name, num);
}

module.exports = AnomalyReport;