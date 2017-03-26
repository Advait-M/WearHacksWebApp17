from flask import Flask, request
from flask import render_template
import pyredb
app = Flask(__name__)


@app.route('/')
def index():
    return render_template("index.html")

@app.route('/submit', methods=['POST'])
def submit_textarea():
    print("hereee")
    pyredb.ForgetMeNot().editScript(request.form['styled-textarea'])
    return index()
if __name__ == '__main__':
    pyredb.ForgetMeNot().start()
    app.run()
