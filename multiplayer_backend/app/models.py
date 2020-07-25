from django.db import models
from django.contrib.auth.models import User

class Game(models.Model):
    white = models.ForeignKey(User, on_delete=models.CASCADE, related_name='started_games')
    black = models.ForeignKey(User, on_delete=models.CASCADE, related_name='joined_games')
    game_type = models.CharField(max_length=20)
    

class Turn(models.Model):
    game = models.ForeignKey(Game, on_delete=models.CASCADE, related_name='turns')
    time = models.DateTimeField(auto_now=True)
    color_moved = models.CharField(max_length=6)
    snapshot = models.TextField()