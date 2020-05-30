import React from 'react';
import ReactDOM from 'react-dom';
import UserInterface from './UserInterface.jsx';
import RequestHandler from './RequestHandler';

const requestHandler = new RequestHandler(URL)
ReactDOM.render(
    <UserInterface requestHandler={requestHandler} />, 
    document.getElementById('single-page-app')
);