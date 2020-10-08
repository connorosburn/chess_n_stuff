import React, {useState} from 'react';
import {register} from '../request';

function Registration(props) {
    const[username, setUsername] = useState('');
    const[password, setPassword] = useState('');
    const[passwordConfirmation, setPasswordConfirmation] = useState('');

    const submitForm = async (event) => {
        event.preventDefault();    
        let response = await register(username, password, passwordConfirmation);
        let data = await response.json();
        if(response.status == 200) {
            props.setUserInfo(data);
            props.resetPage();
        } else {
            console.log(data.message);
        }
    }

    return (
        <form onSubmit={submitForm} className="login-register">
            <label htmlFor="username">Username:</label>
            <input
                type="text"
                name="username"
                value={username}
                onChange={(event) => setUsername(event.target.value)}
            />
            <label htmlFor="password">Password:</label>
            <input
                type="password"
                name="password"
                value={password}
                onChange={(event) => setPassword(event.target.value)}
            />
            <label htmlFor="confirm_password">Confirm Password:</label>
            <input
                type="password"
                name="confirm_password"
                value={passwordConfirmation}
                onChange={(event) => setPasswordConfirmation(event.target.value)}
            />
            <input type="submit" value="Register" />
        </form>
    );
}

export default Registration;