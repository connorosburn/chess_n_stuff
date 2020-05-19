from django.urls import path

import app.views.users as users

app_name = '/app'

urlpatterns = [
    path('register', users.register),
]