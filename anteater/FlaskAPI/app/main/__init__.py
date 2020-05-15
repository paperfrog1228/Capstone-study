from flask import Flask
from flask_pymongo import PyMongo
from flask_bcrypt import Bcrypt     # 암호화 관련 모듈

from .config import config_by_name

mongo = PyMongo()
flask_bcrypt = Bcrypt()

def create_app(config_name):
    app = Flask(__name__)
    app.config.from_object(config_by_name[config_name])

    mongo.init_app(app)    # 알아서 app.config['MONGO_URI']를 찾아서 사용.

    flask_bcrypt.init_app(app)

    return app