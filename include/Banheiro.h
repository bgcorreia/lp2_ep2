#ifndef BANHEIRO_H_
#define BANHEIRO_H_

#include <mutex>

#define MAX_UTILIZACAO 50 // Nº MAXIMO DE VOLTAS

// TODOS EM SEGUNDOS
#define TEMPO_DELAY 2 // DELAY P/ ALGUNS LUGARES

#define TEMPO_PASSEIO 2// TEMPO DE PASSEIO DA PESSOA
#define TEMPO_VOLTA 4 // TEMPO DE VOLTA DO CARRO

#define RANDOM 0 // TEMPO RANDOMICO
#define MAX_TEMPO 5 // RAMDOMICO ATE N

#define TEMPO_ENCHER_ESVAZIAR 3 // TEMPO PARA ENCHER E ESVAZIAR

using namespace std;

void delay(int);

class Banheiro;

class Carro {
public:
	static const int CAPACIDADE;
	//static atomic<int> numPassageiros;
	//static std::atomic_flag lock;
	//static bool voltaAcabou;

	Carro(Parque *);
	virtual ~Carro();
	void EsperaEncher();
	void DaUmaVolta();
	void EsperaEsvaziar();
	int GetNVoltas();
	void run();

	void operator()()
	{
		run();
	}

private:
	int voltas;
	Parque *parque;

};

#endif /* CARRO_H_ */
