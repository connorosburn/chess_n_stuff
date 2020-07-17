import React, { useState, useEffect } from 'react';
import ChessGame from './ChessGame';
import {startGame} from '../request/fetch'

function ActiveGame(props) {
    const[gameData, setGameData] = useState(null);
    const sendMove = async (move) => {
        // let response = await startGame(move, props.config.opponent);
        // if(response.status == 200) {
        //     let data = await response.json();
        //     props.setGameID(data.id)
        // } else {
        //     props.nullifyGame();
        // }
        game.postMessage({
            action: 'move',
            move: move
        });
    }

    useEffect(() => {
        let workerMessage = {};
        if(props.resetGame) {
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