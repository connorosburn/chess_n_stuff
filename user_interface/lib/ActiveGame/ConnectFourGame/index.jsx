import React from 'react';

function ConnectFourGame(props) {
    const tileClass = (x, y, type) => {
        let className = `connect-four-${type}`;
        if(type == 'null') {
            className = 'grey';
        }
        return className;
    }

    const isLegal = (x) => {
        return props.gameData.hasOwnProperty('legalMoves') && props.gameData.legalMoves.includes(x)
    };

    const dropperClass = (x) => {
        let type = 'grey';
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
            <div className="connect-four-board">
                {Array(props.gameData.snapshot[0].length).fill().map((_, x) => {
                    return (
                        <button className={`connect-four-tile ${dropperClass(x)}`} onClick={() => dropTile(x)} key={x}>
                        </button>
                    );
                })}
                {props.gameData.snapshot.map((row, y) => {
                        return row.map((piece, x) => {
                            return (
                                <button 
                                    className={`${tileClass(x, y, piece)} connect-four-tile`}
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