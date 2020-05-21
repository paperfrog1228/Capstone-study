from pymodm import fields, MongoModel
from app.main import flask_bcrypt

class User(MongoModel):
    __modelname__ = "user"

    userName = fields.CharField(required=True)
    email = fields.EmailField()
    dateRegistered = fields.DateTimeField()
    favorites = fields.ListField(field=fields.CharField())
    # lines = fields.EmbeddedDocumentListField(line)    # Embedded 이렇게 하면 될듯
    userPassword_hash = fields.CharField()

    @property
    def userPassword(self):
        raise AttributeError('userPassword: write-only field')

    @userPassword.setter
    def userPassword(self, userPassword):
        self.userPassword_hash = flask_bcrypt.generate_password_hash(userPassword).decode('utf-8')  # hashing userPassword

    def check_userPassword(self, userPassword):
        return flask_bcrypt.check_userPassword_hash(self.userPassword_hash, userPassword)

    def __repr__(self):
        return "<user '{}'".format(self.userName)

    class Meta:
        collection_name = 'users'   # 지정 안해주면 User collection을 따로 만들어버림