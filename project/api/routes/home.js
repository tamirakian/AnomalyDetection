const express = require('express');
//const router = express.Router();

const fileUpload = require('express-fileupload')
const app = express()

/////////
app.use(express.Router())

app.use(express.urlencoded({
    extended: false
}))
app.use(fileUpload({}))
app.use(express.static('view'))
////////////

const {
    getHomePage,
    postDetect,
    postDetact

} = require('../controllers/home');

app.get('/', getHomePage);
app.post('/detact', postDetact);
app.post('/detect', postDetect);

module.exports = app;