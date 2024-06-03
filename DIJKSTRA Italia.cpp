#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define NON_HA_PARENT -1
using namespace std;

//funzioni generali
bool controllo_presenza_classe(string input);
bool controllo_presenza_vettore(string input, vector<string> vect_citta);
bool controllo_presenza_file(string input);
bool risp_main();
void dijkstra();

//specifiche
int prendi_n_nodi();
vector<string> crea_vect_citta(int n_nodi);
vector<vector<int>> costruisci_graph(int n_nodi, vector<string> vect_citta);
int prendi_source(int n_nodi, vector<string> vect_citta);

//algoritmo
int controllo_dei_nodi_vicini(vector<int> distanze, vector<bool> sono_stati_visitati, int n_nodi);
void algoritmo(vector<vector<int>> graph, int radice, int n_nodi, vector<string> vect_citta);
void mostra_path(int vertice_corrente, vector<int> parent, vector<string>vect_citta);



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
										  {0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,143,169,0  }           //torino
										   
	};

	vector<string> nomi_citta = {"bari","lecce","catania","palermo","napoli","roma","pescara","firenze","bologna","venezia","milano","genova","torino"};

	vector<vector<int>> costruzione_graph_classe(int n_nodi, vector<string> vect_citta)
	{
		vector<vector<int>> v1;
		for (int i = 0; i < n_nodi; i++) // loop per n nodi che ho messo
		{
			for (int j = 0; j < nomi_citta.size(); j++) //loop per n citta che esistono
			{
				if (vect_citta[i] == nomi_citta[j])
				{
					v1.push_back(distanze_citta[j]);
				}
			}
		}
		return v1;
	}

	void aggiungi_citta() {
		string nuova_citta;
		fstream f;
		string s;
		f.open("nomidellecitta", ios::app);
		
		cout << "Inserisci il nome della nuova citta: ";
		cin >> nuova_citta;
		for (int u = 0; u < nuova_citta.size(); u++)
		{
			nuova_citta[u] = tolower(nuova_citta[u]);
		}
		
		while (controllo_presenza_file(nuova_citta) == true) //c'è la città
		{
			cout << "Citta fa gia parte del database!" << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> nuova_citta;
		}
		nomi_citta.push_back(nuova_citta);
		f << "\n";
		f<<nuova_citta;

		cout << "Inserisci le distanze della nuova citta per " << endl;
		vector<int> distanze_nuova_citta;
		int d = 0;

		for (int i = 0; i < nomi_citta.size(); i++)
		{
			f >> s;
			cout << s;
			cin >> d;
			distanze_nuova_citta.push_back(d);
		}
		f.close();
		cout << "Ecco il vettore con le distanze della nuova citta!"<<endl;
		for (int a = 0; a < distanze_nuova_citta.size(); a++)
		{
			cout << distanze_nuova_citta[a] << " , ";
		}

	}

	void display_nomi() {
		cout << "Ecco le citta disponibili" << endl;
		ifstream f;
		string nomi;
		int i = 0;
		f.open("nomidellecitta", ios::in);
		while (getline(f, nomi))
		{
			cout << i<<") "<< nomi << endl;
			i++;
			istringstream iss(nomi);
			if (!(iss >> nomi)) {
				cerr << "Errore: " << nomi << "\n";
				continue;
			}
		}
		f.close();
	}

	void display_database() {
		cout << "Ecco le distanze presenti nel database" << endl;
		for (int i = 0; i < distanze_citta.size(); i++)
		{
			cout << "Citta di "<< nomi_citta[i] << ":   \t";
			for (int j = 0; j < distanze_citta[i].size(); j++)
			{
				cout << distanze_citta[i][j] << " ";
			}cout << endl;
		}
	}

	int prendi_source_classe(int n_nodi) {
		string rad;
		vector<string>::iterator a;

		cout << "Inserisci il nome della citta da cui vuoi partire: ";
		cin >> rad;
		for (int u = 0; u < rad.size(); u++)
		{
			rad[u] = tolower(rad[u]);
		}

		while (controllo_presenza_classe(rad) == false) //non c'è la città
		{
			cout << "Citta non fa parte delle citta nel database!" << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> rad;
		}

		a = find(nomi_citta.begin(), nomi_citta.end(), rad);
		if (a != nomi_citta.end())
		{
			return (a - nomi_citta.begin());
		}

	}

	void dijkstrasudb() {
		int source = prendi_source_classe(nomi_citta.size());
		algoritmo(distanze_citta, source, nomi_citta.size(), nomi_citta);
	}
};

