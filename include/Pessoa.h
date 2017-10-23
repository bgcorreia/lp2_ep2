#ifndef PESSOA_H
#define PESSOA_H

class Banheiro;

class Pessoa {
public:
	char sexo;
	int id;

	// nr - passagem de bastao
    int homensNoBanheiro = 0; // Nº de homens dentro do WC
    int mulheresNoBanheiro = 0; // Nº de mulheres dentro do WC

    // dr - passagem de bastao
    int homensDormindo = 0; // Nº de homens na fila
    int mulheresDormindo = 0; // Nº de mulheres na fila

    //
    int homensConsecutivos = 0; // Nº de homens que entraram seguidamente no WC
    int mulheresConsecutivos = 0; // Nº de mulheres que entraram seguidamente no WC

    mutex entrada = 1;
    mutex homem = 0;
    mutex mulher = 0;


	Pessoa(int id,sexo s);
	void entraNoBanheiro();

	// void saiDoBanheiro();
	void utilizaOBanheiro();

	void ficaMoscando();
	// bool banheiroFechado();

	void run();

private:

	Banheiro *banheiro;

};

#endif /* PASSAGEIRO_H_ */
