import React from 'react';
import ReactDOM from 'react-dom';
import UserInterface from './UserInterface.jsx';


Module.onRuntimeInitialized = function() {
    ReactDOM.render(<UserInterface Module={Module} />, document.getElementById('single-page-app'));
}