import React, {useState} from 'react';
import ChessGame from './ChessGame';
import ModeChoice from './ModeChoice';
import Chess from './WasmWrapper/Chess.js';
import AppHeader from './AppHeader'

function UserInterface(props) {
    const[displayMode, setDisplayMode] = useState('choose-mode');
    const[chess, setChess] = useState(null);

    const choosePlayer = (player) => {
        setChess(new Chess(player));
        setDisplayMode('chess-game');
    }

    const displayBody = () => {
        switch(displayMode) {
            case 'choose-mode':
                return <ModeChoice choosePlayer={choosePlayer} />
            case 'chess-game':
                return <ChessGame className="chess-game" chess={chess} />
            default:
                return <div></div>
        }
    }
    return (
        <div className="app-wrapper">
            <AppHeader className="app-header" />
            <div className="app-body">
                {displayBody()}
            </div>
        </div>
    );
}

export default UserInterface;