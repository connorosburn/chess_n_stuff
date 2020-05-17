const boardSize = 8;

class Chess {
    #player;
    constructor(player) {
            this.#player = player;
    }

    getBoard() {
        return Module.onRuntimeInitialized = () => {
            let board = [];
            const rawBoard = Module.getBoard();
            for(let i = 0; i < rawBoard.size(); i++) {
                if(i % boardSize == 0) {
                    board.push([]);
                }
                board[board.length - 1].push(rawBoard.get(i));
            }
            return board;
        }
    }

    move(start, end, pieceType) {
        start = new Module.Position(start.x, start.y);
        end = new Module.Position(end.x, end.y);
        let success = false;

        if(this.isPawnPromotion(start, end)) {
            success = Module.promotePawn(start, end, pieceType);
        } else {
            success = Module.move(start, end);
        }
        return success;
    }

    endState() {
        const endState = Module.endState();
        if(endState.condition == "") {
            return null;
        } else if(endState.winner != 0 && endState.winner != 1) {
            return {
                condition: endState.condition,
                winner: null
            };
        } else {
            return {
                condition: endState.condition,
                winner: endState.winner
            };
        }
    }

    isPawnPromotion(start, end) {
        start = new Module.Position(start.x, start.y);
        end = new Module.Position(end.x, end.y);
        return Module.isPawnPromotion(start, end);
    }
    
    updateAI() {
        let aiTurn = (this.#player == 1 || this.#player == 0) && Module.playerTurn() != this.#player;
        if(aiTurn) {
            Module.aiMove();
        }
        return aiTurn;
    }

    isAI(piece) {
        return (this.#player == 1 || this.#player == 0) && piece.player != this.#player && !piece.null;
    }
}

export default Chess;