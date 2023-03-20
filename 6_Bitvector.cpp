#include <iostream>
#include "Bitvector.h"
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>

Bitvector playerStatus = Bitvector(20);;

const char* translateBool(bool b){
	return b ? "SI" : "NO";
}

void parseStatusBits(){
	//interpretar cada bit

	std::cout << "Tiene pocion de agua? " << translateBool(playerStatus[0]) << "\n";
	std::cout << "Tiene pocion de fuego? " << translateBool(playerStatus[1]) << "\n";
	std::cout << "Tiene pocion de aire? " << translateBool(playerStatus[2]) << "\n";
	std::cout << "Tiene pocion de tierra? " << translateBool(playerStatus[3]) << "\n";
	std::cout << "Tiene espada de piedra? " << translateBool(playerStatus[6]) << "\n";
	std::cout << "Tiene arco encantado? " << translateBool(playerStatus[7]) << "\n";
	std::cout << "Tiene cuchillo? " << translateBool(playerStatus[8]) << "\n";
	std::cout << "Tiene un martillo rojo? " << translateBool(playerStatus[13]) << "\n";
	std::cout << "Tiene una rifle? " << translateBool(playerStatus[14]) << "\n";
	std::cout << "Tiene el libro encantado? " << translateBool(playerStatus[15]) << "\n";
	std::cout << "Se completo el primer nivel? " << translateBool(playerStatus[16]) << "\n";
	std::cout << "Se completo el segundo nivel? " << translateBool(playerStatus[17]) << "\n";
	std::cout << "Se hablo con el gran hechicero? " << translateBool(playerStatus[18]) << "\n";
	std::cout << "Se completo la ultima mision? " << translateBool(playerStatus[19]) << "\n";
	std::cout << "Boss defeated? " << translateBool(playerStatus[20]) << "\n";
}

//Leer archivo "de juego"
void readSaveFile(){
	//primero, leer 20 bits de estado
	/*	12 bits son el inventario :
		6 pociones y 6 armas
		bit 13 : modo oscuro
		bit 14 : status del personaje(gigante o normal)
		bit 15 : status del personaje(enraged o normal)
		bit 16 : status del personaje(flotando o en piso)
		bit 17 : ha vencido nivel 1
		bit 18 : ha vencido nivel 2
		bit 19 : ha vencido nivel 3
		bit 20 : ha vencido el jefe final*/
		//siguiente: entero de 32 bits que contiene el tamano de pantalla en X
		//siguiente: entero de 32 bits con el tamano de pantalla en Y 
	std::ifstream savefile;
	savefile.open("/otro/save/datamine", std::ifstream::binary);
	if (savefile.is_open()){
		//determinar tamano del archivo
		savefile.seekg(0, savefile.end);
		int filesize = savefile.tellg();
		std::cout << "file size: " << filesize << "\n";
		if (filesize > 0){
			savefile.seekg(0, savefile.beg);
			//buffer de 3 bytes para almacenar temporalmente los 20 bits
			unsigned char* buffer = new unsigned char[3];
			//limpiar buffer
			memset(buffer, 0, 3);
			savefile.read((char*)buffer, 3);

			//imprimir valores en binario (para debug)
			for (int i = 0; i < 3; i++){
				std::cout << "savefile: 0b";
				for (int j = 7; j >= 0; j--){
					uint32 b = (buffer[i] >> j) & 1;
					std::cout << (b ? "1" : "0");
				}
				std::cout << std::endl;
			}

			//convertir el buffer a bitvector
			uint32 data = 0;
			data = ((uint32)buffer[0] << 24);
			data |= ((uint32)buffer[1] << 16);
			data |= ((uint32)buffer[2] << 8);
			playerStatus.setbank(data, 0);
			playerStatus.binaryprint();
			//invertir el orden de los bits
			Bitvector temp = Bitvector(playerStatus);
			temp.binaryprint("temp");
			for (int i = 31; i >= 0; i--){
				bool b = playerStatus.getval(i);
				std::cout << "val " << (b ? "1" : "0") << "\n";
				playerStatus.setval(temp.getval(i), 31 - i);
			}
			playerStatus.binaryprint("playerstatus");

			//interpretar los bits segun el protocolo disenado
			parseStatusBits();

			//interpretar res X
			//savefile.seekg(4);
			int* resx = new int;
			*resx = 9999;
			std::cout << "pos = " << savefile.tellg() << "\n";
			savefile.read((char*)resx, 4);
			std::cout << "resx = " << *resx << "\n";
			std::cout << "pos = " << savefile.tellg() << "\n";

			//interpretar res Y
			int* resy = new int;
			savefile.read((char*)resy, 4);
			std::cout << "resy = " << *resy << "\n";

			savefile.close();
			//agregar codigo para que el programa pida al usuario
			//resolucion en X, Y y se guarde en el archivo
			//offset de resx = 3, offset de resy = 7

		}
	}
	else puts("Your game data save file is corrupted. :(");
	return;
}

int main(){
	//Bitvector *misbits = new Bitvector(65);
	Bitvector misbits(65);
	std::cout << "se pidieron 65 bits, se otorgan " << misbits.getsize() << "\n";


	misbits.setval(1, 0);
	misbits.setval(1, 8);
	misbits.setval(1, 16);
	/*
	misbits.setval(1, 3);
	//misbits->binaryprint();
	misbits.setval(1, 15);
	misbits.setval(0, 63);
	misbits.setval(1, 64);
	misbits.binaryprint();
	printf("valor de 63: %x \n", misbits[63] );
	*/
	misbits.binaryprint();
	//Guardar a memoria persistente
	std::fstream savefile;
	savefile.open("data.bin", std::fstream::out |
		std::fstream::binary);
	if (savefile.is_open()){
		savefile.write((const char*)misbits.getArray(), 4 * 3);
		char* buffer = new char[1];
		*buffer = 0;
		//savefile.write( buffer , 1);

		savefile.close();
	}
	else{
		std::cout << "savefile not found\n";
	}

	// cambiar resolución del programa
	DEVMODE devmode;
	devmode.dmPelsWidth = 1920;
	devmode.dmPelsHeight = 1080;
	devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	devmode.dmSize = sizeof(DEVMODE);

	long result = ChangeDisplaySettings(&devmode, 0);

	//prueba de lectura de archivo binario
	readSaveFile();

	system("pause");
	return 0;
}
