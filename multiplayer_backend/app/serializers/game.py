def game_serializer(game):
    return {
        'id': game.id,
        'white': game.white.username,
        'black': game.black.username,
        'snapshot': game.turns.last().snapshot
    }