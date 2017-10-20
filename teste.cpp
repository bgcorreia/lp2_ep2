#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#define NUM_PESSOAS 10
#define MAX_UTILIZACAO 30
#define CAPACIDADE_BANHEIRO 3
#define UTILIZACAO_CONSEC 3
#define TEMPO 2

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

//mutex entrada = 1;
mutex entrada;
//entrada = 1;
mutex homem;
mutex mulher;
mutex mensagem;
//mutex mensagem;

/*
mutex entrada = 1;
mutex homem = 0;
mutex mulher = 0;
*/

void delay(int sec) {
    chrono::seconds time(sec); // Define tempo em segundos
    this_thread::sleep_for(time); // Faz o delay
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
		cout << endl << "Pessoa Nº:" << this->id << " | Sexo:" << this->sexo << endl;
	}

	void entraNoBanheiro(){

		mensagem.lock();
		cout << endl << "Thread Nº: " << this->id << " | Sexo: " << this->sexo << endl;
		mensagem.unlock();

	    if (this->sexo == 'M') { // SEXO MASCULINO

	        while (true){

	            entrada.lock(); // Método P(e); -- A primeira thread a executar o método é a primeira a sair.
	            mensagem.lock();
	            cout << endl <<  "Homens no banheiro: " << homensNoBanheiro << " | Mulheres no banheiro: " << mulheresNoBanheiro << endl;
	            mensagem.unlock();

	            if ( utilizacaoGeral == MAX_UTILIZACAO ) { // Verifica se chegou no limite de utilizações.
	                // destroy thread
	                //mensagem.lock();
	                mensagem.lock();
	            	cout << endl << "Banheiro chegou em sua utilização máxima." << endl;
	            	mensagem.unlock();
	            	break;
	            	//mensagem.unlock();

	            } else if ( mulheresNoBanheiro > 0 || homensNoBanheiro == CAPACIDADE_BANHEIRO ) { // Se a condição for verdadeira, o homem entra na fila.
	                
	                mensagem.lock();
	            	cout << endl << "Entrei na fila - [" << this->id << "] | Sexo [" << this->sexo << "]." << endl;
	            	mensagem.unlock();

	                homensDormindo++; // Incremento do contador de homens na fila.
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

	                } else {

	                    entrada.unlock(); // Método V(e); -- Libera a fila do P(e);

	                } 

	            } else {

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	            }

	            // SEÇÃO CRÍTICA
	            mensagem.lock();
	            cout << "---- ENTREI NO WC -" << utilizacaoGeral << "- Pessoa: [" << this->id << "] - Sexo: " << this->sexo << " (TEM DE SER MASCULINO)" << endl; 
	            mensagem.unlock();
	            delay(TEMPO);
	            //utilizaOBanheiro();
	            // FIM DA SEÇÃO CRÍTICA

	            entrada.lock(); // P(e); -- Necessário para garantir atomicidade.

	            homensNoBanheiro--; // Decrementa o contador de homens no banheiro.

	            // SIGNAL DE SAÍDA ( HOMEM ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if (homensConsecutivos >= UTILIZACAO_CONSEC && mulheresDormindo > 0 && homensNoBanheiro == 0){ // Condição para liberar uma mulher da fila.
	                mulheresDormindo--; // Decrementa contador de mulheres na fila.
	                mulher.unlock(); // Libera uma mulher da fila.
	            }
	            else if ((homensConsecutivos < UTILIZACAO_CONSEC && homensDormindo > 0 && mulheresDormindo == 0) || (homensConsecutivos >= UTILIZACAO_CONSEC && mulheresDormindo == 0)){ // Condição para liberar um homem da fila.
	                homensDormindo--; // Decrementa contador de homens na fila
	                homem.unlock(); // Libera um homem da fila.
	            } else {
	                entrada.unlock(); // Método V(e);
	            }

		        // SEÇÃO NÃO CRÍTICA
		        mensagem.lock();
		        cout << "---- SAI DO WC -" << utilizacaoGeral << "- Pessoa: [" << this->id << "] - Sexo: " << this->sexo << " (TEM DE SER MASCULINO)" << endl; 
		        mensagem.unlock();
		        delay(5);
	            //ficaMoscando();
	            //cout << "---- SEÇÃO NÃO CRÍTICA | Pessoa: " << (this->id)+1 << " - Sexo: " << this->sexo << " (TEM DE SER MASCULINO)" << endl; 
	            //delay(5);

	        }

	    } else if (this->sexo == 'F') { // SEXO FEMININO

	        while (true){

	            entrada.lock(); // Método P(e); -- A primeira thread a executar o método é a primeira a sair.
	            mensagem.lock();
	            cout << endl <<  "Homens no banheiro: " << homensNoBanheiro << " | Mulheres no banheiro: " << mulheresNoBanheiro << endl;
	            mensagem.unlock();

	            if ( utilizacaoGeral == MAX_UTILIZACAO ){ // Verifica se chegou no limite de utilizações.
	                // destroy thread
	                mensagem.lock();
					cout << endl << "Banheiro chegou em sua utilização máxima." << endl;
					mensagem.unlock();
					break;
	            }
	            else if ( homensNoBanheiro > 0 || mulheresNoBanheiro == CAPACIDADE_BANHEIRO ){ // Se a condição for verdadeira, a mulher entra na fila.

	            	mensagem.lock();
	            	cout << endl << "Entrei na fila - [" << this->id << "] | Sexo [" << this->sexo << "]." << endl;
	            	mensagem.unlock();

	                mulheresDormindo++; // Incremento do contador de mulheres na fila.
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

	                } else {

	                    entrada.unlock(); // Método V(e); -- Libera a fila do P(e);

	                }

	            } else {

	                entrada.unlock(); // Método V(e); -- Libera a fila do P(e);
	            }

	            // SEÇÃO CRÍTICA
	            mensagem.lock();
	            cout << "---- ENTREI NO WC -" << utilizacaoGeral << "- | Pessoa: [" << this->id << "] - Sexo: " << this->sexo << " (TEM DE SER FEMININO)" << endl; 
	            mensagem.unlock();
	            delay(TEMPO);
	            //utilizaOBanheiro();
	            // FIM DA SEÇÃO CRÍTICA

	            entrada.lock(); // P(e); -- Necessário para garantir atomicidade.

	            mulheresNoBanheiro--; // Decrementa o contador de mulheres no banheiro.

	            // SIGNAL DE SAÍDA ( MULHER ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
	            if (mulheresConsecutivos >= UTILIZACAO_CONSEC && homensDormindo > 0 && mulheresNoBanheiro == 0){ // Condição para liberar um homem da fila.
	            
	                homensDormindo--; // Decrementa contador de homens na fila.
	                homem.unlock(); // Libera um homem da fila.
	            
	            } else if ((mulheresConsecutivos < UTILIZACAO_CONSEC && mulheresDormindo > 0 && homensDormindo == 0) || (mulheresConsecutivos > UTILIZACAO_CONSEC && homensDormindo == 0)){ // Condição para liberar uma mulher da fila.
	            
	                mulheresDormindo--; // Decrementa contador de mulheres na fila
	                mulher.unlock(); // Libera uma mulher da fila.

	            } else {

	                entrada.unlock(); // Método V(e);

	            }

	            // SEÇÃO NÃO CRÍTICA
	            mensagem.lock();
	            cout << "---- SAI DO WC -" << utilizacaoGeral << "- Pessoa: [" << this->id << "] - Sexo: " << this->sexo << " (TEM DE SER FEMININO)" << endl; 
	            mensagem.unlock();
	            delay(5);
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

	vector<Pessoa*> pessoas; // Vector de ponteiros p/ a Classe Pessoa
	vector<thread> tPessoas; // Vector de Threads

	for ( int i = 0 ; i < NUM_PESSOAS ; i++ ){
		pessoas.push_back( new Pessoa(i, (i%2)?'M':'F') ); // Cria os Objetos (as "pessoas") e define id e sexo de cada uma
	}


		homem.lock();
		mulher.lock();

	for (auto &info : pessoas){
		//tPessoas.push_back( thread(&Pessoa::run,info) ); // Cria uma Thread para cada Objeto e adiciona no Vector de Threads
		tPessoas.push_back( thread(&Pessoa::entraNoBanheiro,info) ); // Cria uma Thread para cada Objeto e adiciona no Vector de Threads
	}

	for (auto &th : tPessoas){
        th.join(); // Faz o processo principal aguardar todas as Threads concluirem o seu trabalho
    }

	return 0;
}
