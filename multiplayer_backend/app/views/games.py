from django.http import JsonResponse
from django.views import View
import app.cpp_wrapper.cpp_chess as cpp_chess
from app.models import Game, Turn
from django.contrib.auth.decorators import login_required
from django.utils.decorators import method_decorator
from app.serializers.games import games_serializer
from django.contrib.auth.models import User
import json

@method_decorator(login_required, name='dispatch')
class GamesView(View):
    def post(self, request):
        body = json.loads(request.body.decode('utf-8'))
        opponent = User.objects.filter(username=body['opponent'])
        new_board = ''
        new_board = cpp_chess.move(cpp_chess.new_game(), body['move']['start']['x'], body['move']['start']['y'], body['move']['end']['x'], body['move']['end']['y'])

        if(new_board == ''):
            return JsonResponse({'message': 'nope'}, status=400)
        else:
            game = Game.objects.create(white=request.user, black=opponent.first())
            turn = Turn.objects.create(game=game, color_moved='white', snapshot=new_board)
            return JsonResponse({'id': game.id})
    
    def get(self, request):
        response = {
            'started': games_serializer(request.user.started_games.all()),
            'joined': games_serializer(request.user.joined_games.all())
        }
        return JsonResponse(response)

