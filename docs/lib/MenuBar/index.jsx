import React from 'react';

function MenuBar(props) {
    const buttonClass = (buttonTitle) => {
        if(props.selected == buttonTitle) {
            return 'selected-menu-button';
        } else {
            return 'menu-button';
        }
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

    return (
        <div className="menu-bar">
                <button 
                    className={buttonClass('new-game')}
                    onClick={() => props.setDisplayMode('new-game')}
                >
                New Game
            </button>
            {activeGameButton()}
        </div>
    );
}

export default MenuBar;