#include <iostream>
#include <stdio.h>
#include <vector>
#define NON_HA_PARENT -1
using namespace std;

//void mostra_citta(vector<string> vect_citta, int n_nodi);
//void mostra(vector<int> distanze, int n_nodi, int radice, vector<int> parent, vector<string> vect_citta);

void algoritmo(vector<vector<int>> graph, int radice, int n_nodi, vector<string> vect_citta);
void mostra_path(int vertice_corrente, vector<int> parent, vector<string>vect_citta);
int controllo_dei_nodi_vicini(vector<int> distanze, vector<bool> sono_stati_visitati, int n_nodi);
int prendi_source(int& n_nodi, vector<string>& vect_citta);
bool controllo_presenza(string& input);
vector<vector<int>> costruisci_graph(int n_nodi, vector<string> vect_citta);
vector<string> crea_vect_citta(int& n_nodi);
vector<vector<int>> costruzione_graph_classe(int n_nodi, vector<string> vect_citta);

class db_le_citta
{
public:
	vector<vector<int>> distanze_citta ={//0   1   2   3   4   5   6   7   8   9   10  11  12
		                                  {0  ,153,0  ,0  ,263,0  ,312,0  ,0  ,0  ,0  ,0  ,0  },          //bari
									      {153,0  ,582,0  ,413,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  },          //lecce
									      {0  ,582,0  ,211,592,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  },          //catania
									      {0  ,0  ,211,0  ,716,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  },          //palermo
									      {263,413,592,716,0  ,227,247,0  ,0  ,0  ,0  ,0  ,0  },          //napoli
									      {0  ,0  ,0  ,0  ,227,0  ,210,275,0  ,0  ,0  ,513,0  },          //roma
									      {312,0  ,0  ,0  ,247,210,0  ,399,365,0  ,0  ,0  ,0  },          //pescara
									      {0  ,0  ,0  ,0  ,0  ,275,399,0  ,105,0  ,0  ,253,0  },          //firenze
									      {0  ,0  ,0  ,0  ,0  ,0  ,365,105,0  ,154,213,294,0  },          //bologna
									      {0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,154,0  ,278,0  ,0  },          //venezia
									      {0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,213,278,0  ,346,143},          //milano
									      {0  ,0  ,0  ,0  ,0  ,513,0  ,253,294,0  ,346,0  ,169},          //genova
									      {0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,143,169,0  },          //torino
	};
	vector<string> nomi_citta = {"bari","lecce","catania","palermo","napoli","roma","pescara","firenze","bologna","venezia","milano","genova","torino"};
	//aggiungere metodi per aggiunta di citta
};

int main()
{
	int n_nodi = 0;
	cout << "Inserisci il numero di nodi: ";
	cin >> n_nodi;

	vector<string> vect_citta = crea_vect_citta(n_nodi);
	vector<vector<int>> grap = costruisci_graph(n_nodi, vect_citta);
	int source = prendi_source(n_nodi, vect_citta);
	algoritmo(grap, source, n_nodi, vect_citta);
	return 0;
}

void algoritmo(vector<vector<int>> graph, int radice, int n_nodi, vector<string> vect_citta) {

	vector<int> distanze(n_nodi), parent(n_nodi);
	vector<bool> sono_stati_visitati(n_nodi); //flags per i visitati
	//inizializzo tutto
	for (int i = 0; i < n_nodi; i++)
	{
		distanze[i] = INT_MAX;
		sono_stati_visitati[i] = false;
	}

	distanze[radice] = 0;// distanza dalla radice = 0
	parent[radice] = NON_HA_PARENT;

	for (int i = 0; i < n_nodi - 1; i++)
	{
		int nodo_vicino = controllo_dei_nodi_vicini(distanze, sono_stati_visitati, n_nodi);
		//nodi vicini vengono analizzati, int nodo vicino è il nodo preso in considerazione

		sono_stati_visitati[nodo_vicino] = true; //è stato visitato il nodo vicino

		for (int adiacente = 0; adiacente < n_nodi; adiacente++) //fase di update delle distanze dei n_nodi adiacenti
		{
			if (!sono_stati_visitati[adiacente]            //se non è nell'array dei visitati "sono_stati_visitati"
				&& graph[nodo_vicino][adiacente]           //esiste la connessione tra il vicino e l'adiacente
				&& distanze[nodo_vicino] != INT_MAX        //distanza del nodo vicino non è infinita
				&& distanze[nodo_vicino] + graph[nodo_vicino][adiacente] < distanze[adiacente]) //il peso del viaggio dalla source al nodo adiacente è piccolo rispetto alla distanza corrente dell'adiacente
			{
				parent[adiacente] = nodo_vicino; //parent serve per printare il path
				distanze[adiacente] = distanze[nodo_vicino] + graph[nodo_vicino][adiacente];
			}
		}
	}

	//mostriamo
	for (int i = 0; i < n_nodi; i++) {
		if (i != radice)
		{
			cout << endl;
			cout << "Nodo: " << vect_citta[radice] << " -> " << vect_citta[i] << "\tDistanza: " << distanze[i] << "\tPercorso:\t";
			mostra_path(i, parent, vect_citta);
		}
	}
} // non toccare
/*
void mostra(vector<int> distanze, int n_nodi, int radice, vector<int> parent, vector<string> vect_citta)
{
	for (int i = 0; i < n_nodi; i++) {
		if (i != radice)
		{
			cout << endl;
			cout << "Nodo: " << vect_citta[radice] << " -> " << vect_citta[i] << "\tDistanza: " << distanze[i] << "\tPercorso:\t";
			mostra_path(i, parent, vect_citta);
		}
	}
}
*/
void mostra_path(int vertice_corrente, vector<int> parent, vector<string>vect_citta) {
	if (vertice_corrente == NON_HA_PARENT) {
		return;
	}
	mostra_path(parent[vertice_corrente], parent, vect_citta);
	cout << vect_citta[vertice_corrente] << " ";
}
/*
void mostra_citta(vector<string> vect_citta, int n_nodi) {
	cout << "Ecco le citta ordinate per numero: " << endl;
	for (int k = 0; k < n_nodi; k++)
	{
		cout << k << ") " << vect_citta[k] << endl;
	}
}
*/
int controllo_dei_nodi_vicini(vector<int> distanze, vector<bool> sono_stati_visitati, int n_nodi) {
	int valoremin = INT_MAX;
	int nodominimo = 0;
	for (int i = 0; i < n_nodi; i++)
	{
		if (!sono_stati_visitati[i] && distanze[i] <= valoremin) //viene preso e aggiunto il nodo con la 
			//minima distanza che non è nel vect bool dei visitati
		{
			valoremin = distanze[i];
			nodominimo = i;
		}
	}
	return nodominimo; //viene dato all'algoritmo
}