int main()
{
	while(1){
		db_le_citta db;
		int x;
		cout << "Ciao! Cosa vuoi fare?" << endl;
		cout << "1. Aggiungere una nuova citta" << endl;
		cout << "2. Vedere le citta disponibili" << endl;
		cout << "3. Vedere il database" << endl;
		cout << "4. Usare Dijkstra" << endl;
		cout << "5. Dijkstra su tutto il database" << endl;
		cin >> x;
		switch (x)
		{
		case 1:
			db.aggiungi_citta();
			break;
		case 2:
			db.display_nomi();
			break;
		case 3:
			db.display_database();
			break;
		case 4:
			dijkstra();
			break;
		case 5:
			db.dijkstrasudb();
			break;
		default:
			break;
		}	
		cout << endl;
	}
	
	return 0;
}

int prendi_n_nodi() {
	int n_nodi;
	cout << "Inserisci il numero di nodi: ";
	cin >> n_nodi;
	while (cin.fail())
	{
		cout << "Inserisci un numero!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
		cin >> n_nodi;
	}
	return n_nodi;
	
}

vector<string> crea_vect_citta(int n_nodi) {
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
			if (controllo_presenza_classe(citta) == false)
			{
				cout << "Citta non dichiarata!" << endl;
			}
			
		} while (controllo_presenza_classe(citta) == false);
		vet.push_back(citta);
	}
	
	return vet;
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
		db_le_citta db;
		return db.costruzione_graph_classe(n_nodi, vect_citta);
	}
}

int prendi_source(int n_nodi, vector<string> vect_citta) {
	string rad;
	vector<string>::iterator f;
	
	cout << "Inserisci il nome della citta da cui vuoi partire: ";
	cin >> rad;
	for (int u = 0; u < rad.size(); u++)
	{
		rad[u] = tolower(rad[u]);
	}

	while (controllo_presenza_classe(rad) == false || controllo_presenza_vettore(rad, vect_citta) == false) //non c'è la città
	{
		cout << "Citta non fa parte delle città nel database o è estranea alle citta che hai selezionato!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
		cin >> rad;
	}

	f = find(vect_citta.begin(), vect_citta.end(), rad);
	if (f != vect_citta.end())
	{
		return (f - vect_citta.begin());
	} 
	
}

bool controllo_presenza_classe(string input) {
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

bool controllo_presenza_vettore(string input, vector<string> vect_citta) {
	for (int i = 0; i <vect_citta.size(); i++)
	{
		if (input == vect_citta[i])
		{
			return true;
		}
	}
	return false;
}

int controllo_dei_nodi_vicini(vector<int> distanze, vector<bool> sono_stati_visitati, int n_nodi) {
	int valoremin = INT_MAX;
	int nodominimo = 0;
	for (int i = 0; i < n_nodi; i++)
	{
		if (!sono_stati_visitati[i] && distanze[i] <= valoremin)
			//viene preso e aggiunto il nodo con la 
			//minima distanza che non è nel vect bool dei visitati
		{
			valoremin = distanze[i];
			nodominimo = i;
		}
	}
	return nodominimo; //viene dato all'algoritmo
}

void algoritmo(vector<vector<int>> graph, int radice, int n_nodi, vector<string> vect_citta) {

	vector<int> distanze(n_nodi), parent(n_nodi);
	vector<bool> sono_stati_visitati(n_nodi); //flags per i visitati
	//inizializzo
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
				&& distanze[nodo_vicino] + graph[nodo_vicino][adiacente] <= distanze[adiacente]) //il peso del viaggio dalla source al nodo adiacente è piccolo rispetto alla distanza corrente dell'adiacente
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
			cout << "Nodo: " << vect_citta[radice] << " -> " << vect_citta[i] << "\t\tDistanza: " << distanze[i] << "\t\tPercorso: ";
			mostra_path(i, parent, vect_citta);
		}
	}
} 

void mostra_path(int vertice_corrente, vector<int> parent, vector<string>vect_citta) {
	if (vertice_corrente == NON_HA_PARENT) {
		return;
	}
	mostra_path(parent[vertice_corrente], parent, vect_citta);
	cout << vect_citta[vertice_corrente] << " ";
}

bool risp_main() {
	string risp;
	cout << "Continuare? ";
	cin >> risp;

	for (int u = 0; u < risp.size(); u++)
	{
		risp[u] = tolower(risp[u]);
	}

	if (risp == "si")
	{
		return true;
	}
	else
	{
		return false;
	}
}

void dijkstra() {
	int n_nodi = prendi_n_nodi();
	vector<string> vect_citta = crea_vect_citta(n_nodi);
	vector<vector<int>> graph = costruisci_graph(n_nodi, vect_citta);
	int source = prendi_source(n_nodi, vect_citta);
	algoritmo(graph, source, n_nodi, vect_citta);
}

bool controllo_presenza_file(string input) {
	ifstream f;
	string nomi;
	f.open("nomidellecitta", ios::in);
	while (getline(f, nomi))
	{
		istringstream iss(nomi);
		if (!(iss >> nomi)) {
			std::cerr << "Errore: " << nomi << "\n";
			continue;
		}
		if (input == nomi)
		{
			return true;
		}
	}
	return false;

	f.close();
}

