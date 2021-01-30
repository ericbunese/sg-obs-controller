module.exports = (obs) => {
    const controller = {};

    controller.changeScene = ((req, res) => {
        var id = parseInt(req.params.id);

        var result = obs.changeScene(obs, id);

        if (result)
            res.status(200).json(result);
        else res.status(500).json(result);
    });

    return controller;
}