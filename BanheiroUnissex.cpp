/*
 * BanheiroUnissex.cpp
 *
 *  Created on: 16 de out de 2017
 *      Author(s): - Bruno Correia
 *		   - Ícaro Targino
 *		   - Jose Olívio
 */

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#include "include/Colors.h" // FUNCOES PARA FORMATAR TEXTO

#define NUM_PESSOAS 10
#define MAX_UTILIZACAO 20
#define CAPACIDADE_BANHEIRO 3
#define UTILIZACAO_CONSEC 3
#define TEMPO_MINIMO_WC 10
#define TEMPO_MAXIMO_WC 20
#define TEMPO_MINIMO_MOSCANDO 10
#define TEMPO_MAXIMO_MOSCANDO 20

using namespace std;

// nr - passagem de bastao
int homensNoBanheiro = 0; // Nº de homens dentro do WC
int mulheresNoBanheiro = 0; // Nº de mulheres dentro do WC

// dr - passagem de bastao
int homensDormindo = 0; // Nº de homens na fila
int mulheresDormindo = 0; // Nº de mulheres na fila

//
int homensConsecutivos = 0; // Nº de homens que entraram seguidamente no WC
int mulheresConsecutivos = 0; // Nº de mulheres que entraram seguidamente no WC

int utilizacaoGeral = 0;

mutex entrada;
mutex homem;
mutex mulher;
mutex mensagem;

void delay(int sec) {
    chrono::seconds time(sec); // Define tempo em segundos
    this_thread::sleep_for(time); // Faz o delay
}

void delayRandomWC(){
	delay( rand() % TEMPO_MAXIMO_WC + TEMPO_MINIMO_WC );
}

void delayRandomMoscando(){
	delay( rand() % TEMPO_MAXIMO_MOSCANDO + TEMPO_MINIMO_MOSCANDO );
}

class Pessoa {
public:
	
	int getId(){ return this->id; }
	char getSexo(){ return this->sexo; }

	Pessoa(int id, char sexo){
		this->id = id;
		this->sexo = sexo;
	}

	void run(){
		entraNoBanheiro();
	}

