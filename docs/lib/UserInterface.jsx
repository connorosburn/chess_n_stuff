import React, {useState} from 'react';
import ChessGame from './ChessGame';
import NewGame from './NewGame';
import Chess from './WasmWrapper/Chess.js';
import AppHeader from './AppHeader'
import MenuBar from './MenuBar';

function UserInterface(props) {
    const[displayMode, setDisplayMode] = useState('new-game');
    const[chess, setChess] = useState(null);
    const[gameActive, setGameActive] = useState(false);

    const choosePlayer = (player) => {
        setChess(new Chess(player));
        setDisplayMode('active-game');
        setGameActive(true);
    }

    const displayBody = () => {
        switch(displayMode) {
            case 'new-game':
                return <NewGame choosePlayer={choosePlayer} />
            case 'active-game':
                return <ChessGame chess={chess} />
            default:
                return <div></div>
        }
    }
    return (
        <div className="app-wrapper">
            <AppHeader />
            <MenuBar 
                selected={displayMode} 
                gameActive={gameActive}
                setDisplayMode={setDisplayMode}
            />
            <div className="app-body">
                {displayBody()}
            </div>
        </div>
    );
}

export default UserInterface;