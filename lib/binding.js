const addon = require('../build/Release/test2-native');

function APIdll(name1, name2, bool) {

    this.GetAllAnomalyReportCpp = function () {
        return _addonInstance.GetAllAnomalyReportCpp();
    }
    var _addonInstance = new addon.APIdll(name1, name2, bool);
}

module.exports = APIdll;