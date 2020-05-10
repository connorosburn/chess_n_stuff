import React from 'react';
import ReactDOM from 'react-dom';
import UserInterface from './UserInterface.jsx';
import Chess from './WasmWrapper/Chess.js';


Module.onRuntimeInitialized = function() {
    ReactDOM.render(<UserInterface chess={new Chess(Module)} />, document.getElementById('single-page-app'));
}