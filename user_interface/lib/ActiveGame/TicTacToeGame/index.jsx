import React from 'react';

function TicTacToeGame(props) {    
    const isLegal = (x, y) => {
        let legal = false;
        if(props.gameData.hasOwnProperty('legalMoves')) {
            props.gameData.legalMoves.forEach((position) => {
                legal = legal || (position.x == x && position.y == y);
            });
        }
        return legal;
    }

    const buttonClass = (x, y, type) => {
        let className = `${type}-tile`;
        if(isLegal(x, y)) {
            className = 'selectable-tile';
        } else if(type == 'null') {
            className = 'medium-tile';
        }
        return className;
    }

    const clickTile = (x, y) => {
        if(isLegal(x, y)) {
            props.sendMove({x: x, y: y});
        }
    }

    if(props.gameData.hasOwnProperty('snapshot')) {
        return (
            <div className="grid-container">
                {props.gameData.snapshot.map((row, y) => {
                    return row.map((piece, x) => {
                        return (
                            <button 
                                className={`grid-tile-${row.length} ${buttonClass(x, y, piece)} divided-tile`} 
                                key={x}
                                onClick={() => clickTile(x, y)}
                            >
                                {' '}
                            </button>
                        );
                    })
                })}
            </div>
        );
    } else {
        return <div />
    }
}

export default TicTacToeGame;