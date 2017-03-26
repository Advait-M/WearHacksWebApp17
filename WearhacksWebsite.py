<<<<<<< HEAD
from flask import Flask
from flask import render_template
import pyredb

app = Flask(__name__)


@app.route('/')
def index():
    return render_template("index.html", script = "")


if __name__ == '__main__':
    pyredb.ForgetMeNot().start()
    app.run()
=======
from flask import Flask
from flask import render_template

app = Flask(__name__,)


@app.route('/')
def hello_world():
    return render_template("index.html")


if __name__ == '__main__':
    app.run()
>>>>>>> origin/master
