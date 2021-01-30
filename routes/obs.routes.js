module.exports = (app, obs) => {
    const controller = require('../controllers/obs.controller')(obs);

    app.route('/api/v1/obs/:id').get(controller.changeScene);
}