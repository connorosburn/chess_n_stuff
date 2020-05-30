import React from 'react';
import LoginButtons from './LoginButtons.jsx';

function AppHeader(props) {
    return (
        <div className="app-header">
            <div className="left-margin"></div>
            <div className="title">
                <h1>Chess N' Stuff</h1>
            </div>
            <LoginButtons
                selected={props.selected} 
                setDisplayMode={props.setDisplayMode}
                logout={props.logout}
                userInfo={props.userInfo}
                loginChecked={props.loginChecked}
            />
        </div>
    );
}

export default AppHeader;