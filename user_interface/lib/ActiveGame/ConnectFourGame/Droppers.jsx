import React from 'react';

function Droppers(props) {
    const dropperClass = (x) => {
        let type = 'medium-tile dark-grey-text';
        if(props.isLegal(x)) {
            type = `selectable-tile ${props.playerTurn}-text`;
        }
        return type;
    }

    return (
        <div className="dropper-container">
            {Array(props.length).fill().map((_, x) => {
                return (
                    <button 
                        className={`connect-four-dropper ${dropperClass(x)}`} 
                        onClick={() => props.dropTile(x)} 
                        key={x}
                    >
                        {'\u2B07\uFE0E'}
                    </button>
                );
            })}
        </div>
    );
}

export default Droppers;