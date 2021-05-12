const addon = require('../build/Release/test2-native');

function Point(float1, float2) {

    this.GetX = function () {
        return _addonInstance.GetX();
    }
    var _addonInstance = new addon.Point(float1, float2);
}

module.exports = Point;