import pickle
import os.path as path
import base64
import rsa
import flask
import sqlite3
import functools

def transact(fun):
    @functools.wraps(fun)
    def wrapper():
        conn = sqlite3.connect('ids.db')
        cursor = conn.cursor()
        try:
            ret = fun(cursor=cursor)
            conn.commit()
            return ret
        except Exception as ex:
            conn.rollback()
            raise ex
        finally:
            conn.close()
    
    return wrapper

PUBKEY_PATH = 'public.pem'

with open(PUBKEY_PATH, 'rb') as keyfile:
    pubkey = rsa.PublicKey.load_pkcs1(keyfile.read())

# IDS_PATH = 'ids.pickle'

# ids = set()

# if path.exists(IDS_PATH) and path.isfile(IDS_PATH):
#     with open(IDS_PATH, 'rb') as file:
#         ids = pickle.load(file)

app = flask.Flask(__name__)

@app.route('/check', methods=['GET'])
@transact
def check(cursor):
    id_ = flask.request.args.get('id', None)

    if id_ is None:
        return 'FALSE'
    
    # if id_ not in ids:
    #     return 'FALSE'
    cursor.execute('SELECT id FROM ids WHERE id=?', (id_,))
    row = cursor.fetchone()
    if row is None:
        return 'FALSE'
    
    return 'TRUE'

@app.route('/register', methods=['POST'])
@transact
def register(cursor):
    if not flask.request.is_json:
        return 'FAILED'

    try:
        data = flask.request.json

        id_ = data['id']
        signature = base64.b64decode(data['signature'])

        rsa.verify(id_.encode('utf-8'), signature, pubkey)

        # ids.add(id_)

        # with open(IDS_PATH, 'wb') as file:
        #     pickle.dump(ids, file)
        cursor.execute('INSERT OR IGNORE INTO ids VALUES (?)', (id_,))

        return 'OK'
    except Exception as ex:
        return 'FAILED'


@app.cli.command('init-db')
@transact
def initdb(cursor):
    cursor.execute('''CREATE TABLE IF NOT EXISTS ids (id TEXT PRIMARY KEY)''')
