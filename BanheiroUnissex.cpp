/*
 * BanheiroUnissex.cpp
 *
 *  Created on: 16 de out de 2017
 *      Author(s): - Bruno Correia
 *		           - Ícaro Targino
 *		           - Jose Olívio
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#include "include/Colors.h" // FUNCOES PARA FORMATAR TEXTO

#define TEMPO_MINIMO_WC 1
#define TEMPO_MAXIMO_WC 20
#define TEMPO_MINIMO_MOSCANDO 5
#define TEMPO_MAXIMO_MOSCANDO 20

using namespace std;

int NUM_PESSOAS, MAX_UTILIZACAO, CAPACIDADE_BANHEIRO, UTILIZACAO_CONSEC;

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

vector<int> pessoasNaFila; // Guardar pessoas na fila

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
		cout << endl << BOLD("CHEGOU PESSOA Nº: [") << BOLD(FGRN(<< this->id <<)) << BOLD("] - ") << ((this->sexo=='M')?BBLU("[HOMEM]"):BMAG("[MULHER]")) << endl;
		mensagem.unlock();

	    if (this->sexo == 'M') { // SEXO MASCULINO

	        while (true){

	        	entrada.lock(); // Método P(e); -- A primeira thread a executar o método é a primeira a sair.
	       
	       		/*
	            mensagem.lock();
	            cout << endl <<  BOLD("---- " << FCYN("LIBEROU ENTRADA PRINCIPAL")) << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();
	            */

	            if ( utilizacaoGeral >= MAX_UTILIZACAO ) { // Verifica se chegou no limite de utilizações.
	                
	            	if ( !( find(pessoasNaFila.begin(), pessoasNaFila.end(), this->id) != pessoasNaFila.end() ) ){
	                    mensagem.lock();
		            	cout << endl << BOLD("BANHEIRO CHEGOU A SUA UTILIZAÇÃO MÁXIMA - INDO EMBORA - PESSOA Nº: [") BOLD(FGRN(<< this->id <<)) BOLD("] - ") << BBLU("[HOMEM]") << endl;
		            	mensagem.unlock();
		            	
		            	entrada.unlock();
		            	break;
		            }
		            
	            } else if ( (mulheresNoBanheiro > 0) || (homensNoBanheiro == CAPACIDADE_BANHEIRO) || ((homensConsecutivos >= UTILIZACAO_CONSEC) && (mulheresDormindo > 0)) ) { // Se a condição for verdadeira, o homem entra na fila.	
	                
	                homensDormindo++; // Incremento do contador de homens na fila.

	                // Adiciona da pessoa do vector de info da fila
	                pessoasNaFila.push_back(this->id);

	                mensagem.lock();
	            	cout << endl << BOLD("---- " << FYEL("ENTREI NA FILA")) << BOLD(" - ") << BBLU("[HOMEM]") << BOLD(" - PESSOA Nº: [" FGRN(<< this->id <<) BOLD("]")) << endl;
	            	cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            	<< "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            	cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            	mensagem.unlock();

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	                homem.lock(); // "Dorme" na fila até ser liberado.
	            }

	            homensNoBanheiro++; // Homem entra no WC

	            // Remove id da pessoa do vector de info da fila
	            pessoasNaFila.erase(remove(pessoasNaFila.begin(), pessoasNaFila.end(), this->id), pessoasNaFila.end());

	            mulheresConsecutivos = 0; // Zera mulheres no WC.

	            homensConsecutivos++; // Incrementa nº de homens no WC

	            utilizacaoGeral++; // Incrementa o nº de utilizações

	            mensagem.lock();	            
	            cout << endl << BOLD("---- ") << BOLD(FGRN("ENTREI NO BANHEIRO")) << BOLD(" - ") << BBLU("[HOMEM]") << BOLD(" - PESSOA Nº: [") << BOLD(FGRN(<< this->id <<)) BOLD("]") << BOLD(" | UTILIZAÇÕES: [") BOLD(FGRN(<< utilizacaoGeral <<)) BOLD("]") << endl;	            
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            // SIGNAL DE ENTRADA ( HOMEM ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if (homensNoBanheiro < CAPACIDADE_BANHEIRO){ // Verifica se cabe outro homem.

	                if (homensConsecutivos < UTILIZACAO_CONSEC && homensDormindo > 0 ){ // Verifica se tem homens na fila e se ainda não chegou no limite de utilização.

	                    homensDormindo--; // Decrementa contador de homens na fila.
	                    homem.unlock(); // Libera um homem dormindo na fila a entrar no banheiro.

	                } else {

	                    entrada.unlock(); // Método V(e); -- Libera a fila do P(e);

	                } 

	            } else {

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	            }

	            // SEÇÃO CRÍTICA
	            delayRandomWC();
	            // FIM DA SEÇÃO CRÍTICA

	            entrada.lock(); // P(e); -- Necessário para garantir atomicidade.

	            homensNoBanheiro--; // Decrementa o contador de homens no banheiro.

	            mensagem.lock();
		        cout << endl << BOLD("---- ") << BOLD(FRED("SAI DO BANHEIRO")) << BOLD(" - ") << BBLU("[HOMEM]") << BOLD(" - PESSSOA Nº: [") BOLD(FGRN(<< this->id <<)) BOLD("]") << endl;
		        cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            // SIGNAL DE SAÍDA ( HOMEM ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if ( (mulheresDormindo > 0) && (homensNoBanheiro == 0) ){ // Condição para liberar uma mulher da fila.
	            
	                mulheresDormindo--; // Decrementa contador de mulheres na fila.
	                mulher.unlock(); // Libera uma mulher da fila.
	            
	            //} else if ( (homensDormindo > 0) && (mulheresDormindo == 0) ){ // Condição para liberar um homem da fila.	   
	            } else if ( (homensDormindo > 0) && (homensConsecutivos < UTILIZACAO_CONSEC) ){ // Condição para liberar um homem da fila.	           	
	            
	                homensDormindo--; // Decrementa contador de homens na fila
	                homem.unlock(); // Libera um homem da fila.
	            
	            } else {
	                entrada.unlock(); // Método V(e);
	            }

		        // SEÇÃO NÃO CRÍTICA
		        if ( !(utilizacaoGeral >= MAX_UTILIZACAO) )
		        	delayRandomMoscando();

	        }

	    } else if (this->sexo == 'F') { // SEXO FEMININO

	        while (true){

	        	entrada.lock(); // Método P(e); -- A primeira thread a executar o método é a primeira a sair.
	            
	            /*
	            mensagem.lock();
	            cout << endl <<  BOLD("---- " << FCYN("LIBEROU ENTRADA PRINCIPAL")) << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();
	            */

	            if ( utilizacaoGeral >= MAX_UTILIZACAO ){ // Verifica se chegou no limite de utilizações.
	            
	            	if ( !( find(pessoasNaFila.begin(), pessoasNaFila.end(), this->id) != pessoasNaFila.end() ) ){
	                    mensagem.lock();
		            	cout << endl << BOLD("BANHEIRO CHEGOU A SUA UTILIZAÇÃO MÁXIMA - INDO EMBORA - PESSOA Nº: [") BOLD(FGRN(<< this->id <<)) BOLD("] - ") << BRED("[MULHER]") << endl;
		            	mensagem.unlock();
		            	
		            	entrada.unlock();
		            	break;
		            }
		        
	            } else if ( (homensNoBanheiro > 0) || (mulheresNoBanheiro == CAPACIDADE_BANHEIRO) || ((mulheresConsecutivos >= UTILIZACAO_CONSEC) && (homensDormindo > 0) ) ) { // Se a condição for verdadeira, a mulher entra na fila.

	            	mulheresDormindo++; // Incremento do contador de mulheres na fila.

	            	// Adiciona da pessoa do vector de info da fila
	                pessoasNaFila.push_back(this->id);

	            	mensagem.lock();
	               	cout << endl << BOLD("---- " << FYEL("ENTREI NA FILA")) << BOLD(" - ") << BMAG("[MULHER]") << BOLD(" - PESSOA Nº: [" FGRN(<< this->id <<) BOLD("]")) << endl;
	               	cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	           	 	<< "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            	cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            	mensagem.unlock();

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	                mulher.lock(); // "Dorme" na fila até ser liberada.
	            }

	            mulheresNoBanheiro++; // Mulher entra no WC

	            // Remove id da pessoa do vector de info da fila
	            pessoasNaFila.erase(remove(pessoasNaFila.begin(), pessoasNaFila.end(), this->id), pessoasNaFila.end());

	            homensConsecutivos = 0; // Zera homens no WC

	            mulheresConsecutivos++; // Incrementa nº de mulheres no WC

	            utilizacaoGeral++; // Incrementa o nº de utilizações

	            mensagem.lock();
	            cout << endl << BOLD("---- ") << BOLD(FGRN("ENTREI NO BANHEIRO")) << BOLD(" - ") << BMAG("[MULHER]") << BOLD(" - PESSOA Nº: [") << BOLD(FGRN(<< this->id <<)) BOLD("]") << BOLD(" | UTILIZAÇÕES: [") BOLD(FGRN(<< utilizacaoGeral <<)) BOLD("]") << endl;
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            // SIGNAL DE ENTRADA ( MULHER ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if (mulheresNoBanheiro < CAPACIDADE_BANHEIRO){ // Verifica se cabe outra mulher.
	            
	                if (mulheresConsecutivos < UTILIZACAO_CONSEC && mulheresDormindo > 0 ){ // Verifica se tem mulheres na fila e se ainda não chegou no limite de utilização.
	                
	                    mulheresDormindo--; // Decrementa contador de mulheres na fila.
	                    mulher.unlock(); // Libera uma mulher dormindo na fila a entrar no banheiro.

	                } else {

	                    entrada.unlock(); // Método V(e); -- Libera a fila do P(e);

	                }

	            } else {

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	            }
	            
	            // SEÇÃO CRÍTICA
	            delayRandomWC();
	            // FIM DA SEÇÃO CRÍTICA

	            entrada.lock(); // P(e); -- Necessário para garantir atomicidade.

	            mulheresNoBanheiro--; // Decrementa o contador de mulheres no banheiro.

	           	mensagem.lock();
	            cout << endl << BOLD("---- ") << BOLD(FRED("SAI DO BANHEIRO")) << BOLD(" - ") << BMAG("[MULHER]") << BOLD(" - PESSSOA Nº: [") BOLD(FGRN(<< this->id <<)) BOLD("]") << endl;
	            //
	            cout << "Homens no banheiro: [" << homensNoBanheiro << "] | Mulheres no banheiro: [" << mulheresNoBanheiro 
	            << "] || Homens na fila: [" << homensDormindo << "] | Mulheres na fila: [" << mulheresDormindo << "]" << endl;
	            cout << "Homens Consecutivos: [" << homensConsecutivos << "] | Mulheres Consecutivas: [" << mulheresConsecutivos << "]" << endl;
	            mensagem.unlock();

	            // SIGNAL DE SAÍDA ( MULHER ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if (homensDormindo > 0 && mulheresNoBanheiro == 0){ // Condição para liberar um homem da fila.	
	            
	                homensDormindo--; // Decrementa contador de homens na fila.
	                homem.unlock(); // Libera um homem da fila.
	            
	            //} else if ( (mulheresDormindo > 0 && homensDormindo == 0)){ // Condição para liberar uma mulher da fila.	
	            } else if ( mulheresDormindo > 0 && (mulheresConsecutivos < UTILIZACAO_CONSEC)){ // Condição para liberar uma mulher da fila.	
	            
	                mulheresDormindo--; // Decrementa contador de mulheres na fila
	                mulher.unlock(); // Libera a entrada de uma mulher da fila no WC.

	            } else {

	                entrada.unlock(); // Método V(e);

	            }

	            // SEÇÃO NÃO CRÍTICA
	            if ( !(utilizacaoGeral >= MAX_UTILIZACAO) )
	            	delayRandomMoscando();

	        }

	    } else { // SEXO INDEFINIDO

	        cout << endl << "Sexo indefinido." << endl;

	    }
	}

