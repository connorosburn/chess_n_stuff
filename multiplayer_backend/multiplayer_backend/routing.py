from django.conf.urls import url

from channels.routing import ProtocolTypeRouter
from channels.auth import AuthMiddlewareStack

from app.consumer import LiveUpdate
from channels.sessions import SessionMiddlewareStack

application = ProtocolTypeRouter({
    'websocket': AuthMiddlewareStack(LiveUpdate)
})