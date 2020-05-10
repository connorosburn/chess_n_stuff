import React from 'react';
import ChessBoard from './ChessBoard';

function UserInterface(props) {
    return (
        <ChessBoard chess={props.chess} />
    );
}

export default UserInterface;