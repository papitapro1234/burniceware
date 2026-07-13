#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <cstdint>
#include <vector>
std::string generate_pseudorandom_string(int size_string , long long seed);
long long obtener_fecha_modificacion(const std::string& ruta_archivo);
uint64_t get_time_to_rng();
std::vector<unsigned char>aes256_cypher(std::string bytes_a_encriptar , long long buffer);

#endif