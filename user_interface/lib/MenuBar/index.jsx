import React from 'react';

function MenuBar(props) {
    const buttonClass = (buttonTitle) => {
        let className = "menu-button";
        if(props.selected == buttonTitle) {
            className += ' selected-button';
        }
        return className;
    }

    const activeGameButton = () => {
        if(props.gameActive) {
            return (
                <button 
                    className={buttonClass('active-game')}
                    onClick={() => props.setDisplayMode('active-game')}
                >
                    Active Game
                </button>
            );
        }
    }

    const onlineGamesButton = () => {
        if(props.loggedIn) {
            return (
                <button 
                    className={buttonClass('online-games')}
                    onClick={() => props.setDisplayMode('online-games')}
                >
                    Online Games
                </button>
            );
        }
    }

    return (
        <div className="menu-bar">
            <button 
                className={buttonClass('new-game')}
                onClick={() => props.setDisplayMode('new-game')}
            >
                New Game
            </button>
            {onlineGamesButton()}
            {activeGameButton()}
        </div>
    );
}

export default MenuBar;