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