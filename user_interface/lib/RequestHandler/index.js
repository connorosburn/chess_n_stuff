class RequestHandler {
    #url
    constructor(backendURL) {
        this.#url = backendURL;
    }

    startGame(move, opponent) {
        return this.#fetchBackend('/games', 'POST', {move: move, opponent: opponent});
    }
    
    sendMove(gameID, move) {
        return this.#fetchBackend(`/games/${gameID}`, 'PATCH', {move: move});
    }

    getGame(gameID) {
        return this.#fetchBackend(`/games/${gameID}`);
    }

    getGames() {
        return this.#fetchBackend('/games');
    }

    async checkLogin() {
        let response = await this.#fetchBackend('/check_login');
        let data = await response.json();
        return data;
    }

    login(username, password) {
        return this.#fetchBackend('/login', 'POST', {
            username: username,
            password: password
        });
    }

    logout() {
        return this.#fetchBackend('/logout', 'DELETE');
    }

    register(username, password, passwordConfirmation) {
        return this.#fetchBackend('/register', 'POST', {
            username: username,
            password: password,
            confirm_password: passwordConfirmation
        });
    }

    #fetchBackend(uri, method = 'GET', data = null) {
        let request = {
            method: method,
            headers: {
                'Content-Type': 'application/json'
            },
            credentials: 'include'
        }
        if(data) {
            request.body = JSON.stringify(data);
        }
        return fetch(`${this.#url}/app${uri}`, request);
    }
}

export default RequestHandler;