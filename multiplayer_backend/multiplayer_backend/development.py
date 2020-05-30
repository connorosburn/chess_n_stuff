from multiplayer_backend.settings import *

CORS_ORIGIN_ALLOW_ALL = True

DATABASES = {
    'default': {

        'ENGINE': 'django.db.backends.postgresql_psycopg2',
        'NAME': 'backend_dev'
    }
}

SECRET_KEY = '!3z&z749o@83%15r8_4-)cb=v9^jy)k^h0t2)z8bujmdkydre6'

DEBUG = True

ALLOWED_HOSTS = []