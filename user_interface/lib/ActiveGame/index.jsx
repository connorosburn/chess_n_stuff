import React, {useState} from 'react';
import ChessGame from './ChessGame';
import {startGame} from '../request/fetch'

function ActiveGame(props) {
    switch(props.gameType) {
        case 'chess':
            return (
                <ChessGame
                    config={props.config}
                    resetGame={props.resetGame}
                    startGame={props.startGame}
                />
            );
        default:
            return <div></div>;
    }
}

export default ActiveGame;