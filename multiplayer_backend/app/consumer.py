from asgiref.sync import async_to_sync
from channels.generic.websocket import JsonWebsocketConsumer

class LiveUpdate(JsonWebsocketConsumer):
    def connect(self):
        self.accept()

    def receive_move(self, event):
        print(event)
        self.send_json({'move': event['move'], 'player': event['player']})

    def receive_json(self, content):
        print(content)
        if 'gameID' in content:
            print('gotit')
            async_to_sync(self.channel_layer.group_add)(str(content['gameID']), self.channel_name)
