const express = require('express');
const app = express();
const mongoose = require('mongoose');

mongoose.connect(`mongodb+srv://${process.env.MONGO_USERNAME}:${process.env.MONGO_PASSWORD}@anomaly-detection-api.neocm.mongodb.net/myFirstDatabase?retryWrites=true&w=majority`, {
    useNewUrlParser: true,
    useUnifiedTopology: true
});

mongoose.connection.on('connected', () => {
    console.log('MongoDB Connected!');
});



app.use(express.json());

const modelRoutes = require('./api/routes/models');
const anomalyRoutes = require('./api/routes/anomalys');

// Routes
app.use('/api/model', modelRoutes);
app.use('/api/anomaly', anomalyRoutes);


module.exports = app;