import React from 'react';

function PawnPromotionMenu(props) {
    return(
        <div>
            <p>Choose type to promote pawn to</p>
            <button 
                className="promotion-tile"
                onClick={() => props.promote('r')}
            >
                {'\u265C'}
            </button>
            <button
                className="promotion-tile"
                onClick={() => props.promote('n')}
            >
                {'\u265E'}
            </button>
            <button 
                className="promotion-tile"
                onClick={() => props.promote('b')}
            >
                {'\u265D'}
            </button>
            <button 
                className="promotion-tile"
                onClick={() => props.promote('q')}
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