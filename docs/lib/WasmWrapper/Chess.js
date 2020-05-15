const boardSize = 8;

class Chess {
    #board;
    #Module;
    #player;
    constructor(module, player) {
        this.#Module = module;
        this.#player = player;
        this.#updateBoard();
    }

    getBoard() {
        return this.#board;
    }

    move(start, end, pieceType) {
        start = new this.#Module.Position(start.x, start.y);
        end = new this.#Module.Position(end.x, end.y);
        let success = false;

        if(this.isPawnPromotion(start, end)) {
            success = this.#Module.promotePawn(start, end, pieceType);
        } else {
            success = this.#Module.move(start, end);
        }

        if(success) {
            this.#updateBoard();
        } else {
            throw "Invalid move";
        }
    }

    endState() {
        const endState = this.#Module.endState();
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
        start = new this.#Module.Position(start.x, start.y);
        end = new this.#Module.Position(end.x, end.y);
        return this.#Module.isPawnPromotion(start, end);
    }
    
    updateAI() {
        let aiTurn = (this.#player == 1 || this.#player == 0) && this.#Module.playerTurn() != this.#player;
        if(aiTurn) {
            this.#Module.aiMove();
            this.#updateBoard();
        }
        return aiTurn;
    }

    isAI(piece) {
        return (this.#player == 1 || this.#player == 0) && piece.player != this.#player && !piece.null;
    }

    #updateBoard() {
        this.#board = [];
        const rawBoard = this.#Module.getBoard();
        for(let i = 0; i < rawBoard.size(); i++) {
            if(i % boardSize == 0) {
                this.#board.push([]);
            }
            this.#board[this.#board.length - 1].push(rawBoard.get(i));
        }
    }
}

export default Chess;