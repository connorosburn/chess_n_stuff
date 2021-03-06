const fetchBackend = (uri, method = 'GET', data = null) => {
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
    return fetch(`${URL}/app${uri}`, request);
}

export const register = (username, password, passwordConfirmation) => {
    return fetchBackend('/register', 'POST', {
        username: username,
        password: password,
        confirm_password: passwordConfirmation
    });
}

export const logout = () => {
    return fetchBackend('/logout', 'DELETE');
}

export const login = (username, password) => {
    return fetchBackend('/login', 'POST', {
        username: username,
        password: password
    });
}

export const checkLogin = async () => {
    let response = await fetchBackend('/check_login');
    let data = await response.json();
    return data;
}

export const getGames = (gameType) => {
    return fetchBackend(`/games/${gameType}`);
}

export const getGame = (gameID) => {
    return fetchBackend(`/game/${gameID}`);
}

export const sendOnlineMove = (gameID, move) => {
    return fetchBackend(`/game/${gameID}`, 'PATCH', {move: JSON.stringify(move)});
}

export const startOnlineGame = (gameType, move, opponent) => {
    return fetchBackend(`/games/${gameType}`, 'POST', {move: JSON.stringify(move), opponent: opponent});
}