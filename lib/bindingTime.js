const addon = require('../build/Release/test2-native');

function TimeSeries(name) {

    var _addonInstance = new addon.TimeSeries(name);
}

module.exports = TimeSeries;