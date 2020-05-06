import rsa

pubkey, privatekey = rsa.newkeys(2048)

with open('public.pem', 'wb') as pubpem, open('private.pem', 'wb') as privatepem:
    pubpem.write(pubkey.save_pkcs1())
    privatepem.write(privatekey.save_pkcs1())
