import requests
import pcid

SERVER_URL = 'http://localhost:5000'

id_ = pcid.pcid()

try:
    response = requests.get(f'{SERVER_URL}/check', params={
        'id': id_
    }).text
    
    if response == 'TRUE':
        print('Program is activated')
    else:
        print('Program is not activated')
except:
    print('Can not check activation status')
