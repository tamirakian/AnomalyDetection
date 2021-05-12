const addon = require('../build/Release/test2-native');

function featureID() {
    this.Set = function (vec, name) {
        return _addonInstance.Set(vec, name);
    }

    var _addonInstance = new addon.featureID();
}

module.exports = featureID;