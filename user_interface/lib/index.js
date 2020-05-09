import React from 'react';
import ReactDOM from 'react-dom';
import UserInterface from './UserInterface.jsx';
import Chess from './WasmWrapper/Chess.js';


Module.onRuntimeInitialized = function() {
    console.log(Module.getBoard().get(0));
    ReactDOM.render(<UserInterface chess={new Chess(Module)} />, document.getElementById('single-page-app'));
}