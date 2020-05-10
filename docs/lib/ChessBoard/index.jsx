import React, { useState } from 'react';
import ChessPiece from './ChessPiece.jsx';
import EndReport from './EndReport.jsx';

function ChessBoard(props) {
    const[selectedTile, setSelectedTile] = useState(null);
    const[selectableTiles, setSelectableTiles] = useState(null);
    const[renderBoard, setRenderBoard] = useState(props.chess.getBoard());
    const[endState, setEndState] = useState(props.chess.endState());

    const pieceColor = (player) => {
        if(player == 1) {
            return 'black';
        } else if(player == 0) {
            return 'white'
        } else {
            return ' ';
        }
    }

    const tileSelected = (x, y) => {
        return selectedTile && selectedTile.x == x && selectedTile.y == y;
    }

    const tileSelectable = (x, y, piece) => {
        let selectable = false;
        if(selectableTiles) {
            for(let i = 0; i < selectableTiles.size(); i++) {
                if(selectableTiles.get(i).x == x && selectableTiles.get(i).y == y) {
                    selectable = true;
                }
            }
        } else if(!piece.null && !selectableTiles) {
            selectable = piece.legalMoves.size() > 0;
        }
        return selectable;
    }

    const selectTile = (x, y, piece) => {
        if(!piece.null && tileSelected(x, y)) {
            setSelectedTile(null);
            setSelectableTiles(null);
        } else if(tileSelectable(x, y, piece)) {
            if(piece.legalMoves.size() == 0) {
                setRenderBoard(props.chess.move(
                    {
                        x: selectedTile.x,
                        y: selectedTile.y
                    },
                    {
                        x: x,
                        y: y
                    }
                ));
                setSelectedTile(null);
                setSelectableTiles(null);
                setEndState(props.chess.endState());
            } else {
                setSelectedTile({x: x, y: y});
                setSelectableTiles(piece.legalMoves);
            }
        }
    }

    return(
        <div className="chess-game">
            <div className="chess-board">
                {renderBoard.map((row, y) => { 
                    return (
                        <div className="board-row" key={y}>
                            {row.map((piece, x) => {
                                return (
                                    <ChessPiece
                                        key={x}
                                        piece={piece}
                                        position={{x: x, y: y}}
                                        selected={() => tileSelected(x, y)}
                                        selectable={() => tileSelectable(x, y, piece)}
                                        selectTile={selectTile}
                                        pieceColor={pieceColor(piece.player)}
                                    />
                                )
                            })}
                        </div>
                    )
                        
                })}
            </div>
            <EndReport endState={endState} pieceColor={pieceColor} />
        </div>
    );
}

export default ChessBoard;