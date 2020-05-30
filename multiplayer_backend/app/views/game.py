from django.http import JsonResponse
from django.views import View
import app.cpp_wrapper.cpp_chess as cpp_chess
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
            if 'pieceType' not in move:
                new_board = cpp_chess.move(last_turn.snapshot, move['start']['x'], move['start']['y'], move['end']['x'], move['end']['y'])
            else:
                new_board = cpp_chess.promote_pawn(last_turn.snapshot, move['start']['x'], move['start']['y'], move['end']['x'], move['end']['y'], move['pieceType'])
            if new_board != '':
                channel_layer = get_channel_layer()
                async_to_sync(channel_layer.group_send)(str(game.id), {"type": 'receive_move', "move": move})
                new_turn = Turn(game=game, color_moved=color, snapshot=new_board)
                new_turn.save()
                return JsonResponse({'success': True})