const express = require('express');
const app = express();

app.use(express.json());

const home = require('./api/routes/home')
// Routes
app.use('/', home);

module.exports = app;

console.log('Server connected')