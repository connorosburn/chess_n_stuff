import React from 'react';

function PawnPromotionMenu(props) {
    return(
        <div>
            <p>Choose type to promote pawn to</p>
            <button 
                className="promotion-tile"
                onClick={() => props.promote('rook')}
            >
                {'\u265C'}
            </button>
            <button
                className="promotion-tile"
                onClick={() => props.promote('knight')}
            >
                {'\u265E'}
            </button>
            <button 
                className="promotion-tile"
                onClick={() => props.promote('bishop')}
            >
                {'\u265D'}
            </button>
            <button 
                className="promotion-tile"
                onClick={() => props.promote('queen')}
            >
                {'\u265B'}
            </button>
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