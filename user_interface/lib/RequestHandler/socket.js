class LiveGameListener {
    #socket
    #buffer = []
    #listener = null
    constructor(gameID) {
        this.#socket = new WebSocket(WSURL);
        this.#socket.onopen = () => {
            this.#socket.send(JSON.stringify({gameID: gameID}));
        }
        this.setCustomListener = this.setCustomListener.bind(this);
        this.setDefaultListener = this.setDefaultListener.bind(this);
        this.clearBuffer = this.clearBuffer.bind(this);
        this.setDefaultListener();
    }

    setCustomListener(listener) {
        this.#socket.removeEventListener('MessageEvent', this.#listener);
        this.#listener = listener;
        this.#socket.onmessage = this.#listener;
    }

    setDefaultListener() {
        this.#socket.removeEventListener('MessageEvent', this.#listener);
        this.#listener = (e) => {
            this.#buffer.push(e);
        };
        this.#socket.onmessage = this.#listener;
    }

    clearBuffer() {
        const contents = this.#buffer;
        this.#buffer = [];
        return contents;
    }
}

export default LiveGameListener;