	void entraNoBanheiro(){

		mensagem.lock();
		//cout << endl << BOLD("CHEGOU PESSOA Nº: [") << BOLD(FGRN(<< this->id <<)) << BOLD("] - [") << ((this->sexo=='M')?BOLD(BBLU("HOMEM")):BOLD(BRED("MULHER"))) << BOLD("]") << endl;
		cout << endl << BOLD("CHEGOU PESSOA Nº: [") << BOLD(FGRN(<< this->id <<)) << BOLD("] - ") << ((this->sexo=='M')?BBLU("[HOMEM]"):BMAG("[MULHER]")) << endl;
		mensagem.unlock();

	    if (this->sexo == 'M') { // SEXO MASCULINO

	        while (true){

	        	bool liberouHomemAntes = false;

	            entrada.lock(); // Método P(e); -- A primeira thread a executar o método é a primeira a sair.
	       
	            mensagem.lock();
	            cout << endl <<  BOLD("---- " << FCYN("LIBEROU ENTRADA PRINCIPAL")) << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            if ( utilizacaoGeral >= MAX_UTILIZACAO ) { // Verifica se chegou no limite de utilizações.
	                // destroy thread
	                //mensagem.lock();
	                mensagem.lock();
	            	cout << endl << "Banheiro chegou em sua utilização máxima." << endl;
	            	mensagem.unlock();
	            	
	            	break;
	            	//this->detach();
	            	//mensagem.unlock();

	            } 
	            else if ( (mulheresNoBanheiro > 0) || ((homensNoBanheiro == CAPACIDADE_BANHEIRO) && (homensConsecutivos >= UTILIZACAO_CONSEC))) { // Se a condição for verdadeira, o homem entra na fila.
	                
	                homensDormindo++; // Incremento do contador de homens na fila.

	                mensagem.lock();
	            	//cout << endl << "Entrei na fila - [" << this->id << "] | Sexo [" << this->sexo << "]." << endl;
	            	cout << endl << BOLD("---- " << FYEL("ENTREI NA FILA")) << BOLD(" - ") << BBLU("[HOMEM]") << BOLD(" - PESSOA Nº: [" FGRN(<< this->id <<) BOLD("]")) << endl;
	            	//cout << "Homens no banheiro: " << homensNoBanheiro << " | Mulheres no banheiro: " << mulheresNoBanheiro << endl;
	            	cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            	<< "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            	cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            	mensagem.unlock();

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	                homem.lock(); // "Dorme" na fila até ser liberado.
	            }

	            homensNoBanheiro++; // Homem entra no WC

	            mulheresConsecutivos = 0; // Zera mulheres no WC.

	            homensConsecutivos++; // Incrementa nº de homens no WC

	            utilizacaoGeral++; // Incrementa o nº de utilizações

	            // SIGNAL DE ENTRADA ( HOMEM ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if (homensNoBanheiro < CAPACIDADE_BANHEIRO){ // Verifica se cabe outro homem.

	                if (homensConsecutivos < UTILIZACAO_CONSEC && homensDormindo > 0 ){ // Verifica se tem homens na fila e se ainda não chegou no limite de utilização.

	                    homensDormindo--; // Decrementa contador de homens na fila.
	                    homem.unlock(); // Libera um homem dormindo na fila a entrar no banheiro.

	                    liberouHomemAntes = true;

	                } else {

	                    entrada.unlock(); // Método V(e); -- Libera a fila do P(e);

	                } 

	            } else {

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	            }

	            /*
	            homensNoBanheiro++; // Homem entra no WC

	            mulheresConsecutivos = 0; // Zera mulheres no WC.

	            homensConsecutivos++; // Incrementa nº de homens no WC

	            utilizacaoGeral++; // Incrementa o nº de utilizações
	            */

	            // SEÇÃO CRÍTICA
	            mensagem.lock();
	            //cout << "---- ENTREI NO WC -" << utilizacaoGeral << "- Pessoa: [" << this->id << "] - Sexo: " << this->sexo << " (TEM DE SER MASCULINO)" << endl; 
	            cout << endl << BOLD("---- ") << BOLD(FGRN("ENTREI NO BANHEIRO")) << BOLD(" - ") << BBLU("[HOMEM]") << BOLD(" - PESSOA Nº: [") << BOLD(FGRN(<< this->id <<)) BOLD("]") << BOLD(" | UTILIZAÇÕES: [") BOLD(FGRN(<< utilizacaoGeral <<)) BOLD("]") << endl;
	            //cout << "Homens no banheiro: " << homensNoBanheiro << " | Mulheres no banheiro: " << mulheresNoBanheiro << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            delayRandomWC();
	            //utilizaOBanheiro();
	            // FIM DA SEÇÃO CRÍTICA

	            entrada.lock(); // P(e); -- Necessário para garantir atomicidade.

	            //mensagem.lock();
	            //cout << endl << "---DEBUG " << "Homens Consecutivos: " << homensConsecutivos << " | Mulheres na fila: " << mulheresDormindo 
	            //<< " | Homens na fila: " << homensDormindo << " | Homens no Banheiro: " << homensNoBanheiro << endl;
	            //mensagem.unlock();

	            homensNoBanheiro--; // Decrementa o contador de homens no banheiro.

	            // SIGNAL DE SAÍDA ( HOMEM ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            //if ( (homensConsecutivos >= UTILIZACAO_CONSEC) && (mulheresDormindo > 0) && (homensNoBanheiro == 0) ){ // Condição para liberar uma mulher da fila.
	            if ( (mulheresDormindo > 0) && (homensNoBanheiro == 0) ){ // Condição para liberar uma mulher da fila.
	                mulheresDormindo--; // Decrementa contador de mulheres na fila.
	                mulher.unlock(); // Libera uma mulher da fila.
	            //} else if ( !liberouHomemAntes && ((homensConsecutivos < UTILIZACAO_CONSEC && homensDormindo > 0 && mulheresDormindo == 0) || (homensConsecutivos > UTILIZACAO_CONSEC && mulheresDormindo == 0))){ // Condição para liberar um homem da fila.
				//} else if ( (homensDormindo > 0) && (mulheresDormindo == 0) ){ // Condição para liberar um homem da fila.	           
				} else if ( !liberouHomemAntes && ( (homensDormindo > 0) && (mulheresDormindo == 0) )){ // Condição para liberar um homem da fila.	           	
	            //else if ( homensDormindo > 0 || (homensConsecutivos > UTILIZACAO_CONSEC && mulheresDormindo == 0)){ // Condição para liberar um homem da fila.	
	                homensDormindo--; // Decrementa contador de homens na fila
	                homem.unlock(); // Libera um homem da fila.
	            } else {
	                entrada.unlock(); // Método V(e);
	            }

		        // SEÇÃO NÃO CRÍTICA
		        mensagem.lock();
		        //cout << "---- SAI DO WC -" << utilizacaoGeral << "- Pessoa: [" << this->id << "] - Sexo: " << this->sexo << " (TEM DE SER MASCULINO)" << endl; 
		        cout << endl << BOLD("---- ") << BOLD(FRED("SAI DO BANHEIRO")) << BOLD(" - ") << BBLU("[HOMEM]") << BOLD(" - PESSSOA Nº: [") BOLD(FGRN(<< this->id <<)) BOLD("]") << endl;
		        //cout << "Homens no banheiro: " << homensNoBanheiro << " | Mulheres no banheiro: " << mulheresNoBanheiro << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

		        delayRandomMoscando();
	            //ficaMoscando();
	            //cout << "---- SEÇÃO NÃO CRÍTICA | Pessoa: " << (this->id)+1 << " - Sexo: " << this->sexo << " (TEM DE SER MASCULINO)" << endl; 
	            //delay(5);

	        }

	    } else if (this->sexo == 'F') { // SEXO FEMININO

	        while (true){

	        	bool liberouMulherAntes = false;

	            entrada.lock(); // Método P(e); -- A primeira thread a executar o método é a primeira a sair.
	            
	            mensagem.lock();
	            //cout << endl <<  "Homens no banheiro: " << homensNoBanheiro << " | Mulheres no banheiro: " << mulheresNoBanheiro << endl;
	            cout << endl <<  BOLD("---- " << FCYN("LIBEROU ENTRADA PRINCIPAL")) << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            if ( utilizacaoGeral >= MAX_UTILIZACAO ){ // Verifica se chegou no limite de utilizações.
	                // destroy thread
	                mensagem.lock();
					cout << endl << BOLD(BLNK("BANHEIRO CHEGOU A SUA UTILIZAÇÃO MÁXIMA.")) << endl;
					mensagem.unlock();
					
					//this->detach();
					break;

	            }
	            else if ( (homensNoBanheiro > 0) || ((mulheresNoBanheiro == CAPACIDADE_BANHEIRO) && (mulheresConsecutivos >= UTILIZACAO_CONSEC))){ // Se a condição for verdadeira, a mulher entra na fila.

	            	mulheresDormindo++; // Incremento do contador de mulheres na fila.

	            	mensagem.lock();
	            	//cout << endl << "Entrei na fila - [" << this->id << "] | Sexo [" << this->sexo << "]." << endl;
	            	//cout << "---- ENTREI NA FILA - [" << this->id << "] | Sexo [" << this->sexo << "]." << endl;
	            	cout << endl << BOLD("---- " << FYEL("ENTREI NA FILA")) << BOLD(" - ") << BMAG("[MULHER]") << BOLD(" - PESSOA Nº: [" FGRN(<< this->id <<) BOLD("]")) << endl;
	            	//
	            	cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	           	 	<< "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            	cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
		            mensagem.unlock();

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	                mulher.lock(); // "Dorme" na fila até ser liberada.
	            }

	            mulheresNoBanheiro++; // Mulher entra no WC

	            homensConsecutivos = 0; // Zera homens no WC

	            mulheresConsecutivos++; // Incrementa nº de mulheres no WC

	            utilizacaoGeral++; // Incrementa o nº de utilizações

	            // SIGNAL DE ENTRADA ( MULHER ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if (mulheresNoBanheiro < CAPACIDADE_BANHEIRO){ // Verifica se cabe outra mulher.
	            
	                if (mulheresConsecutivos < UTILIZACAO_CONSEC && mulheresDormindo > 0 ){ // Verifica se tem mulheres na fila e se ainda não chegou no limite de utilização.
	                
	                    mulheresDormindo--; // Decrementa contador de mulheres na fila.
	                    mulher.unlock(); // Libera uma mulher dormindo na fila a entrar no banheiro.

	                    liberouMulherAntes = true;

	                } else {

	                    entrada.unlock(); // Método V(e); -- Libera a fila do P(e);

	                }

	            } else {

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	            }
	            
	            /*
	           	mulheresNoBanheiro++; // Mulher entra no WC

	            homensConsecutivos = 0; // Zera homens no WC

	            mulheresConsecutivos++; // Incrementa nº de mulheres no WC

	            utilizacaoGeral++; // Incrementa o nº de utilizações
				*/
	            
	            // SEÇÃO CRÍTICA
	            mensagem.lock();
	            //cout << "---- ENTREI NO WC -" << utilizacaoGeral << "- | Pessoa: [" << this->id << "] - Sexo: " << this->sexo << " (TEM DE SER FEMININO)" << endl; 
	            cout << endl << BOLD("---- ") << BOLD(FGRN("ENTREI NO BANHEIRO")) << BOLD(" - ") << BMAG("[MULHER]") << BOLD(" - PESSOA Nº: [") << BOLD(FGRN(<< this->id <<)) BOLD("]") << BOLD(" | UTILIZAÇÕES: [") BOLD(FGRN(<< utilizacaoGeral <<)) BOLD("]") << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            delayRandomWC();
	            //utilizaOBanheiro();
	            // FIM DA SEÇÃO CRÍTICA

	            entrada.lock(); // P(e); -- Necessário para garantir atomicidade.

	            //mensagem.lock();
	            //cout << endl << "---DEBUG " << "Homens Consecutivos: " << homensConsecutivos << " | Mulheres na fila: " << mulheresDormindo 
	            //<< " | Homens na fila: " << homensDormindo << " | Homens no Banheiro: " << homensNoBanheiro << endl;
	            //mensagem.unlock();

	            mulheresNoBanheiro--; // Decrementa o contador de mulheres no banheiro.

	            // SIGNAL DE SAÍDA ( MULHER ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            //if (mulheresConsecutivos >= UTILIZACAO_CONSEC && homensDormindo > 0 && mulheresNoBanheiro == 0){ // Condição para liberar um homem da fila.
	            if (homensDormindo > 0 && mulheresNoBanheiro == 0){ // Condição para liberar um homem da fila.	
	            
	                homensDormindo--; // Decrementa contador de homens na fila.
	                homem.unlock(); // Libera um homem da fila.
	            
	            //} else if ((mulheresDormindo > 0 && homensDormindo == 0)){ // Condição para liberar uma mulher da fila.
	            } else if ( !liberouMulherAntes && (mulheresDormindo > 0 && homensDormindo == 0)){ // Condição para liberar uma mulher da fila.	
	            
	                mulheresDormindo--; // Decrementa contador de mulheres na fila
	                mulher.unlock(); // Libera a entrada de uma mulher da fila no WC.

	            } else {

	                entrada.unlock(); // Método V(e);

	            }

	            // SEÇÃO NÃO CRÍTICA
	            mensagem.lock();
	            cout << endl << BOLD("---- ") << BOLD(FRED("SAI DO BANHEIRO")) << BOLD(" - ") << BMAG("[MULHER]") << BOLD(" - PESSSOA Nº: [") BOLD(FGRN(<< this->id <<)) BOLD("]") << endl;
	            //
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            delayRandomMoscando();
	            //ficaMoscando();
	            //cout << "---- SEÇÃO NÃO CRÍTICA | Pessoa: " << (this->id)+1 << " - Sexo: " << this->sexo << " (TEM DE SER FEMININO)" << endl; 
	            //delay(5);

	        }

	    } else { // SEXO INDEFINIDO

	        cout << endl << "Sexo indefinido." << endl;

	    }
	}

private:
	int id;
	char sexo;
};

int main() 
{

	// P/ "HABILITAR" ALEATORIEDADE DA FUNCAO RAND
	srand (time(NULL));

	vector<Pessoa*> pessoas; // Vector de ponteiros p/ a Classe Pessoa
	vector<thread> tPessoas; // Vector de Threads

	for ( int i = 1 ; i <= NUM_PESSOAS ; i++ ){
		pessoas.push_back( new Pessoa(i, ((rand() % NUM_PESSOAS + 1) % 2)?'M':'F') ); // Cria os Objetos (as "pessoas") e define id e sexo de cada uma
	}

	homem.lock();
	mulher.lock();

	for (auto &obj : pessoas){
		tPessoas.push_back( thread(&Pessoa::run,obj) ); // Cria uma Thread para cada Objeto e adiciona no Vector de Threads
	}

	for (auto &threads : tPessoas){
	    threads.join(); // Faz o processo principal aguardar todas as Threads concluirem o seu trabalho
	}

	return 0;
}
