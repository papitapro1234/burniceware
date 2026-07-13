//Cypher
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "functions.h"

using namespace std::chrono_literals;

/*
Este codigo puede sacar medio de un onda a un par, pero tratare de explicar lo
mas escencial.

Efectivamente, cada que ciframos un archivo lo hacemos con una llave totalmente diferente
esto con el fin de que no sea la misma llave para cada archivo, porque creanme
que adivinar una sola llave fija seria mucho mas facil, entonces decidi mejor crear
una para cada archivo. Pero pos por como funciona el codigo, muchas veces archivos
diferentes pueden usar la misma llave, entonces tampoco es que sea lo mejor.

Lo otro es que si se fijan puse un pequeño delay de 10 milisegundos, la razon de esto
es que mientras probaba me di cuenta que generaba las llaves tan rapido, que algunas hasta
se bugeaban, entonces decidi que haya un delay entre que se obtiene la semilla y entre
que se usa, para asi evitar ese bug (Realmente no me acuerdo porque se bugeaba, pero pos
el chiste es que asi funciona, asi lo dejo).

Lo de crear un vector con un buffer y luego sumarle 16, es lo que mas dolores de cabeza
me dio, esto debido a que habia ignorado ese pequeño detalle, y habia archivos que no se
encriptaban bien, estuve dias buscando el porque pasaba esto, e investigando me di cuenta
que era porque aes256cbc, encriptaba el contenido que le pasaba, pero tambien le añadia
un padding (relleno), esto del padding es simplemente por temas de como funciona aes,
pero el chiste es que como le puse un tamaño exacto me estaba desbordando el buffer
del vector, y por eso daba error al encriptar algunos archivos, entonces al ponerle 16 bytes
extras, en caso de que incluyera un padding no me desbordaria la memoria. Si se preguntan
el ¿Porque especificament 16 bytes ?, esto es porque ese es el tamaño del padding que deja
aes256, igualmente mas abajo del codigo le cambiamos el tamaño al buffer, para que luego
no escribamos en el archivo bytes que nada que ver (Si, eso mismo me paso).
*/

std::vector<unsigned char>aes256_cypher(std::string bytes_a_encriptar, long long buffer){
    uint64_t seed = get_time_to_rng();
    std::this_thread::sleep_for(10ms);
    std::string llave_base = generate_pseudorandom_string(32 , seed);
    std::string iv = generate_pseudorandom_string(16 , seed); 
    std::vector<unsigned char> cypher_text_(buffer + 16);

    int len = 0;
    int cypher_text_len = 0;

    /*
    El EVP_CIPHER_CTX_new(), es simplemente un contexto modificable, esto quiere decir
    que nosotros podemos cambiar ese contexto segun lo que queramos, en este caso
    el contexto de mi encriptacion, es que quiero usar AES256_CBC, pero tambien
    puedes crear un contexto en el que uses RSA o CHACHA20, entre otros algoritmos
    de cifrado.
    */
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx){
        return {};
    };


    //--------------------------------------------
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(llave_base.data()), reinterpret_cast<const unsigned char *>(iv.data())) != 1){
        return {};
    };
    
    if (EVP_EncryptUpdate(ctx, cypher_text_.data(), &len, reinterpret_cast<const unsigned char*>(bytes_a_encriptar.c_str()), bytes_a_encriptar.length()) != 1){
        return {};
    };

    cypher_text_len = len;

    if (EVP_EncryptFinal_ex(ctx, cypher_text_.data() + len, &len) != 1){
        return {};
    };
    cypher_text_len += len;
    EVP_CIPHER_CTX_free(ctx);

    //Esto es simplemente el proceso de encriptacion
    //--------------------------------------------------------
    /*
    Aca le cambiamos el tamaño al buffer, para evitar devolver un vector con 
    bytes que nada que ver, lo que hacemos es verificar si al archivo se le agrego
    padding, y si se le agrego pos devolvera completo el buffer que creamos arriba, y
    si no, pos simplemente reducira el tamaño del buffer a devolver.
    */

    cypher_text_.resize(cypher_text_len);
    return cypher_text_;

}
