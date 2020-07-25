importScripts('wasmgames.js');

let config = {};

const actions = {
    resetGame: (data) => {
        config = data.config;
        if(config.hasOwnProperty('snapshot')) {
            Module.fromSnapshot(config.gameType, config.snapshot);
        } else {
            Module.newGame(config.gameType);
        }
    },

    move: (data) => {
        Module.move(JSON.stringify(data.move));
    }
};

const formulateResponse = () => {
    let response = {
        snapshot: JSON.parse(Module.getSnapshot()),
        playerTurn: Module.playerTurn()
    };

    if(Module.playerTurn() == config.player || !config.hasOwnProperty('player')) {
        response.legalMoves = JSON.parse(Module.everyLegalMove());
    }

    const endState = Module.endState();
    
    if(endState != '') {
        response.endState = JSON.parse(endState);
    }
    return response;
}

onmessage = (e) => {
    if(e.data.hasOwnProperty('action')) {
        actions[e.data.action](e.data);
    }

    postMessage(formulateResponse());

    if(config.mode == 'ai' && Module.playerTurn() != config.player) {
        Module.aiMove();
        postMessage(formulateResponse());
    }
}