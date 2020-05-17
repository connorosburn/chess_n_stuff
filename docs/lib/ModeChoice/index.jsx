import React from 'react';

function ModeChoice(props) {
    return (
        <div className="mode-choice">
            <button 
                onClick={() => props.choosePlayer(-1)}
                className="mode-button"
            >
                Local Multiplayer
            </button>
            <h1 className="ai-match-text">
                <p>AI Match:</p>
            </h1>
            <button
                onClick={() => props.choosePlayer(0)}
                className="mode-button"
            >
                White
            </button>
            <button
                onClick={() => props.choosePlayer(1)}
                className="mode-button"
            >
                Black
            </button>
        </div>
    );
}

export default ModeChoice;