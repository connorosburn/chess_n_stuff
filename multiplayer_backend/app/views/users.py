from django.http import HttpResponse, JsonResponse
from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.db import IntegrityError
from django.views.decorators.http import require_http_methods
from django.contrib.auth import authenticate
import json

@require_http_methods(["POST"])
def register(request):
    body = json.loads(request.body.decode('utf-8'))
    if registration_valid(body):
        try:
            user = User.objects.create_user(body['username'], None, body['password'])
            login(request, user)
            return JsonResponse({'username': user.get_username(), 'id': user.id})
        except IntegrityError:
            return JsonResponse({'message': f'Username {body["username"]} is already taken.'}, status=400)
    else:
        return JsonResponse(\
            {'message': 'Please fill out all fields and match password with confirmed password'},\
            status=400)

def registration_valid(body):
    return has_field(body, 'username') and\
    has_field(body, 'password') and\
    has_field(body, 'confirm_password') and\
    body['password'] == body['confirm_password']

def has_field(body, field):
    return field in body and type(body[field]) is str and body[field] != ''

@require_http_methods(["POST"])
def login_user(request):
    body = json.loads(request.body.decode('utf-8'))
    user = authenticate(username=body['username'], password=body['password'])
    if user is not None:
        login(request, user)
        return JsonResponse({'username': user.get_username(), 'id': user.id})
    else:
        return JsonResponse({'message': 'Incorrect credentials'}, status=400)
    
@require_http_methods(["DELETE"])
def logout_user(request):
    logout(request)
    return JsonResponse({'logged_out': True})

@require_http_methods(["GET"])
def check_login(request):
    if request.user.is_authenticated:
        return JsonResponse({'username': request.user.get_username(), 'id': request.user.id})
    else:
        return JsonResponse({})