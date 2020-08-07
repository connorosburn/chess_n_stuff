from django.http import JsonResponse
from django.views import View
import app.cpp_wrapper.cpp_games as cpp_games
from app.models import Game, Turn
from django.contrib.auth.decorators import login_required
from django.utils.decorators import method_decorator
from app.serializers.game import game_serializer
from django.contrib.auth.models import User
from asgiref.sync import async_to_sync
from channels.layers import get_channel_layer
import json

@method_decorator(login_required, name='dispatch')
class GameView(View):
    def get(self, request, **kwargs):
        print('sfsdfdsf')
        game = Game.objects.get(id=kwargs['id'])
        if game.white == request.user or game.black == request.user:
            return JsonResponse(game_serializer(game))
    
    def patch(self, request, **kwargs):
        game = Game.objects.get(id=kwargs['id'])
        last_turn = game.turns.last()
        body = json.loads(request.body.decode('utf-8'))
        if(request.user.id == game.white.id and last_turn.color_moved == 'black'):
            return self._save_move('white', game, body['move'])
        elif(request.user.id == game.black.id and last_turn.color_moved == 'white'):
            return self._save_move('black', game, body['move'])

    def _save_move(self, color, game, move):
            last_turn = game.turns.last()
            new_board = ''
            new_board = cpp_games.from_snapshot(game.game_type, move, last_turn.snapshot)
            if new_board != '':
                channel_layer = get_channel_layer()
                async_to_sync(channel_layer.group_send)(str(game.id), {"type": 'receive_move', "move": move, "player": color})
                new_turn = Turn(game=game, color_moved=color, snapshot=new_board)
                new_turn.save()
                return JsonResponse({'success': True})