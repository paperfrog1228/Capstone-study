from flask_testing import TestCase
from manage import app
from pymodm import connect
from pymodm import connection

class BaseTestCase(TestCase):
    """ Base Tests """

    def create_app(self):
        app.config.from_object('app.main.config.TestingConfig')
        connect(app.config['MONGO_URI'])
        return app

    def tearDown(self):
        connection._get_db()['users'].delete_many({})