from django.urls import path

import app.views.users as users
from app.views.games import GamesView
from app.views.game import GameView

app_name = '/app'

urlpatterns = [
    path('register', users.register),
    path('login', users.login_user),
    path('logout', users.logout_user),
    path('check_login', users.check_login),
    path('games/<str:game_type>', GamesView.as_view()),
    path('game/<int:id>', GameView.as_view()),
]