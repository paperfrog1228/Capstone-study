from flask import Flask
from flask import request

app = Flask(__name__)

@app.route("/")
def index():
    return "Index page"

@app.route("/user", methods=["GET", "POST"])
def hello():
    if request.method == "GET":
        return "Hello, World!"
    else:
        return "POST!"
