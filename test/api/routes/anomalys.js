const express = require('express');
const router = express.Router();

const {
    getAnomaly

} = require('../controllers/anomalys');

router.get('/:modelId', getAnomaly);

module.exports = router;