import React, {useState} from 'react';
import ChessGame from './ChessGame';
import {startGame} from '../request/fetch'

function ActiveGame(props) {
    const firstMove = async (move) => {
        let response = await startGame(move, props.config.opponent);
        if(response.status == 200) {
            let data = await response.json();
            props.setGameID(data.id)
        } else {
            props.nullifyGame();
        }
    }
    switch(props.gameType) {
        case 'chess':
            return (
                <ChessGame
                    config={props.config}
                    resetGame={props.resetGame}
                    startGame={firstMove}
                />
            );
        default:
            return <div></div>;
    }
}

export default ActiveGame;