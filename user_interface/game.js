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
        Module.move(data.move);
    }
};

const formulateResponse = () => {
    let response = {
        snapshot: Module.getSnapshot(),
        playerTurn: Module.playerTurn()
    };


    const endState = Module.endState();

    if(endState.condition != "") {
        if(endState.winner == 'null') {
            response.endState = {
                condition: endState.condition,
                winner: null
            };
        } else {
            response.endState = {
                condition: endState.condition,
                winner: endState.winner
            };
        }
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