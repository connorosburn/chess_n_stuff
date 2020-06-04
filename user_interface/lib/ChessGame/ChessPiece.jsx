import React from 'react';

function ChessPiece(props) {
    const pieceCharacters = (type) => {
        let character = '';
        switch(type) {
            case 'r':
                character = '\u265C\uFE0E';
                break;
            case 'n':
                character = '\u265E\uFE0E';
                break;
            case 'b':
                character = '\u265D\uFE0E';
                break;
            case 'q':
                character = '\u265B\uFE0E';
                break;
            case 'k':
                character = '\u265A\uFE0E';
                break;
            case 'p':
                character = '\u265F\uFE0E'; 
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
            className={`chess-tile ${tileClass()}`}
            onClick={clickTile}
        >
            <div className={`chess-piece ${props.pieceColor}`}>
                {pieceCharacters(props.piece.type)}
            </div>
        </button>
    );
}

export default ChessPiece;