import React, { useState, useEffect } from 'react';
import EndReport from './EndReport.jsx';
import PawnPromotionMenu from './PawnPromotionMenu.jsx';
import ChessGrid from './ChessGrid.jsx';

function ChessGame(props) {
    const [displayMode, setDisplayMode] = useState(null);
    const [renderBoard, setRenderBoard] = useState(null);
    const [changedTiles, setChangedTiles] = useState(null);
    const [legalMoves, setLegalMoves] = useState(null);
    const [inverted, setInverted] = useState(false);
    const [pendingMove, setPendingMove] = useState(null);
    const [endState, setEndState] = useState(null);

    useEffect(() => {
        let board = props.gameData.snapshot.board;
        if(inverted && renderBoard) {
            const realBoard = invertBoard(renderBoard);
            diffTiles(board, realBoard);
            setRenderBoard(invertBoard(board));
        } else if(!inverted && renderBoard) {
            diffTiles(board, renderBoard);
            setRenderBoard(board);
        } else if(!renderBoard) {
            setRenderBoard(board);
        }
        setLegalMoves(props.gameData.legalMoves);
        if(props.gameData.hasOwnProperty('endState')) {
            setEndState(props.gameData.endState);
        }
    }, [props.gameData]);

    useEffect(() => {
        if(renderBoard && !displayMode) {
            setDisplayMode('chess-grid');
        }
    }, [renderBoard]);

    const diffTiles = (board, diffBoard) => {
        let differences = [];
        board.forEach((row, y) => {
            row.forEach((tile, x) => {
                let diffTile = diffBoard[y][x];
                let newTile = tile;
                if(diffTile.type == 'en-passant') {
                    diffTile.type = 'null'; 
                    diffTile.player = 'null'; 
                }
                if(newTile.type == 'en-passant') {
                    newTile.type = 'null';
                    newTile.player = 'null';
                }
                if(diffTile.type != newTile.type || diffTile.player != newTile.player) {
                    differences.push({x: x, y: y});
                }
            });
        });
        if(differences.length > 0) {
            setChangedTiles(differences);
        }
    }

    const isPawnPromotion = (move, board) => {
        return (
                board[move.start.y][move.start.x].type == 'pawn' && 
                (
                    (move.start.y == 1 && move.end.y == 0) ||
                    (move.start.y == 6 && move.end.y == 7)
                )
            );
    }

    const movePiece = (move) => {
        let realBoard = renderBoard;
        if(inverted) {
            realBoard = invertBoard(renderBoard);
        }
        if(isPawnPromotion(move, realBoard)) {
            setPendingMove(move);
            setDisplayMode('pawn-promotion');
        } else {
            props.sendMove(move);
        }
    }

    const promotePawn = (type) => {
        let move = pendingMove;
        move.pawnPromotion = type;
        props.sendMove(move);
        setPendingMove(null);
        setDisplayMode('chess-grid');
    }

    const invertBoard = (board) => {
        let invertedBoard = [...board]
        invertedBoard = invertedBoard.map((row) => {
            let revRow = [...row];
            revRow.reverse();
            return revRow;
        });
        invertedBoard.reverse();
        return invertedBoard;
    }

    const toggleInversion = () => {
        setInverted(!inverted);
        setRenderBoard(invertBoard(renderBoard));
    }

    const displayChessGame = () => {
        switch(displayMode) {
            case 'chess-grid':
                return (
                    <ChessGrid
                        renderBoard={renderBoard}
                        legalMoves={legalMoves}
                        changedTiles={changedTiles}
                        inverted={inverted}
                        move={movePiece}
                    />
                );
            case 'pawn-promotion':
                return (
                    <PawnPromotionMenu
                        promote={promotePawn}
                        cancel={() => {
                            setPendingMove(null);
                            setDisplayMode('chess-grid');
                        }}
                    />
                );
        }
    }

    const displayEndReport = () => {
        if(endState) {
            return <EndReport endState={endState} />
        }
    }

    return(
        <div className="grid-container">
            {displayChessGame()}
            <button 
                onClick={toggleInversion}
            >
                Invert Board
            </button>
            {displayEndReport()}
        </div>
    );
}

export default ChessGame;