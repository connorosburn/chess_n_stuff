import React, { useState, useEffect } from 'react';
import ChessGame from './ChessGame';
import {startOnlineGame, sendOnlineMove} from '../request/fetch'

function ActiveGame(props) {
    const[gameData, setGameData] = useState(null);
    const[socket, setSocket] = useState(null);
    const sendMove = async (move) => {
        game.postMessage({
            action: 'move',
            move: move
        });
        if(props.config.mode == 'online') {
            if(props.config.hasOwnProperty('gameID')) {
                let response = await sendOnlineMove(props.config.gameID, move);
                if(response.status != 200) {
                    props.nullifyGame();
                }
            } else {
                let response = await startOnlineGame(props.gameType, move, props.config.opponent);
                if(response.status == 200) {
                    let data = await response.json();
                    props.setGameID(data.id)
                } else {
                    props.nullifyGame();
                }
            }
        }
    }

    useEffect(() => {
        if(props.config.mode == 'online' && props.config.gameID && !socket) {
            setSocket(new WebSocket(WSURL));
        }
    });

    useEffect(() => {
        if(socket) {
            socket.onopen = () => {
                socket.send(JSON.stringify({gameID: props.config.gameID}));
            };
        }
    }, [socket])

    useEffect(() => {
        if(socket) {
            socket.onmessage = (e) => {
                const data = JSON.parse(e.data);
                if(gameData.playerTurn == data.player) {
                    game.postMessage({
                        action: 'move',
                        move: JSON.parse(data.move)
                    });
                }
            };
        }
    }, [gameData]);

    useEffect(() => {
        let workerMessage = {};
        if(props.resetGame()) {
            setSocket(null);
            workerMessage.action = 'resetGame';
            workerMessage.config = props.config;
        }
        game.postMessage(workerMessage);
    }, []);

    game.onmessage = (e) => {
        setGameData(e.data);
    }

    if(gameData) {
        switch(props.gameType) {
            case 'chess':
                return (
                    <ChessGame
                        sendMove={sendMove}
                        gameData={gameData}
                    />
                );
        }
    }
    return <div></div>;
}

export default ActiveGame;