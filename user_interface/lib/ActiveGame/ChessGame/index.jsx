import React, { useState, useEffect } from 'react';
import ChessPiece from './ChessPiece.jsx';
import EndReport from './EndReport.jsx';
import PawnPromotionMenu from './PawnPromotionMenu.jsx';
import {sendMove} from '../../request/fetch';

function ChessGame(props) {
    const[selectedTile, setSelectedTile] = useState(null);
    const[selectableTiles, setSelectableTiles] = useState(null);
    const[renderBoard, setRenderBoard] = useState(null);
    const[endState, setEndState] = useState(null);
    const[displayMode, setDisplayMode] = useState("chess-grid");
    const[pawnPromotionCandidate, setPawnPromotionCandidate] = useState(null);
    const[playerTurn, setPlayerTurn] = useState(null);
    const[newGame, setNewGame] = useState(false);

    useEffect(() => {
        if(props.config.listener) {
            const buffer = props.config.listener.clearBuffer();
            buffer.forEach((e) => {
                websocketListener(e);
            });
            props.config.listener.setCustomListener(websocketListener)
        }
        if(props.resetGame()) {
            let strippedConfig = props.config;
            strippedConfig.listener = null;
            chess.postMessage({
                action: 'resetGame',
                config: strippedConfig
            });
            if(props.config.mode == 'online' && !props.config.hasOwnProperty('snapshot')) {
                setNewGame(true);
            }
        } else {
            chess.postMessage({});
        }
        return () => {
            if(props.config.listener) {
                props.config.listener.setDefaultListener();
            }
        };
    }, [props.config.gameID]);

    const websocketListener = (e) => {
        const data = JSON.parse(e.data);
        chess.postMessage({
            action: 'move',
            start: data.start,
            end: data.end,
            pieceType: data.pieceType
        });
    }

    chess.onmessage = (e) => {
        deselectTile();
        setRenderBoard(e.data.board);
        setPlayerTurn(e.data.playerTurn);
        if(e.data.hasOwnProperty('endState')) {
            setEndState(e.data.endState);
        }
    };

    const tileSelected = (x, y) => {
        return selectedTile && selectedTile.x == x && selectedTile.y == y;
    }

    const tileSelectable = (x, y, piece) => {
        let selectable = false;
        if(selectableTiles) {
            for(let i = 0; i < selectableTiles.length; i++) {
                if(selectableTiles[i].x == x && selectableTiles[i].y == y) {
                    selectable = true;
                }
            }
        } else if(!piece.null && !selectableTiles && playerTurn) {
            selectable = (playerTurn == props.config.player || !props.config.player) && piece.legalMoves.length > 0;
        }
        return selectable;
    }

    const move = async (start, end, pieceType) => {
        chess.postMessage({
            action: 'move',
            start: start, 
            end: end, 
            pieceType: pieceType
        });

        if(props.config.mode == 'online') {
            const move = {
                start: start, end: end, 
                pieceType: pieceType
            };
            let response;
            if(newGame) {
                response = await props.startGame(move);
            } else {
                response = await sendMove(props.config.gameID, move);
            }
        }
        setNewGame(false);
    }

    const promotePawn = (pieceType) => {
        move(selectedTile, pawnPromotionCandidate, pieceType);
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
        const piece = renderBoard[selectedTile.y][selectedTile.x];
        if(piece.type == "p" && (y == 7 || y == 0)) {
            setPawnPromotionCandidate(end);
            setDisplayMode("pawn-promotion");
        } else {
            move(start, end)
        }
    }

    const deselectTile = () => {
        setSelectedTile(null);
        setSelectableTiles(null);
    }

    const selectTile = (x, y, piece) => {
        if(tileSelectable(x, y, piece)) {
            if(piece.legalMoves.length == 0) {
                movePiece(x, y);
            } else {
                setSelectedTile({x: x, y: y});
                setSelectableTiles(piece.legalMoves);
            }
        }
    }

    const displayChessGrid = () => {
        if(renderBoard) {
            return(
                <div className="chess-board">
                    {renderBoard.map((row, y) => { 
                        return (
                            row.map((piece, x) => {
                                return (
                                    <ChessPiece
                                        key={x}
                                        piece={piece}
                                        position={{x: x, y: y}}
                                        selected={() => tileSelected(x, y)}
                                        selectable={() => tileSelectable(x, y, piece)}
                                        selectTile={() => selectTile(x, y, piece)}
                                        deselectTile={deselectTile}
                                        pieceColor={piece.player}
                                    />
                                )
                            })
                        )
                            
                    })}
                </div>
            );
        }
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

    const displayEndReport = () => {
        if(endState) {
            return <EndReport className="end-report" endState={endState} />
        }
    }

    return(
        <div className="chess-game">
            {displayChessBoard()}
            {displayEndReport()}
        </div>
    );
}

export default ChessGame;