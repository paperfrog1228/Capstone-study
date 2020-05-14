## Entry Point

import os
import unittest

from flask_migrate import Migrate, MigrateCommand   # Flask-Migrate is an extension that handles SQLAlchemy database migrations for Flask applications using Alembic.
                                                    # 지금 단계에서는 필요 없음.
from flask_script import Manager                    # The Flask-Script extension provides support for writing external scripts in Flask.

from app.main import create_app     #, db

app = create_app(os.getenv('BOILERPLATE_ENV') or 'dev')

app.app_context().push()

manager = Manager(app)

# migrate = Migrate(app, db) # db is dumb yet.

manager.add_command('db', MigrateCommand)

@manager.command
def run():
    app.run()

@manager.command
def test():
    """Runs the unit tests."""
    tests = unittest.TestLoader().discover('app/test', pattern='test*.py')  # app/test에서 test*.py 파일을 찾아서 로드
    result = unittest.TextTestRunner(verbosity=2).run(tests)
    if result.wasSuccessful():
        return 0
    return 1

if __name__ == '__main__':
    manager.run()