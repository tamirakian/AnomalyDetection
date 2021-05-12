const addon = require('../build/Release/test2-native');

function Line(float1, float2) {

    this.func = function(num1) {
        return _addonInstance.func(num1);
    }
    var _addonInstance = new addon.Line(float1, float2);
}

module.exports = Line;

