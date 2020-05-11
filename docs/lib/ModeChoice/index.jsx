import React from 'react';

function ModeChoice(props) {
    return (
        <div>
            <button onClick={() => props.choosePlayer(-1)}>Local Multiplayer</button>
            <p>AI Match:</p>
            <button onClick={() => props.choosePlayer(0)}>White</button>
            <button onClick={() => props.choosePlayer(1)}>Black</button>
        </div>
    );
}

export default ModeChoice;