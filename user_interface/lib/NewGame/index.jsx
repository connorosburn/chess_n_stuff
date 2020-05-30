import React from 'react';
import OnlineMenu from './OnlineMenu';

function NewGame(props) {
    const setUpLocalGame = () => {
        props.setUpGame({
            mode: 'local'
        });
    }

    const setUpOnlineGame = (opponent) => {
        props.setUpGame({
            mode: 'online',
            player: 'white',
            opponent: opponent
        });
    }

    const setUpAIGame = (color) => {
        props.setUpGame({
            mode: 'ai',
            player: color
        });
    }
    return (
        <div className="mode-choice">
            <button 
                onClick={setUpLocalGame}
                className="mode-button"
            >
                Local Multiplayer
            </button>
            <OnlineMenu
                loggedIn={props.loggedIn}
                setUpGame={setUpOnlineGame}
                loginChecked={props.loginChecked}
            />
            <h1 className="ai-match-text">
                <p>AI Match:</p>
            </h1>
            <button
                onClick={() => setUpAIGame('white')}
                className="mode-button"
            >
                White
            </button>
            <button
                onClick={() => setUpAIGame('black')}
                className="mode-button"
            >
                Black
            </button>
        </div>
    );
}

export default NewGame;