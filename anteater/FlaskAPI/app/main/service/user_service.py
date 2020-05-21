import datetime

from app.main.model.user import User
from pymodm.errors import DoesNotExist
from pymongo.errors import DuplicateKeyError

def save_new_user(data):
    new_user = User(
        userName=data['userName'],
        email=data['email'],
        dateRegistered=datetime.datetime.utcnow()
    )
    new_user.userPassword = data['userPassword']    
    try:
        new_user.save()
    except DuplicateKeyError:
        response_object = {
            'status': 'fail',
            'message': 'User already exists. Please Log in.'
        }
        return response_object, 409     # 409 status : Conflict
    else:
        response_object = {
            'status': 'success',
            'message': 'Successfully registered.'
        }
        return response_object, 201     # 201 status : Created

def get_all_users():
    print(User.objects.all())
    return list(User.objects.all())

def get_a_user(userName):
    try:
        return User.objects.get({'userName': userName})
    except DoesNotExist:
        return None