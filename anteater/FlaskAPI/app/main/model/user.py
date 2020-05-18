from pymodm import fields, MongoModel
from .. import mongo, flask_bcrypt
from itertools import imap

class User(Model):
    __modelnname__ = "user"

    collection = mongo.db['users']

    userName = fields.CharField(required=true)
    email = fields.EmailField()
    dateRegistered = fields.DateTimeField()
    favorites = fields.ListField(field=fields.CharField)
    # lines = fields.EmbeddedDocumentListField(line)    # Embedded 이렇게 하면 될듯
    userPassword_hash = fields.CharField()

    @property
    def userPassword(self):
        raise AttributeError('userPassword: write-only field')

    @userPassword.setter
    def userPassword(self, userPassword):
        self.userPassword_hash = flask_bcrypt.generate_userPassword_hash(userPassword).decode('utf-8')  # hashing userPassword

    def check_userPassword(self, userPassword):
        return flask_bcrypt.check_userPassword_hash(self.userPassword_hash, userPassword)

    def __repr__(self):
        ]return "<user '{}'".format(self.userName)