from flask import Flask, jsonify, abort, request
from flask_cors import CORS
import pymysql
import json

app = Flask(__name__)
CORS(app,supports_credentials=True)
class MyDB:
    def __init__(self, h='127.0.0.1', u='123', p="123", db=None):
        self.db = pymysql.connect(host=h, user=u, password=p, database=db,
                                  cursorclass=pymysql.cursors.DictCursor)
        self.cursor = self.db.cursor()

    def select(self):
        self.cursor.execute("select * from attrs order by id desc limit 1")
        self.db.commit()
        return self.cursor.fetchall()

@app.route('/sensorData', methods=['GET'])
def get_result():
    result = my.select()
    print(result)
    return jsonify(result)

if __name__ == '__main__':
    my = MyDB(db='sensor')
    app.run(host='0.0.0.0')



