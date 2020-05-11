import React, { useState } from 'react';
import ChessPiece from './ChessPiece.jsx';
import EndReport from './EndReport.jsx';
import PawnPromotionMenu from './PawnPromotionMenu.jsx';

function ChessBoard(props) {
    const[selectedTile, setSelectedTile] = useState(null);
    const[selectableTiles, setSelectableTiles] = useState(null);
    const[renderBoard, setRenderBoard] = useState(props.chess.getBoard());
    const[endState, setEndState] = useState(props.chess.endState());
    const[displayMode, setDisplayMode] = useState("chess-grid");
    const[pawnPromotionCandidate, setPawnPromotionCandidate] = useState(null);

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

    const promotePawn = (pieceType) => {
        setRenderBoard(props.chess.move(selectedTile, pawnPromotionCandidate, pieceType));
        deselectTile();
        setEndState(props.chess.endState());
        setDisplayMode("chess-grid");
    }

    const movePiece = (x, y) => {
        const start = {
            x: selectedTile.x,
            y: selectedTile.y
        };
        const end = {
            x: x,
            y: y
        }
        if(props.chess.isPawnPromotion(start, end)) {
            setPawnPromotionCandidate(end);
            setDisplayMode("pawn-promotion");
        } else {
            setRenderBoard(props.chess.move(start, end));
            deselectTile();
            setEndState(props.chess.endState());
        }
    }

    const deselectTile = () => {
        setSelectedTile(null);
        setSelectableTiles(null);
    }

    const selectTile = (x, y, piece) => {
        if(tileSelectable(x, y, piece)) {
            if(piece.legalMoves.size() == 0) {
                movePiece(x, y);
            } else {
                setSelectedTile({x: x, y: y});
                setSelectableTiles(piece.legalMoves);
            }
        }
    }

    const displayChessGrid = () => {
        return(
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
                                        selectTile={() => selectTile(x, y, piece)}
                                        deselectTile={deselectTile}
                                        pieceColor={pieceColor(piece.player)}
                                    />
                                )
                            })}
                        </div>
                    )
                        
                })}
            </div>
        );
    }

    const displayChessBoard = () => {
        if(displayMode == 'chess-grid') {
            return displayChessGrid();
        } else if(displayMode == 'pawn-promotion') {
            return (
                <PawnPromotionMenu 
                    promote={promotePawn}
                    cancel={() => setDisplayMode('chess-grid')}
                />
            );
        }
    }

    return(
        <div className="chess-game">
            {displayChessBoard()}
            <EndReport endState={endState} pieceColor={pieceColor} />
        </div>
    );
}

export default ChessBoard;