#!/usr/bin/python3

from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256
from base64 import b64decode

# Assuming you have the public key, message, and signature
public_key = RSA.import_key(open('c0.pem').read())
signature = b64decode('your_signature_here')  # Decode the signature if it's base64
message = b'original_message_here'

# Hash the message
message_hash = SHA256.new(message)

# Verify the signature using pkcs1_15 or PSS
try:
    pkcs1_15.new(public_key).verify(message_hash, signature)
    print("The signature is valid.")
except (ValueError, TypeError):
    print("The signature is invalid.")


