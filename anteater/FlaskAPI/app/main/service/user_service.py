import datetime

from app.main import mongo
from app.main.model.user import User

collection = mongo.db['users']

def save_new_user(data):
    user = collection.find_one({"userName": data[userName]})
    if not user:    # 그런 사용자 없읍니다.
        new_user = User(
            userName=data['userName'],
            email=data['email'],
            dateRegistered=datetime.datetime.utcnow()
            userPassword=data['userPassword']
        )
        save_changes(new_user)
        response_object = {
            'status': 'success',
            'message': 'Successfully registered.'
        }
        return response_object, 201     # 201 status : Created
    else:
        response_object = {
            'status': 'fail',
            'message': 'User already exists. Please Log in.'
        }
        return response_object, 409     # 409 status : Conflict

def get_all_users():
    return collection.find()

def get_a_user(userName):
    return collection.find_one({'userName': userName})

def save_changes(data):
    collection.insert_one(data)