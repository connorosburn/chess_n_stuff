importScripts('wasmchess.js');
const boardSize = 8;

let config = {};

const actions = {
    resetGame: (data) => {
        Module.resetGame();
        if(data.config.hasOwnProperty('snapshot')) {
            Module.fromSnapshot(data.config.snapshot);
        }
        config = data.config;
    },

    move: (data) => {
        start = new Module.Position(data.start.x, data.start.y);
        end = new Module.Position(data.end.x, data.end.y);
        let success = false;

        if(isPawnPromotion(start, end)) {
            success = Module.promotePawn(start, end, data.pieceType);
        } else {
            success = Module.move(start, end);
        }
    }
};

const isPawnPromotion = (start, end) => {
    start = new Module.Position(start.x, start.y);
    end = new Module.Position(end.x, end.y);
    return Module.isPawnPromotion(start, end);
}

const formatBoard = () => {
    let board = [];
    const rawBoard = Module.getBoard();
    for(let i = 0; i < rawBoard.size(); i++) {
        if(i % boardSize == 0) {
            board.push([]);
        }
        const lastRow = board.length - 1;
        board[lastRow].push({
            type: rawBoard.get(i).type,
            player: rawBoard.get(i).player,
            legalMoves: []
        });
        const lastItem = board[lastRow].length - 1;
        for(let j = 0; j < rawBoard.get(i).legalMoves.size(); j++) {
            board[lastRow][lastItem].legalMoves.push({
                x: rawBoard.get(i).legalMoves.get(j).x,
                y: rawBoard.get(i).legalMoves.get(j).y
            });
        }
    }
    return board;
}

const formulateResponse = () => {
    let response = {
        board: formatBoard(),
        playerTurn: Module.playerTurn()
    };


    const endState = Module.endState();

    if(endState.condition != "") {
        if(endState.winner != 0 && endState.winner != 1) {
            response.endState = {
                condition: endState.condition,
                winner: null
            };
        } else {
            let winner;
            if(endState.winner == 0) {
                winner = 'white';
            } else if(endState.winner == 1) {
                winner = 'black';
            }
            response.endState = {
                condition: endState.condition,
                winner: winner
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