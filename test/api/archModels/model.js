const mongoose = require('mongoose');

const modelSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    train_data: { type: String, required: true },
    status: { type: String, required: true },
    upload_time: { type: String, required: true },
    // learn_normal_r: { type: String, required: false },
    // learn_normal_h: { type: String, required: false }

});

module.exports = mongoose.model('Model', modelSchema);