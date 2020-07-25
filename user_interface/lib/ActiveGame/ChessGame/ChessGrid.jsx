import React, {useState, useEffect} from 'react';
import pieceCharacter from './pieceCharacter';

function ChessGrid(props) {
    const[selectedTile, setSelectedTile] = useState(null);

    useEffect(() => {
        setSelectedTile(null);
    }, [props.renderBoard]);

    
    const containsPosition = (position, positionList) => {
        let contains = false;
        positionList.forEach((listPosition) => {
            contains = contains || 
                        (position.x == listPosition.x && position.y == listPosition.y);
        });
        return contains;
    }

    const moveChoiceTileClass = (x, y) => {
        let tileState = '';
        if(selectedTile.x == x && selectedTile.y == y) {
            tileState = 'selected-tile';
        } else {
            const potentialEnds = props.legalMoves.filter((move) => {
                return move.start.x == selectedTile.x && move.start.y == selectedTile.y;
            }).map(m => m.end);
            const tile = {x: x, y: y};
            if(containsPosition(tile, potentialEnds)) {
                tileState = 'selectable-tile';
            }
        }
        return tileState;
    }

    const pieceChoiceTileClass = (x, y) => {
        let tileState = '';
        const starts = props.legalMoves.map((move) => move.start);
        if(containsPosition({x: x, y: y}, starts)) {
            tileState = 'selectable-tile';
        }
        return tileState;
    }

    const tileClass = (x, y) => {
        let tileState = '';
        if(props.legalMoves) {
            if(selectedTile) {
                tileState = moveChoiceTileClass(x, y);
            } else {
                tileState = pieceChoiceTileClass(x, y);
            }
        }
        if( props.changedTiles && 
            tileState == '' && 
            containsPosition({x: x, y: y}, props.changedTiles)) {
            tileState = 'changed-Tile';
        }

        if(tileState == '' && (x + y) % 2 == 0) {
            tileState = 'light-tile';
        } else if(tileState == '' && (x + y) % 2 == 1) {
            tileState = 'dark-tile';
        }
        return tileState;
    }
    
    const clickTile = (position) => {
        if(selectedTile && position.x == selectedTile.x && position.y == selectedTile.y) {
            setSelectedTile(null);
        } else if(selectedTile) {
            const eligibleEnds = props.legalMoves.filter((move) => {
                return move.start.x == selectedTile.x && move.start.y == selectedTile.y;
            }).map(m => m.end);
            if(containsPosition(position, eligibleEnds)) {
                props.move({start: selectedTile, end: position});
            }
        } else if (
            containsPosition(
                position, 
                props.legalMoves.map(move => move.start)
            )
        ) {
            setSelectedTile(position);
        }
    }

    if(props.renderBoard) {
        return (
            <div className="chess-board">
                {props.renderBoard.map((row, y) => {
                    if(props.inverted) {
                        y = props.renderBoard.length - 1 - y;
                    }
                    return (
                        row.map((piece, x) => {
                            if(props.inverted) {
                                x = props.renderBoard[y].length - 1 - x;
                            }
                            return (
                                <button 
                                    className={`chess-tile ${tileClass(x, y)}`}
                                    onClick={() => clickTile({x: x, y: y})}
                                    key={x}
                                >
                                    <div className={`chess-piece ${piece.player}`} key={x}>
                                        {pieceCharacter(piece.type)}
                                    </div>
                                </button>
                            );
                        })
                    );
                        
                })}
            </div>
        );
    }
}

export default ChessGrid;