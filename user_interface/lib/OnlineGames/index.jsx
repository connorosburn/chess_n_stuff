import React, {useState, useEffect} from 'react';
import LiveGameListener from '../request/socket.js';
import {getGames} from '../request/fetch';

function OnlineGames(props) {
    const[gameData, setGameData] = useState({});
    useEffect(() => {
        const fetchGames = async () => {
            let response = await getGames();
            let data = await response.json();
            setGameData(data);
        }
        fetchGames();
    }, []);

    const startGame = (id, opponent, color) => {
        props.setUpGame({
            mode: 'online',
            player: color,
            opponent: opponent,
            gameID: id,
            listener: new LiveGameListener(id)
        });
    }

    const gameList = (data, color) => {
        if(data) {
            return (
                <div>
                    {data.map((entry, i) => {
                        let opponent = '';
                        if(color == 'white') {
                            opponent = entry.black;
                        } else if(color == 'black') {
                            opponent = entry.white;
                        }
                        return (
                            <button key={i} onClick={() => startGame(entry.id, opponent, color)}>
                                <p>You play {color}</p>
                                <p>Up next: {entry.up_next}</p>
                                <p>Opponent: {opponent}</p>
                            </button>
                        );
                    })}
                </div>
            );
        }
    }
    return ( 
        <div> 
            {gameList(gameData.started, 'white')}
            {gameList(gameData.joined, 'black')}
        </div>
    );
}

export default OnlineGames;