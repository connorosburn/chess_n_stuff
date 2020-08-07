import React from 'react';
import Droppers from './Droppers'

function ConnectFourGame(props) {
    const tileClass = (x, y, type) => {
        let className = `${type}-tile`;
        if(type == 'null') {
            className = 'medium-tile';
        }
        return className;
    }

    const isLegal = (x) => {
        return props.gameData.hasOwnProperty('legalMoves') && props.gameData.legalMoves.includes(x);
    };

    const dropTile = (x) => {
        if(isLegal(x)) {
            props.sendMove({column: x});
        }
    }

    if(props.gameData.hasOwnProperty('snapshot')) {
        console.log(props.gameData);
        return (
            <div className="grid-container">
                <Droppers 
                    isLegal={isLegal}
                    dropTile={dropTile}
                    length={props.gameData.snapshot[0].length}
                    playerTurn={props.gameData.playerTurn}
                />
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