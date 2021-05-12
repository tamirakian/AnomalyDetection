const mongoose = require('mongoose');
const Model = require('../archModels/model');
const APIdll = require("../../../lib/binding.js");

module.exports = {

    //GET one model by id
    getModel: (req, res) => {
        const modelId = req.params.modelId;

        Model.findById(modelId).then((model) => {
            res.status(200).json(
                model
            )
        }).catch(error => {
            res.status(500).json({
                error
            })
        });
    },

    //GET all models
    getAllModels: (req, res) => {
        Model.find().then((models) => {
            res.status(200).json({
                models
            })
        }).catch(error => {
            res.status(500).json({
                error
            })
        });
    },

    // POST model api
    postModel: (req, res) => {

        console.log("startttt");
        const Sinstance = new APIdll("reg_flight.csv", "anomaly_flight.csv", true);
        var x = Sinstance.GetAllAnomalyReportCpp();
        console.log(x);
        console.log("Tests passed- everything looks OK!");

        train_data = req.body.train_data;
        status = 'ready'

        var d = new Date();
        upload_time = d.toISOString()

        learn_normal_r = "asd rr"
        learn_normal_h = "asd hh"

        const model = new Model({
            _id: new mongoose.Types.ObjectId(),
            train_data,
            status,
            upload_time,
            learn_normal_r,
            learn_normal_h
        });


        model.save().then(() => {
            res.status(200).json({
                _id: model._id,
                upload_time: model.upload_time,
                status: model.status
            })
        }).catch(error => {
            res.status(500).json({
                error
            })
        });
    }

}