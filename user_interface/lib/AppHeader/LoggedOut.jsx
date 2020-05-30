import React from 'react';

function LoggedOut(props) {
    const buttonClass = (type) => {
        let className = 'login-button';
        if(props.selected == type) {
            className += ' selected-button';
        }
        return className;
    }
    
    return (
        <div className="login-buttons">
            <button
                className={buttonClass('login')}
                onClick={() => props.setDisplayMode('login')}
            >
                Login
            </button>
            <button
                className={buttonClass('registration')}
                onClick={() => props.setDisplayMode('registration')}
            >
                Register
            </button>
        </div>
    );
}

export default LoggedOut;