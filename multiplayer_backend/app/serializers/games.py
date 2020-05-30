def games_serializer(games):
    serialized_games = []
    for game in games:
        serialized_games.append(serialize_game(game))
    return serialized_games

def serialize_game(game):
    last_moved = game.turns.last().color_moved
    up_next = ''
    if last_moved == 'white':
        up_next = 'black'
    elif last_moved == 'black':
        up_next = 'white'
    return {
        'id': game.id,
        'white': game.white.username,
        'black': game.black.username,
        'up_next': up_next
    }
