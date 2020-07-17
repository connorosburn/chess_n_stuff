const pieceCharacter = (type) => {
    let character = '';
    switch(type) {
        case 'rook':
            character = '\u265C\uFE0E';
            break;
        case 'knight':
            character = '\u265E\uFE0E';
            break;
        case 'bishop':
            character = '\u265D\uFE0E';
            break;
        case 'queen':
            character = '\u265B\uFE0E';
            break;
        case 'king':
            character = '\u265A\uFE0E';
            break;
        case 'pawn':
            character = '\u265F\uFE0E'; 
            break;
        default: 
            character = ' ';
    }
    return character;
}

export default pieceCharacter;