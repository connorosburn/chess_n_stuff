import React, {useState} from 'react';

function GameMenu(props) {
    const[showMenu, setShowMenu] = useState(false);

    const menuClass = () => {
        let menuClass = 'hidden-game-menu';
        if(showMenu) {
            menuClass = 'game-menu';
        }
        return menuClass;
    }

    const selectedClass = () => {
        if(showMenu) {
            return 'selected-button';
        }
    }

    const gameMenuButtonClass = (gameType) => {
        let buttonClass = 'game-menu-button';
        if(!showMenu) {
            buttonClass = 'hidden-game-menu-button';
        } else if(gameType == props.gameType) {
            buttonClass = 'selected-game-menu-button';
        }
        return buttonClass;
    }

    const gameMenuButton = (gameType) => {
        return (
            <button
                className={gameMenuButtonClass(gameType)}
                onClick={() => {
                    props.setGameType(gameType);
                    setShowMenu(false);
                }}
            >
                {gameType.replace(/-/g, ' ')}
            </button>
        );
    }

    return (
        <div className="game-menu-root">
            <button 
                className={`game-menu-toggle ${selectedClass()}`}
                onClick={() => setShowMenu(!showMenu)}
            >
                <p>Other</p>
                <p className="hamburger-text">{'\u2630'}</p>
                <p>Games</p>
            </button>
            <div className={menuClass()}>
                {gameMenuButton('chess')}
                {gameMenuButton('tic-tac-toe')}
                {gameMenuButton('connect-four')}
            </div>
        </div>
    );
}

export default GameMenu;