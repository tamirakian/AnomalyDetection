const express = require('express');
const router = express.Router();

const {
    postModel,
    getAllModels,
    getModel

} = require('../controllers/models');

router.post('/', postModel);
router.get('/', getAllModels);
router.get('/:modelId', getModel);

module.exports = router;