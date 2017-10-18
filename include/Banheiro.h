#ifndef BANHEIRO_H_
#define BANHEIRO_H_

#include <mutex>

#define MAX_UTILIZACAO 50
#define CAPACIDADE_BANHEIRO 3
#define UTILIZACAO_CONSEC 3

int utilizacaoGeral = 0 ;

using namespace std;

class Banheiro() {
public:
	static const int CAPACIDADE;
	//static atomic<int> numPassageiros;
	//static std::atomic_flag lock;
	//static bool voltaAcabou;

	void EsperaEncher();

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
