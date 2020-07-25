from django.http import JsonResponse
from django.views import View
import app.cpp_wrapper.cpp_games as cpp_games
from app.models import Game, Turn
from django.contrib.auth.decorators import login_required
from django.utils.decorators import method_decorator
from app.serializers.games import games_serializer
from django.contrib.auth.models import User
import json

@method_decorator(login_required, name='dispatch')
class GamesView(View):
    def post(self, request, **kwargs):
        body = json.loads(request.body.decode('utf-8'))
        opponent = User.objects.filter(username=body['opponent']).first()
        new_board = cpp_games.new_game(kwargs['game_type'], body['move'])

        if(new_board == ''):
            return JsonResponse({'message': 'nope'}, status=400)
        else:
            game = Game.objects.create(game_type=kwargs['game_type'], white=request.user, black=opponent)
            turn = Turn.objects.create(game=game, color_moved='white', snapshot=new_board)
            return JsonResponse({'id': game.id})
    
    def get(self, request, **kwargs):
        response = {
            'started': games_serializer(request.user.started_games.filter(game_type=kwargs['game_type']).all()),
            'joined': games_serializer(request.user.joined_games.filter(game_type=kwargs['game_type']).all())
        }
        return JsonResponse(response)

