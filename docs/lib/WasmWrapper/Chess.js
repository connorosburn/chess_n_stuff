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

    move(start, end) {
        start = new this.#Module.Position(start.x, start.y);
        end = new this.#Module.Position(end.x, end.y);
        const success = this.#Module.move(start, end);
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