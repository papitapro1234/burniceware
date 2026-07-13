
![Wallpaper mamalon](https://github.com/papitapro1234/burniceware/blob/main/Imagenes%20y%20GIFs/wallpaper_to_my_ransomware_real.png)

# Un ransomware bastante simple con una vulnerabilidad en el algoritmo de generación de llaves, que permite desencriptar los archivos sin tener la llave generada por el ransomware.
### Este ransomware fue hecho con fines únicamente demostrativos, el ransomware no está pensado para realmente ser una amenaza para ninguna institución o dispositivo, el mal uso del código fuente escapa a mis responsabilidades, de igual manera he creado junto con este ransomware una herramienta capaz de desencriptar devuelta los archivos encriptados con este mismo ransomware, así que si alguien resultara infectado por este, solo tendría que descargar la herramienta y ejecutarla y sus archivos volverían a la normalidad.

---


# PREQUISITOS PARA COMPILAR EL RANSOMWARE
### Para compilar este programa, necesitas tener las cabeceras de [`OPENSSL FULL`](https://slproweb.com/products/Win32OpenSSL.html), y usar [`TDM GCC`](https://jmeubank.github.io/tdm-gcc/download/) que es el compilador que yo use. Una vez tengan esas dos cosas instaladas, pueden proseguir con la respectiva compilacion del proyecto
---
# COMPILAR ARCHIVO
### Para compilar el ransomware, lo que tienen que hacer es descargar [**`cmake`**](https://cmake.org/), y una vez lo instalen tienen que modificar el archivo `CmakeLists.txt`, y poner en los puntos que estan comentados en el archivo, la ruta donde tienen instalado las librerias de `OPENSSL`, y una vez modifiquen eso ya pueden ejecutar los dos siguientes comandos en su terminal.

### `PD: Creo que esta demas decir que tienen que ejecutar esos comandos en la carpeta del proyecto.`

```bash
cmake -G "MinGW Makefiles" .
cmake --build .
```
--- 

# Demostracion del ransomware y de la herramienta de desencriptacion

![Demostracion del ransomware](https://github.com/papitapro1234/burniceware/blob/main/Imagenes%20y%20GIFs/demostracion%20ransomware.gif)

![Demostracion de la herramienta de desencriptacion](https://github.com/papitapro1234/burniceware/blob/main/Imagenes%20y%20GIFs/demostracion%20herramienta%20de%20descifrado.gif)


# [`TODO ESTE PROYECTO LO EXPLIQUE Y USE EN MI ARTICULO DE MEDIUM, SI LES INTERESA DENLE CLIC A ESTE HIPERVINCULO`](https://medium.com/@jesus_papita_55717/como-una-mala-implementaci%C3%B3n-de-cifrado-puede-salvar-tus-archivos-cifrados-con-un-ransomware-cf6613232efd)