vector<vector<int>> costruisci_graph(int n_nodi, vector<string> vect_citta) {
	
	int risp;
	cout << "Vuoi costruire con distanze a tuo piacimento (seleziona 1) oppure gia determinate (seleziona 0) ?" << endl;
	cin >> risp;
	

	if (risp == 1)
	{
		vector<vector<int>> graph;
		cout << "Costruiamo il graph con le citta e le loro distanze." << endl;
		int x;
		for (int i = 0; i < n_nodi; i++)
		{
			vector<int> v1;
			for (int j = 0; j < n_nodi; j++) {
				cout << "Distanza da " << vect_citta[i] << " a " << vect_citta[j] << ": ";
				cin >> x;
				v1.push_back(x);
			}
			graph.push_back(v1);
		}
		return graph;
	}
	else {
		//correggi quiwfghsnefhuweicnhfwefwiecoriwehufwefnweuchfnweoifnowefcoiwefnwefhunwecfhwe
		return costruzione_graph_classe(n_nodi, vect_citta); //sul database uso la funzione che mi controlla
	}
}

vector<string> crea_vect_citta(int& n_nodi) {
	string citta;
	vector<string> vet;
	db_le_citta db;

	cout << "Inserisci i nomi delle citta / i nodi: ";
	for (int o = 0; o < n_nodi; o++)
	{
		do
		{
			cin >> citta;
			for (int u = 0; u < citta.size(); u++)
			{
				citta[u] = tolower(citta[u]);
			}
			if (controllo_presenza(citta) == false)
			{
				cout << "Citta non dichiarata!" << endl;
			}
		} while (controllo_presenza(citta) == false);
		vet.push_back(citta);
	}
	
	return vet;
}

int prendi_source(int& n_nodi, vector<string>& vect_citta) {
	string rad;
	cout << "Inserisci il nome della citta da cui vuoi partire: ";
	
	do
	{
		cin >> rad;
		for (int u = 0; u < rad.size(); u++)
		{
			rad[u] = tolower(rad[u]);
		}
		if (controllo_presenza(rad) == false)
		{
			cout << "Citta non dichiarata!" << endl;
		}
	} while (controllo_presenza(rad) == false);

	for (int w = 0; w < n_nodi; w++)
	{
		if (rad == vect_citta[w]) {
			return w;
		}
	}
}

bool controllo_presenza(string& input) {
	db_le_citta db;
	for (int i = 0; i < db.nomi_citta.size(); i++)
	{
		if (input == db.nomi_citta[i])
		{
			return true;
		}
	}
	return false;
}

vector<vector<int>> costruzione_graph_classe(int n_nodi, vector<string> vect_citta)
{
	db_le_citta db;
	vector<vector<int>> v1;
	for (int i = 0; i < n_nodi; i++)
	{
		v1.push_back(vector<int>());
		for (int j = 0; j < n_nodi; j++)
		{
			if (vect_citta[i] == db.nomi_citta[j])
			{
				v1.push_back(db.distanze_citta[j]);
			}
		}
	}
	return v1;
}