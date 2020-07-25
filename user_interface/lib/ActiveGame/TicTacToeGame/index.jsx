import React, {useState, useEffect} from 'react';

function TicTacToeGame(props) {
    const[renderBoard, setRenderBoard] = useState(null);
    const[legalMoves, setLegalMoves] = useState(null);

    useEffect(() => {
        setRenderBoard(props.gameData.snapshot);
        setLegalMoves(props.gameData.legalMoves);
    }, [props.gameData]);

    const isLegal = (x, y) => {
        let legal = false;
        if(legalMoves) {
            legalMoves.forEach((position) => {
                legal = legal || (position.x == x && position.y == y);
            });
        }
        return legal;
    }

    const buttonClass = (x, y, type) => {
        let className = `ttt-${type}`;
        if(isLegal(x, y)) {
            className = 'selectable-tile';
        } else if(type == 'null') {
            className = 'grey';
        }
        return className;
    }

    const clickTile = (x, y) => {
        if(isLegal(x, y)) {
            props.sendMove({x: x, y: y});
        }
    }

    if(renderBoard) {
        return (
            <div className="ttt-board">
                {renderBoard.map((row, y) => {
                    return row.map((piece, x) => {
                        return (
                            <button 
                                className={`${buttonClass(x, y, piece)} ttt-tile`} 
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