import React, {useState} from 'react';
import ChessBoard from './ChessBoard';
import ModeChoice from './ModeChoice';
import Chess from './WasmWrapper/Chess.js';

function UserInterface(props) {
    const[displayMode, setDisplayMode] = useState('choose-mode');
    const[chess, setChess] = useState(null);

    const choosePlayer = (player) => {
        setChess(new Chess(Module, player));
        setDisplayMode('chess-game');
    }

    const displayBody = () => {
        switch(displayMode) {
            case 'choose-mode':
                return <ModeChoice choosePlayer={choosePlayer} />
            case 'chess-game':
                return <ChessBoard chess={chess} />
            default:
                return <div></div>
        }
    }
    return (
        displayBody()
    );
}

export default UserInterface;