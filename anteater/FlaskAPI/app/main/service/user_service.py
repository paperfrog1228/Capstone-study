import datetime

from app.main import mongo
from app.main.model.user import User

def save_new_user(data):
    collection = mongo.db['users']
    user = collection.find_one({"userName": data['userName']})
    if not user:    # 그런 사용자 없읍니다.
        new_user = User(
            userName=data['userName'],
            email=data['email'],
            dateRegistered=datetime.datetime.utcnow()
        )
        new_user.userPassword = data['userPassword']
        save_changes(new_user.to_son().to_dict())
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
    collection = mongo.db['users']
    return list(collection.find())

def get_a_user(userName):
    collection = mongo.db['users']
    return collection.find_one({'userName': userName})

def save_changes(data):
    collection = mongo.db['users']
    collection.insert_one(data)