private:
	int id;
	char sexo;
};

class Mensagens {
public:

	void opcoesUso(char * comando){
		cerr << endl << "Uso: " << comando << " [OPÇÕES]\n\n\
OPÇÕES:\n\
-p | --pessoas        Número de pessoas\n\
-m | --maxutilizacao  Número máximo de utilizações do Banheiro\n\
-c | --capacidade     Capacidade máxima de pessoas no Banheiro\n\
-u | --consecutivos    Número máximo de utilizações consecutivas por gênero\n\
\n\
OBS.: Todos os parâmetros acima são obrigatórios e podem ser\n\
      chamados em qualquer ordem.\n\
\n\
-v | --version  Versão do programa\n\
-h | --help     Ajuda (esta tela)\n\
\n\
Exemplo de uso:\n\
\n\
  user@host:~dir_projeto$ "<< comando << " -n 10 -m 20 -c 3 -u 3" << endl << endl; 
	}

	void versao(){
		cout << endl << "Versão do programa: " << BOLD("0.1") << endl;
		cout << "Copyright © 2017 - Bruno Correia | Ícaro Targino | José Olívio" << endl << endl;
	}


};

int main(int argc, char* argv[]){

	Mensagens mensagem;

	/* 
	IF Aninhados para captura dos parametros
	Usando if pois o case em c++ não aceita string
	*/

	if (argc == 2){

		if ( !(strcmp(argv[1],"-h")) || !(strcmp(argv[1],"--help")) ) {
			
			mensagem.opcoesUso(argv[0]);
			return 0;

		} else if ( !(strcmp(argv[1],"-v")) || !(strcmp(argv[1],"--version")) ) {
		
			mensagem.versao();
			return 0;

		} else {

			cerr << endl << "Opção inválida, utilize '-h' para ajuda." << endl;
			return 1;

		}
	
	} else if (argc == 9){

		for (int i = 1; i < argc; i++){
		
	        if ( !(strcmp(argv[i],"-p")) || !(strcmp(argv[i],"--pessoas")) ) {
	        		
	        		istringstream iss( argv[++i] );
	        		iss >> NUM_PESSOAS; // CASTING CHAR P/ INT

	        } else if ( !(strcmp(argv[i],"-m")) || !(strcmp(argv[i],"--maxutilizacao")) ) {
	                
	                istringstream iss( argv[++i] );
	        		iss >> MAX_UTILIZACAO; // CASTING CHAR P/ INT

	        } else if ( !(strcmp(argv[i],"-c")) || !(strcmp(argv[i],"--capacidade")) ) {
	                
	                istringstream iss( argv[++i] );
	        		iss >> CAPACIDADE_BANHEIRO; // CASTING CHAR P/ INT

	        } else if ( !(strcmp(argv[i],"-u")) || !(strcmp(argv[i],"--consecutivos")) ) {
	                
	                istringstream iss( argv[++i] );
	        		iss >> UTILIZACAO_CONSEC; // CASTING CHAR P/ INT

	        } else {
	                cerr << endl << "Opção inválida, utilize '-h' para ajuda." << endl;
	                return 1;
	        }

		}

	} else {

		cerr << endl << "Opção inválida, utilize '-h' para ajuda." << endl;
		return 1;
				
	}

	// P/ "HABILITAR" ALEATORIEDADE DA FUNCAO RAND
	srand (time(NULL));

	vector<Pessoa*> pessoas; // Vector de ponteiros p/ a Classe Pessoa
	vector<thread> tPessoas; // Vector de Threads

	for ( int i = 1 ; i <= NUM_PESSOAS ; i++ ){
		pessoas.push_back( new Pessoa(i, ((rand() % NUM_PESSOAS + 1) % 2)?'M':'F') ); // Cria os Objetos (as "pessoas") e define id e sexo de cada uma
	}

	homem.lock();
	mulher.lock();

	system("clear");

	cout << endl << BOLD(BLNK("BANHEIRO ABERTO, PODEM USAR!")) << endl;

	for (auto &obj : pessoas){
		tPessoas.push_back( thread(&Pessoa::run,obj) ); // Cria uma Thread para cada Objeto e adiciona no Vector de Threads
	}

	for (auto &threads : tPessoas){
	    threads.join(); // Faz o processo principal aguardar todas as Threads concluirem o seu trabalho
	}

	return 0;
}
