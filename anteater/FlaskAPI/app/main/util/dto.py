from flask_restplus import Namespace, fields

class UserDto:
    api = Namespace('user', description='user related operations')
    user = api.model('user', {
        'userName': fields.String(required=True, description='user name'),
        'email': fields.String(required=True, description='user email address'),
        'userPassword': fields.String(required=True, description="user password"),
    })