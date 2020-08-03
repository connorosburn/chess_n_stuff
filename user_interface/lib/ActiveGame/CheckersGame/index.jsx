import React, {useState} from 'react';
import pieceCharacter from '../ChessGame/pieceCharacter';

function CheckersGame(props) {
    const [inverted, setInverted] = useState(false);
    const tileClass = (x, y) => {
        let finalClass = '';
        if((x + y) % 2 == 0) {
            finalClass = 'light-tile';
        } else {
            finalClass = 'dark-tile';
        }
        return finalClass;
    }

    const clickTile = () => {

    }

    const pieceCharacter = () => {

    }

    if(props.gameData) {
        const board = props.gameData.snapshot.board;
        return (
            <div className="chess-board">
                {board.map((row, y) => {
                    if(inverted) {
                        y = board.length - 1 - y;
                    }
                    return (
                        row.map((piece, x) => {
                            if(inverted) {
                                x = board[y].length - 1 - x;
                            }
                            return (
                                <button 
                                    className={`chess-tile ${tileClass(x, y)}`}
                                    onClick={() => clickTile({x: x, y: y})}
                                    key={x}
                                >
                                    <div className={`checkers-piece checkers-${piece.player}`}>
                                        {pieceCharacter(x, y)}
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

export default CheckersGame;