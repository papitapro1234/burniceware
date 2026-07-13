#datetime
from pathlib import Path
import string
import os
import mmap
import string
import argparse
from typing import Dict, List, Optional
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

from lcgalgorithm import LinearCongruencialGenerator , CHARS

LISTA_MAGIC_NUMBERS = [
    {"nombre": "JPG/JPEG", "bytes": b"\xFF\xD8\xFF"},
    {"nombre": "PNG",      "bytes": b"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"},
    {"nombre": "WEBP",     "bytes": b"RIFF"},  
    {"nombre": "SVG",      "bytes": b"<?xml"}, 
    {"nombre": "RAW",      "bytes": b"II\x2a\x00"},
    {"nombre": "DOCX/XLSX/PPTX/ZIP", "bytes": b"\x50\x4B\x03\x04"},
    {"nombre": "PDF",      "bytes": b"\x25\x50\x44\x46"},
    {"nombre": "PPT",      "bytes": b"\xD0\xCF\x11\xE0\xA1\xB1\x1A\xE1"},
    {"nombre": "ODT",      "bytes": b"\x50\x4B\x03\x04"},
    {"nombre": "RTF",      "bytes": b"{\\rtf"},
    {"nombre": "MP4",      "bytes": b"\x00\x00\x00\x18ftyp"},
    {"nombre": "MP3",      "bytes": b"ID3"},
    {"nombre": "AVI",      "bytes": b"RIFF"},
    {"nombre": "MOV",      "bytes": b"\x00\x00\x00\x14ftypqt"},
    {"nombre": "WAV",      "bytes": b"RIFF"},
    {"nombre": "MKV",      "bytes": b"\x1A\x45\xDF\xA3"},
    {"nombre": "RAR",      "bytes": b"\x52\x61\x72\x21\x1A\x07"}, 
    {"nombre": "MDB",      "bytes": b"\x00\x01\x00\x00Standard Jet DB"},
    {"nombre": "ACCDB",    "bytes": b"\x00\x01\x00\x00Standard ACE DB"},
    {"nombre": "SQLITE/DB","bytes": b"SQLite format 3\x00"},
    {"nombre": "BIN",      "bytes": b"\x7F\x45\x4C\x46"},
]

def verify_readable_file(bytes_to_verify: bytes) -> bool:
    print(str(bytes_to_verify[:10]))
    caracteres_validos = set(string.printable.encode('ascii'))
    no_imprimibles = sum(1 for byte in bytes_to_verify[:10] if byte not in caracteres_validos)
    porcentaje_basura = no_imprimibles / 5
    return porcentaje_basura < 0.15
    

def magic_numbers_verify(bytes_to_magic_number: bytes) -> bool:
    max_bytes = max(len(formato["bytes"]) for formato in LISTA_MAGIC_NUMBERS)
    bytes_to_read = bytes_to_magic_number[:max_bytes]
    for formato in LISTA_MAGIC_NUMBERS:
        if bytes_to_read.startswith(formato["byte"]):
            return True
    return False

def read_and_overwrite_files(file_to_use:str , key:str) -> bool:
    with open(file_to_use , 'rb+') as fltu:
        with mmap.mmap(fltu.fileno() , length=0 , access=mmap.ACCESS_WRITE) as mm:
            buffer = min(300*1024*1024 , mm.size())
            cypher_file = mm[0:buffer]
            cipher = AES.new(key.encode("utf-8"), AES.MODE_CBC, key[:16].encode("utf-8"))
            try:    
                data_with_padding = cipher.decrypt(cypher_file)
                if (not verify_readable_file(data_with_padding)):
                    if (not magic_numbers_verify(data_with_padding)):
                        raise ValueError("")
                mm[0:buffer] = data_with_padding
                mm.flush()
                print("Se ha descifrado el archivo...")
                return True
            except (ValueError , KeyError):
                print("Esa llave no es la correcta")
                return False

def decypher_file(files_path:str , timestamp:int):
    print(f"probando el timestamp: {timestamp}")
    if os.path.isfile(files_path) and files_path.endswith(".burnice"):   
        lcg = LinearCongruencialGenerator(seed = timestamp)
        key = ''.join(CHARS[lcg.range_pseudorandom_number(0 , len(CHARS) - 1)] for i in range(32))
        print(f"llave de {files_path} : {key}")
        if read_and_overwrite_files(files_path , key):
            if str(args.file).endswith(".burnice"):
                os.rename(files_path , str(files_path).removesuffix(".burnice"))
                os._exit(1)
    else:
        print("Este archivo no ha sido encriptado con el ransomware burnice")
            
parser = argparse.ArgumentParser()

parser.add_argument("-f", "--file", default="", help="Ruta del archivo grande para hacer fuerza bruta" , type=str)
parser.add_argument("-n" , "--number" , default=20 , help="Cantidad de segundos para hacer fuerza bruta" , type=int)

args = parser.parse_args()

if str(args.file) == "":
    print("No ha ingresado ningun archivo, porfavor ingrese uno")
    os._exit(1)
timestamp = Path(str(args.file)).stat().st_mtime

timestamp_to_bruteforce = []

for i in range(int(args.number)):
    timestamp_to_bruteforce.append(int(str(timestamp).split(".")[0]) - i)

for a in timestamp_to_bruteforce:
    decypher_file(str(args.file) , a)
    