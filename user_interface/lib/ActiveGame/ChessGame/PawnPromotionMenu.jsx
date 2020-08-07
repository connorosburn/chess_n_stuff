import React from 'react';
import pieceCharacter from './pieceCharacter';

function PawnPromotionMenu(props) {
    const displayPromotionButton = (type) => {
        return (
            <button 
                className="promotion-tile"
                onClick={() => props.promote(type)}
            >
                {pieceCharacter(type)}
            </button>
        );
    }
    return(
        <div>
            <p>Choose type to promote pawn to</p>
            {displayPromotionButton('rook')}
            {displayPromotionButton('knight')}
            {displayPromotionButton('bishop')}
            {displayPromotionButton('queen')}
            <button
                className="promotion-tile"
                onClick={props.cancel}
            >
                cancel
            </button>
        </div>
    );
}

export default PawnPromotionMenu;