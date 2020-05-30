import React from 'react';

function LoggedIn(props) {
    const logout = () => {
        props.logout();
        props.resetPage();
    }
    
    return (
        <div className="login-buttons">
            <p>Logged in as {props.username}</p>
            <button
                className="login-button"
                onClick={logout}
            >
                Logout
            </button>
        </div>
    );
}

export default LoggedIn;