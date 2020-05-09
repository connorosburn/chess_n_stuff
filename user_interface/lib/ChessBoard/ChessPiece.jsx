import React, { useState, useEffect } from 'react';
import '../style/ChessBoard.css';

function ChessPiece(props) {
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
        if(props.selected || props.selectable()) {
            props.selectTile(props.position.x, props.position.y, props.piece);
        }
    }

    return (
        <div className="fill">
            <button 
                className={`fill ${tileClass()}`} 
                onClick={clickTile}
            >
                {props.piece.type}
            </button>
        </div>
    );
}

export default ChessPiece;