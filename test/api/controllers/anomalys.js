const mongoose = require('mongoose');
const Model = require('../archModels/model');

module.exports = {

    //GET one model by id
    getAnomaly: (req, res) => {
        const modelId = req.params.modelId;

        // console.log(modelId);

        // if (modelType.localeCompare('hybrid')) {
        //     learnNormal = modelItem.learn_normal_h
        // }if (modelType.localeCompare('regression')) {
        //     learnNormal = modelItem.learn_normal_r
        // }

        Model.findById(modelId).then((model) => {
            res.status(200).json({
                anomalies: model
            })
        }).catch(error => {
            res.status(500).json({
                error
            })
        });
    }


}