import base64
import rsa
import requests
import pcid

SERVER_URL = 'http://localhost:5000'

PRIVATEKEY_PATH = 'private.pem'

with open(PRIVATEKEY_PATH, 'rb') as keyfile:
    privatekey = rsa.PrivateKey.load_pkcs1(keyfile.read())

id_ = pcid.pcid()

signature = rsa.sign(id_.encode('utf-8'), privatekey, 'SHA-256')

try:
    response = requests.post(f'{SERVER_URL}/register', json={
        'id': id_,
        'signature': base64.b64encode(signature).decode('utf-8')
    }).text

    if response == 'OK':
        print('Successfully activated')
    else:
        print('An error occurred during activation')
except:
    print('Can not establish connection with activation server')
