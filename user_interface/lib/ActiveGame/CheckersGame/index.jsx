import React, {useState, useEffect} from 'react';

function CheckersGame(props) {
    const [renderBoard, setRenderBoard] = useState(null);
    const [jumpTrack, setJumpTrack] = useState(null);
    const [selectedTile, setSelectedTile] = useState(null);
    const [inverted, setInverted] = useState(false);
    const [changedTiles, setChangedTiles] = useState([]);

    const diffTiles = (board, diffBoard) => {
        let differences = [];
        board.forEach((row, y) => {
            row.forEach((tile, x) => {
                let diffTile = diffBoard[y][x];
                let newTile = tile;
                if(diffTile.type != newTile.type || diffTile.player != newTile.player) {
                    differences.push({x: x, y: y});
                }
            });
        });
        if(differences.length > 0) {
            setChangedTiles(differences);
        }
    }

    const deepBoardCopy = (board) => {
        return board.map((row) => {
            return [...row]
        });
    }

    useEffect(() => {
        setSelectedTile(null);
        if(inverted) {
            if(renderBoard) {
                diffTiles(invertBoard(renderBoard), props.gameData.snapshot.board);
            }
            setRenderBoard(invertBoard(props.gameData.snapshot.board));
        } else {
            if(renderBoard) {
                diffTiles(renderBoard, props.gameData.snapshot.board);
            }
            setRenderBoard(deepBoardCopy(props.gameData.snapshot.board));
        }
    }, [props.gameData]);

    useEffect(() => {
        if(jumpTrack) {
            const jump = deepestJump();
            if(jump.childJumps.length > 0) {
                let board;
                if(inverted) {
                    board = invertBoard(renderBoard);
                } else {
                    board = deepBoardCopy(renderBoard);
                }
                board[jump.destinationPosition.y][jump.destinationPosition.x] = board[jump.currentPosition.y][jump.currentPosition.x];
                board[jump.currentPosition.y][jump.currentPosition.x] = {
                    player: 'null',
                    type: 'null'
                };
                board[jump.attackPosition.y][jump.attackPosition.x] = {
                    player: 'null',
                    type: 'null'
                };
                if(inverted) {
                    setRenderBoard(invertBoard(board));
                } else {
                    setRenderBoard(board);
                }
            } else {
                props.sendMove({
                    start: {x: selectedTile.x, y: selectedTile.y},
                    path: jumpTrack
                });
                setJumpTrack(null);
            }
        }
    }, [jumpTrack]);

    const jumpPiece = (x, y) => {
        if(jumpTrack) {
            let newTrack = [...jumpTrack];
            newTrack.push({x: x, y: y});
            setJumpTrack(newTrack);
        } else {
            setJumpTrack([{x: x, y: y}]);
        }
    }

    const rootJump = () => {
        let jump;
        if(jumpTrack) {
            props.gameData.legalMoves.forEach((move) => {
                if(move.type == 'jump' && 
                move.currentPosition.x == selectedTile.x && move.currentPosition.y == selectedTile.y &&
                move.destinationPosition.x == jumpTrack[0].x && move.destinationPosition.y == jumpTrack[0].y) {
                    jump = move;
                }
            });
        }
        return jump;
    }

    const deepestJump = (deepest = rootJump(), trackIndex = 1) => {
        if(trackIndex < jumpTrack.length) {
            let jump;
            deepest.childJumps.forEach((child) => {

                const matchedJump = (
                    child.destinationPosition.x == jumpTrack[trackIndex].x && 
                    child.destinationPosition.y == jumpTrack[trackIndex].y
                );

                if(matchedJump) {
                    jump = deepestJump(child, trackIndex + 1);
                }
            });
            return jump;
        } else {
            return deepest;
        }
    }

    const validJump = (x, y) => {
        let valid = false;
        if(props.gameData.legalMoves && jumpTrack) {
            const jump = deepestJump();
            jump.childJumps.forEach((child) => {
                valid = valid || (
                    child.destinationPosition.x == x && child.destinationPosition.y == y
                );
            });
        } else if(props.gameData.legalMoves) {
            props.gameData.legalMoves.forEach((move) => {
                valid = valid || (
                    selectedTile && move.type == 'jump' &&
                    move.currentPosition.x == selectedTile.x && move.currentPosition.y == selectedTile.y &&
                    move.destinationPosition.x == x && move.destinationPosition.y == y
                );
            });
        }
        return valid;
    }

    const standardMove = (x, y) => {
        let standard = false;
        if(props.gameData.legalMoves && selectedTile && !jumpTrack) {
            props.gameData.legalMoves.forEach((move) => {
                standard = standard || (
                    move.type == 'standard' &&
                    selectedTile.x == move.start.x && selectedTile.y == move.start.y &&
                    move.end.x == x && move.end.y == y
                );
            });
        }
        return standard;
    }

    const tileSelected = (x, y) => {
        return (
            selectedTile && (
                (selectedTile.x == x && selectedTile.y == y) ||
                (jumpTrack && jumpTrack[jumpTrack.length - 1].x == x && jumpTrack[jumpTrack.length - 1].y == y)
            )
        );
    }

    const validStart = (x, y) => {
        let valid = false;
        if(props.gameData.legalMoves && !selectedTile) {
            props.gameData.legalMoves.forEach((move) => {
                if(move.type == 'standard') {
                    valid = valid || (
                        move.start.x == x && move.start.y == y
                    );
                } else if(move.type == 'jump') {
                    valid = valid || (
                        move.currentPosition.x == x & move.currentPosition.y == y
                    );
                }
            });
        }
        return valid;
    }

    const tileSelectable = (x, y) => {
        return validStart(x, y) || validJump(x, y) || standardMove(x, y);
    }

    const tileChanged = (x, y) => {
        let changed = false;
        changedTiles.forEach((tile) => {
            changed = changed || (tile.x == x && tile.y == y);
        });
        return changed;
    }

    const tileClass = (x, y) => {
        let className = '';
        if(tileSelected(x, y)) {
            className = 'selected-tile';
        } else if(tileSelectable(x, y)) {
            className = 'selectable-tile';
        } else if(tileChanged(x, y)) {
            className = 'changed-tile';
        } else if((x + y) % 2 == 0) {
            className = 'light-tile';
        } else {
            className = 'dark-tile';
        }
        return className;
    }

    const clickTile = (x, y) => {
        if(validStart(x, y)) {
            setSelectedTile({x: x, y: y});
        } else if(tileSelected(x, y)) {
            setSelectedTile(null);
            if(jumpTrack) {
                setJumpTrack(null);
                if(inverted) {
                    setRenderBoard(invertBoard(props.gameData.snapshot.board));
                } else {
                    setRenderBoard(deepBoardCopy(props.gameData.snapshot.board));
                }
            }
        } else if(standardMove(x, y)) {
            props.sendMove({
                start: {x: selectedTile.x, y: selectedTile.y},
                path: [{x: x, y: y}]
            })
        } else if(validJump(x, y)) {
            jumpPiece(x, y);
        }
    }

    const pieceCharacter = (piece) => {
        let character = ' ';
        if(piece.type != 'null') {
            character = '\u2B24'
        }
        return character;
    }

    const kingLabel = (piece) => {
        if(piece.type == 'king') {
            let color = '';
            switch(piece.player) {
                case 'white':
                    color = 'black';
                    break;
                case 'black':
                    color = 'white';
                    break;
            };
            return (
                <div className={`grid-content ${color}-text king-label`}>
                    K
                </div>
            );
        }
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

    if(renderBoard) {
        return (
            <div className="grid-container">
                <div className="grid-container">
                    {renderBoard.map((row, y) => {
                        if(inverted) {
                            y = renderBoard.length - 1 - y;
                        }
                        return (
                            row.map((piece, x) => {
                                if(inverted) {
                                    x = renderBoard[y].length - 1 - x;
                                }
                                return (
                                    <button 
                                        className={`grid-tile-${row.length} ${tileClass(x, y)}`}
                                        onClick={() => clickTile(x, y)}
                                        key={x}
                                    >
                                        <div className={`grid-content ${piece.player}-text`}>
                                            {pieceCharacter(piece)}
                                        </div>
                                        {kingLabel(piece)}
                                    </button>
                                );
                            })
                        );
                            
                    })}
                </div>
                <button 
                    onClick={toggleInversion}
                    className="inversion-button"
                >
                    Invert Board
                </button>
            </div>
        );
    }
    return <div/>
}

export default CheckersGame;