import React, {useState} from 'react';

function Login(props) {
    const[username, setUsername] = useState('');
    const[password, setPassword] = useState('');

    const submitForm = async (event) => {
        event.preventDefault();
        let response = await props.login(username, password);
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
            <input type="submit" value="Login" />
        </form>
    );
}

export default Login;