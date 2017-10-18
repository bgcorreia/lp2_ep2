#include <iostream>
#include <string>
#include <thread>
#include "include/Parque.h"
#include "include/Carro.h"
#include "include/Passageiro.h"
#include <mutex>

using namespace std;
mutex sem;

// CLASSE MENSAGEM
class menssagem
{
public:

    void Exibe(int id,string msg) {
    sem.lock();
    cout << Thread: << id   |<< msg << endl;
    sem.unlock();

    }

};

int main() {
	Parque parque;
	Carro carro(&parque);
	Passageiro *passageiros[10]; // Cria um array do tipo Passageiro

    mensagem exibir; // Objeto da classe mensagem

    for (int i = 0; i < 10; i++) { // Enquanto o i for menor que o nº de passageiros, faça
    	passageiros[i] = new Passageiro(i, &carro, &parque);
	/* 	Cria um novo objeto do tipo Passageiro e armazena o endereço no array passageiros[i]
    	então no caso irão ser criados 10 objetos ( 0 a 9 )	*/
    	parque.addPassageiro(passageiros[i]);
	/* Adiciona endereço armazenado em passageiros[i] no vector private na classe Parque */
    }

    thread tCarro(&Carro::run,&carro); // inicia thread carro

    vector<thread> tPassageiros; // Cria um vetor do tipo thread com nome tPassageiros
    //for (auto &p : parque.getPassageiros()) tPassageiros.push_back(thread(&Passageiro::run, p)); // O que faz isso?
    for (auto &p : parque.getPassageiros()){ //for ate tamanho do vector
        tPassageiros.push_back(thread(&Passageiro::run, p)); //inicia a thread do passageiro e joga no final do vector tPassageiros
    }

    tCarro.join(); // Pausa as demais, enquanto tCarro termina

    //cout << endl << "Carro parado, parque fechou! Até amanhã!" << endl;
    exibir.fechamentoParque();

    for (auto &th : tPassageiros){
        th.join();
    }

    for (int i = 0; i < 10; i++) {
    	delete passageiros[i];
    }

	return 0;
}
