import React from 'react';
import LoggedOut from './LoggedOut';
import LoggedIn from './LoggedIn';

function LoginButtons(props) {
    if(!props.loginChecked) {
        return <div className="login-buttons"></div>
    } else if(props.userInfo) {
        return (
            <LoggedIn
                username={props.userInfo.username}
                resetPage={() => props.setDisplayMode('new-game')}
                logout={props.logout}
            />
        );
    } else {
        return (
            <LoggedOut
                setDisplayMode={props.setDisplayMode}
                selected={props.selected}
            />
        );
    }
}

export default LoginButtons;