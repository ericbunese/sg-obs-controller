const OBSWebSocket = require('obs-websocket-js');

module.exports = (config) => {
    const obsExp = {
        obs: new OBSWebSocket(),
        scenes: [],
        setup: (obsExp) => {
            obsExp.obs.connect({
                address: config.obs
            });

            obsExp.obs.on('ConnectionOpened', () => {
                obsExp.obs.send('GetSceneList').then(data => {
                    obsExp.scenes = data.scenes || [];
                })
            });
        },
        changeScene: (obs, index) => {
            var scene = (obs.scenes && obs.scenes.length > index) ? obs.scenes[index] : null;

            if (scene != null) {
                obs.obs.send('SetCurrentScene', {
                    'scene-name': scene.name
                });
                return true;
            }
            else {
                return false;
            }
        }
    };

    return obsExp;
}