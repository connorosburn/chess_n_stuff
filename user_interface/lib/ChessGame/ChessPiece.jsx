import React from 'react';

function ChessPiece(props) {
    const pieceCharacters = (type) => {
        let character = '';
        switch(type) {
            case 'r':
                character = '\u265C';
                break;
            case 'n':
                character = '\u265E';
                break;
            case 'b':
                character = '\u265D';
                break;
            case 'q':
                character = '\u265B';
                break;
            case 'k':
                character = '\u265A';
                break;
            case 'p':
                character = '\u265F'; 
                break;
            default: 
                character = ' ';
        }
        return character;
    }

    const tileClass = () => {
        if(props.selected()) {
            return 'selected-tile';
        } else if(props.selectable()) {
            return 'selectable-tile';
        } else if((props.position.x + props.position.y) % 2 == 0) {
            return 'light-tile';
        } else {
            return 'dark-tile';
        }
    }

    const clickTile = () => {
        if(props.selectable()) {
            props.selectTile();
        } else if(props.selected()) {
            props.deselectTile();
        }
    }

    return (
        <button 
            className={`chess-piece ${tileClass()} ${props.pieceColor}`}
            onClick={clickTile}
        >
            {pieceCharacters(props.piece.type)}
        </button>
    );
}

export default ChessPiece;