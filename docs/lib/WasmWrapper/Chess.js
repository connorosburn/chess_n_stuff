const boardSize = 8;

class Chess {
    #board;
    #Module;
    constructor(module) {
        this.#Module = module;
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

        return this.getBoard();
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