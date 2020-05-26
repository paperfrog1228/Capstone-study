from pymodm import fields, MongoModel
from app.main import flask_bcrypt

import datetime
import jwt
from app.main.model.blacklist import BlacklistToken
from ..config import key


class User(MongoModel):
    userId = fields.IntegerField(required=True)
    userName = fields.CharField(required=True)
    email = fields.EmailField()
    dateRegistered = fields.DateTimeField()
    favorites = fields.ListField(field=fields.CharField())
    admin = fields.BooleanField(default=False)
    # lines = fields.EmbeddedDocumentListField(line)    # Embedded 이렇게 하면 될듯
    userPassword_hash = fields.CharField()

    @property
    def userPassword(self):
        raise AttributeError('userPassword: write-only field')

    @userPassword.setter
    def userPassword(self, userPassword):
        self.userPassword_hash = flask_bcrypt.generate_password_hash(userPassword).decode('utf-8')  # hashing userPassword

    def check_userPassword(self, userPassword):
        return flask_bcrypt.check_password_hash(self.userPassword_hash, userPassword)

    def encode_auth_token(self, user_id):
        """
        Generates the Auth Token
        :return: string
        """
        try:
            payload = {
                'exp': datetime.datetime.utcnow() + datetime.timedelta(days=1, seconds=5),
                'iat': datetime.datetime.utcnow(),
                'sub': user_id
            }
            return jwt.encode(
                payload,
                key,
                algorithm='HS256'
            )
        except Exception as e:
            return e
        
    @staticmethod
    def decode_auth_token(auth_token):
        """
        Decodes the auth token
        :param auth_token:
        :return: integer|string
        """
        try:
            payload = jwt.decode(auth_token, key)
            is_blacklisted_token = BlacklistToken.check_blacklist(auth_token)
            if is_blacklisted_token:
                return 'Token blacklisted. Please log in again.'
            else:
                return payload['sub']
        except jwt.ExpiredSignatureError:
            return 'Signature expired. Please log in again.'
        except jwt.InvalidTokenError:
            return 'Invalid token. Please log in again.'

    def __repr__(self):
        return "<user '{}'".format(self.userName)

    class Meta:
        collection_name = 'users'   # 지정 안해주면 User collection을 따로 만들어버림
        final = True                # _cls 필드 저장 안하도록 설정