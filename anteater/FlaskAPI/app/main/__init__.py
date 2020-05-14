from flask import Flask
## import Module-For-MongoDB
from flask_bcrypt import Bcrypt     # 암호화 관련 모듈

from .config import config_by_name

flask_bcrypt = Bcrypt()

def create_app(config_name):
    app = Flask(__name__)
    app.config.from_object(config_by_name[config_name])
    # DB 연결 실행 코드 위치
    flask_bcrypt.init_app(app)

    return app