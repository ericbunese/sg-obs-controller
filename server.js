const express = require('express');
const config = require('./config/express.json');
const obs = require('./obs/obs')(config);

module.exports = () => {
    const app = express();
    app.set('port', config.server.port);

    obs.setup(obs);
    require('./routes/obs.routes')(app, obs);

    return app;
};