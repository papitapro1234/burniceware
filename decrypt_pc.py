#Decrypt all pc
import os
from pathlib import Path
import ctypes
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from lcgalgorithm import LinearCongruencialGenerator , CHARS

def decypher_file(path_file:str , key:str):
	try:
		"""
		Leemos el archivo con permisos de lectura y escritura "rb+", asi evitamos
		abrir dos with y nos ahorramos lineas de codigo, luego guardamos el contenido
		en una variable, para luego compararlo con 100mb, en mi articulo explico claramente
		el porque este script no puede descifrar archivos tan grandes, y porque es necesario
		usar el otro script hecho por su servidor, para desencriptar archivos muy grandes.

		Luego simplemente desencriptamos el contenido y le quitamos el padding (Si es que tiene)
		para luego sobreescribir el archivo, y renombrarlo sin la extension ".burnice".

		PD: Smile si lees esto, tuve un problema con poner el codigo como dijiste
		asi que decidi dejarlo como lo tenia antes XD.
		"""
		with open(path_file , "rb+") as fltu:
			cypher_content = fltu.read()
			buffer = 100 * 1024 * 1024
			if min(buffer, len(cypher_content)) == buffer:
				raise ValueError(f"El archivo {path_file} es muy pesado, se recomienda fuerza bruta")
			print(f"Archivo a desencriptar: {path_file}")
			print(f"Llave del archivo: {key}")
			cipher = AES.new(key.encode("utf-8"), AES.MODE_CBC , key[:16].encode("utf-8"))
			data_with_padding = cipher.decrypt(cypher_content)
			data_without_padding = unpad(data_with_padding , AES.block_size)
			fltu.seek(0) #Puntero de escritura al inicio del archivo
			fltu.write(data_without_padding)
			print("Archivo desencriptado con exito..")
			return True
	except Exception as err:
		print(str(err))
		return False

"""
sinceramente creo que esta funcion de aca se entiende lo suficiente como para tener
que explicar que hace, pero igual medio la explicare, lo que hace es simplemente leer 
todo un directorio y busca archivos en esos directorios con la extension ".burnice"
esto lo hace de forma recursiva, por ende de existir una carpeta en ese directorio
tambien leera el contenido de esa carpeta.
"""
def read_directorys_recursively(path:str):
	for i in Path(path).rglob('*'):
		"""
		Lo que hace la funcion os.path.join(), es simplemente unir dos directorios
		en este caso el directorio que le pasamos a la funcion, y los archivos o carpetas
		que hay dentro de ese directorio.
		"""
		final_path = os.path.join(path , i)

		if final_path.endswith(".burnice") and os.path.isfile(final_path):
			"""
			obtenemos la fecha de modificacion del archivo y se la pasamos
			al algoritmo para inicializarlo, y luego generamos un string de un largo de
			32 bytes.
			"""

			"""
			Eso satanico que esta en la funcion int(), es basicamente la funcion que me da la fecha de modificacion
			del archivo, y la convierte a unix time para luego pasarla como un entero a la funcion seed.
			"""
			lcg = LinearCongruencialGenerator(seed=int(Path(str(final_path)).stat().st_mtime)) 
			key = ''.join(CHARS[lcg.range_pseudorandom_number(0 , len(CHARS) - 1)] for i in range(32))
			if decypher_file(final_path , key):
				os.rename(final_path , final_path.removesuffix(".burnice"))

SPI_SETDESKWALLPAPER = 20

#Esto simplemente cambia el fondo de pantalla por uno generico
ctypes.windll.user32.SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0,"C:\\Windows\\Web\\Wallpaper\\Theme1\\img2.jpg" , 3)

user_home = os.getenv("USERPROFILE") #esto simplemente obtiene la carpeta raiz

del_dir = os.path.join(user_home , "Desktop\\YOU COMPUTER HAS ENCRYPTED.txt")

#Esto simplemente borra la nota que deja el ransomware en el escritorio
if os.path.exists(del_dir):
	os.remove(del_dir)

dirs= [
	os.path.join(user_home , "Downloads"),
	os.path.join(user_home , "Documents"),
	os.path.join(user_home , "Music"),
	os.path.join(user_home , "Videos"),
	os.path.join(user_home , "Pictures")
]

for d in dirs:
	try:
		read_directorys_recursively(d)
	except Exception:
		pass