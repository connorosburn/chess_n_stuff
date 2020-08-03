import React from 'react';

function ConnectFourGame(props) {
    const tileClass = (x, y, type) => {
        let className = `${type}-tile`;
        if(type == 'null') {
            className = 'medium-tile';
        }
        return className;
    }

    const isLegal = (x) => {
        return props.gameData.hasOwnProperty('legalMoves') && props.gameData.legalMoves.includes(x)
    };

    const dropperClass = (x) => {
        let type = 'medium-tile';
        if(isLegal(x)) {
            type = 'selectable-tile';
        }
        return type;
    }

    const dropTile = (x) => {
        if(isLegal(x)) {
            props.sendMove({column: x});
        }
    }

    if(props.gameData.hasOwnProperty('snapshot')) {
        return (
            <div className="grid-container">
                {Array(props.gameData.snapshot[0].length).fill().map((_, x) => {
                    return (
                        <button className={`grid-tile-7 ${dropperClass(x)}`} onClick={() => dropTile(x)} key={x}>
                        </button>
                    );
                })}
                {props.gameData.snapshot.map((row, y) => {
                        return row.map((piece, x) => {
                            return (
                                <button 
                                    className={`grid-tile-${row.length} ${tileClass(x, y, piece)} divided-tile`}
                                    key={x}
                                >
                                    
                                </button>
                            );
                        })
                    })}
            </div>
        );
    }
}

export default ConnectFourGame;