import os

basedir = os.path.abspath(os.path.dirname(__file__))


class Config:
    SECRET_KEY = os.getenv('SECRET_KEY')    # KEY (아직까진 어디에 쓰는건지 파악 못함)
    DEBUG = False                           # 누가봐도 디버그


class DevelopmentConfig(Config):            # 개발버전 설정
    DEBUG = True
    ## DB 정보들 ##
    # SQLALCHEMY_DATABASE_URI = 'sqlite:///' + os.path.join(basedir, 'flask_boilerplate_main.db')
	# SQLALCHEMY_TRACK_MODIFICATIONS = False

class TestingConfig(Config):                # 테스팅 설정
    DEBUG = True
    TESTING = True
    PRESERVE_CONTEXT_ON_EXCEPTION = False   # 예외 발생 시 컨텍스트 보존 여부
    ## DB 정보들 ##

class ProductionConfig(Config):             # 배포판 설정
    DEBUG = False


config_by_name = dict(
    dev=DevelopmentConfig,
    test=TestingConfig,
    prod=ProductionConfig
)

key = Config.SECRET_KEY