import React, {useState} from 'react';

function OnlineMenu(props) {
    const[opponent, setOpponent] = useState('');
    if(!props.loginChecked) {
        return <div className="online-menu"></div>
    } else if(props.loggedIn) {
        return (
            <div className="online-menu">
                <p>Opponent Username:</p>
                <input type="text" value={opponent} onChange={(e) => setOpponent(e.target.value)} />
                <button
                    onClick={() => props.setUpGame(opponent)}
                >
                    Start Online Multiplayer
                </button>
            </div>
        );
    } else {
        return (
            <div className="online-menu">
                <p>Create low effort account for online multiplayer</p>
            </div>
        );
    }
}

export default OnlineMenu;