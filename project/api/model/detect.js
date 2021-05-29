const APIdll = require("../../../lib/binding.js");

function detectAnomaly(normalFile, anomalyFile, algoType) {

    if (algoType == 'regression') {
        algoTypeBool = true
    } else if (algoType == 'hybrid') {
        algoTypeBool = false
    }

    const Sinstance = new APIdll(normalFile, anomalyFile, algoTypeBool);
    var result = Sinstance.GetAllAnomalyReportCpp();
    var json = JSON.parse(result);
    return json
}
//export file to module
module.exports.detectAnomaly = detectAnomaly