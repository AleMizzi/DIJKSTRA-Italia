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
bool controllo_presenza_vettore(string input, vector<string> vect_citta);
bool controllo_presenza_file(string input);
bool risp_main();
void dijkstra();

//specifiche
int prendi_n_nodi(string r);
vector<string> crea_vect_citta(int n_nodi);
vector<string> crea_vect_nodi(int n_nodi);
vector<vector<int>> costruisci_graph(int n_nodi, vector<string> vect_citta);
vector<vector<int>> costruisci_graph_nodi(int n_nodi, vector<string> vect_nodi);
int prendi_source(int n_nodi, vector<string> vect_citta);
int prendi_source_nodi(int n_nodi, vector<string> vect_nodi);

//algoritmo
int controllo_dei_nodi_vicini(vector<int> distanze, vector<bool> sono_stati_visitati, int n_nodi);
void algoritmo(vector<vector<int>> graph, int radice, int n_nodi, vector<string> vect_citta);
void mostra_path(int vertice_corrente, vector<int> parent, vector<string>vect_citta);



class db_le_citta
{
public:

	vector < vector<int> > distanze_citta = carica_distanze_dal_file();
	vector<string> nomi_citta = carica_dal_file();

	void aggiungi_citta() {
		string nuova_citta;
		fstream f;
		fstream f2;
		string s;
		f.open("nomidellecitta", ios::app);
		
		cout << "Inserisci il nome della nuova citta: ";
		cin >> nuova_citta;
		for (int u = 0; u < nuova_citta.size(); u++)
		{
			nuova_citta[u] = tolower(nuova_citta[u]);
		}
		
		while (controllo_presenza_file(nuova_citta) == true)
		{
			cout << "Citta fa gia parte del database!" << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> nuova_citta;
		}

		nomi_citta.push_back(nuova_citta);
		f << nuova_citta << endl;
		f.close();

	}

	void carica_distanze() {
		fstream f;
		string citta;
		int distanza = 0;

		cout << "Quale citta scegli?" << endl;
		cin >> citta;

		f.open(citta, ios::app);
		cout << "Inserisci le distanze della citta di" << citta << " per " << endl;

		for (int i = 0; i < nomi_citta.size(); i++)
		{
			cout << i << ") " << nomi_citta[i] << ": ";
			cin >> distanza;
			f << distanza << endl;
		}

	}

	void display_nomi() {
		cout << "Ecco le citta disponibili:" << endl;
		ifstream f;
		string nome;
		int i = 0;
		f.open("nomidellecitta", ios::in);
		while (getline(f, nome))
		{
			istringstream iss(nome);
			if (!(iss >> nome)) {
				cerr << "Errore casella: " << i << "\n";
				continue;
			}
			cout << i<<") "<< nome << endl;
			i++;
		}
		f.close();
	}

	void display_database() {
		cout << "Ecco le distanze presenti nel database:" << endl;
		ifstream f, f2;
		string nome_citta;
		int distanze = 0;
		f.open("nomidellecitta", ios::in);
		while (getline(f, nome_citta))
		{
			istringstream iss(nome_citta);
			if (!(iss >> nome_citta)) {
				cerr << "Errore casella: " << nome_citta << "\n";
				continue;
			}

			f2.open(nome_citta, ios::in);
			cout << nome_citta << ":  \t\t";
			while (f2 >> distanze)
			{
				cout << distanze << " ";
			}
			cout << endl;
			f2.close();
		}
		f.close();
	}

	int prendi_source_classe(int n_nodi) {
		string rad;
		vector<string>::iterator a;

		cout << "Inserisci il nome del nodo (o dalla citta) da cui vuoi partire: ";
		cin >> rad;
		for (int u = 0; u < rad.size(); u++)
		{
			rad[u] = tolower(rad[u]);
		}

		while (controllo_presenza_file(rad) == false) 
		{
			cout << "Citta non fa parte delle citta nel database!" << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> rad;
		}

		ifstream f;
		string nomi;
		int t = 0;
		f.open("nomidellecitta", ios::in);
		while (getline(f, nomi))
		{
			istringstream iss(nomi); //string stream

			if (!(iss >> nomi)) {
				break;
			}
			if (rad == nomi)
			{
				return t;
			}
			t++;
		}
		f.close();

	}

	void dijkstrasudb() {
		int source = prendi_source_classe(nomi_citta.size());

		vector < vector<int> > graph;
		vector<string> vect_citta_aggiornato = { };
		ifstream file, file2;
		string nome;
		int distanze = 0;
		

		file.open("nomidellecitta", ios::in);

		while (getline(file, nome))
		{
			vector<int> vet_temp = { };
			file2.open(nome, ios::in);
			if (file2)
			{
				while (file2 >> distanze)
				{
					vet_temp.push_back(distanze);
					file2.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				vect_citta_aggiornato.push_back(nome);
			    graph.push_back(vet_temp);
			}
			else {
				continue;
			}
			file2.close();
		}
		file.close();
		algoritmo(graph, source, graph.size(), vect_citta_aggiornato);
	}

	void vedi_vettore() {
		for (int i = 0; i < nomi_citta.size(); i++)
		{
			cout << nomi_citta[i] << endl;
		}
	}

	void vedi_distanze_citta() {
		ifstream f;
		string nome_citta;
		int distanza = 0;

		cout << "Che citta scegli? " << endl;
		cin >> nome_citta;
		f.open(nome_citta, ios::in);
		vector<int> v;

		while (f >> distanza) {
			v.push_back(distanza);
			f.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		for (int i = 0; i < v.size(); i++)
		{
			cout << i <<")" << nomi_citta[i] << ": " << v[i] << endl;
		}
		f.close();
	}

	vector<vector<int>> carica_distanze_dal_file() {
		fstream file_nomi;
		fstream f2;
		string nome;
		vector<int> vet;
		int distanze = 0;
		vector<vector<int>> vettorefinale;
		file_nomi.open("nomidellecitta", ios::in);

		while (getline(file_nomi, nome))
		{
			f2.open(nome, ios::in);
			if (f2)
			{
				while (f2 >> distanze)
				{
					vet.push_back(distanze);
					f2.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				f2.close();
				vettorefinale.push_back(vet);
			}
			vet.clear();
		}
		file_nomi.close();
		return vettorefinale;
	}

	vector<string> carica_dal_file() {
		ifstream file;
		string nomi;
		vector<string> vettore;
		file.open("nomidellecitta", ios::in);
		while (getline(file, nomi))
		{
			vettore.push_back(nomi);
		}
		return vettore;
	}


};


int main()
{

	while(1){
		cout << endl;
		db_le_citta db;
		int x;
		cout << "Ciao! Cosa vuoi fare?" << endl;
		cout << "1. Usare Dijkstra" << endl;
		cout << "2. Dijkstra su tutto il database" << endl;
		cout << "3. Aggiungere una nuova citta" << endl;
		cout << "4. Carica le distanze di una citta" << endl;
		cout << "5. Vedere le citta disponibili" << endl;
		cout << "6. Vedere il database" << endl;
		cout << "7. Vedere vettore distanze di una citta" << endl;
		cout << "La tua scelta: ";

		cin >> x;
		switch (x)
		{
		case 1:
			dijkstra();
			break;
		case 2:
			db.dijkstrasudb();
			break;
		case 3:
			db.aggiungi_citta();
			break;
		case 4:
			db.carica_distanze();
			break;
		case 5:
			db.display_nomi();
			break;
		case 6:
			db.display_database();
			break;
		case 7:
			db.vedi_vettore();
			break;
		case 8:
			db.vedi_distanze_citta();
			break;
		default:
			break;
		}	
		cout << endl;
	}
	
	return 0;
}

int prendi_n_nodi(string r) {
	int n_nodi;
	cout << "Inserisci il numero di "<<r<<": ";
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

	for (int o = 0; o < n_nodi; o++)
	{
	    cout << "Inserisci la citta "<< o <<": ";
		cin >> citta;
		while (controllo_presenza_file(citta) == false)
		{
			cout << "Citta non fa parte delle citta nel database!" << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> citta;
		}
		vet.push_back(citta);
	}
	cout << endl;
	return vet;
}

vector<string> crea_vect_nodi(int n_nodi) {
	string nodi;
	vector<string> vet;

	cout << "Inserisci i nodi: ";
	for (int o = 0; o < n_nodi; o++)
	{
		cin >> nodi;
		for (int u = 0; u < nodi.size(); u++)
		{
			nodi[u] = tolower(nodi[u]);
		}
		
		vet.push_back(nodi);
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

		vector < vector<int> > graph;
		vector<int> vet_t2;
		db_le_citta db;
		int a = 0;
		vector<string>::iterator f;

		for (int i = 0; i < vect_citta.size(); i++)
		{
			f = find(db.nomi_citta.begin(), db.nomi_citta.end(), vect_citta[i]);
			if (f != db.nomi_citta.end())
			{
				a = (f - db.nomi_citta.begin());
			}

			for (int k = 0; k < db.distanze_citta[a].size() - (db.nomi_citta.size() - vect_citta.size() - 1); k++)
			{
				vet_t2.push_back(db.distanze_citta[a][k]);
			}
			graph.push_back(vet_t2);

			int a = 0;
			vet_t2.clear();
		}
		return graph;
	}
}

vector<vector<int>> costruisci_graph_nodi(int n_nodi, vector<string> vect_nodi) {

	vector<vector<int>> graph;
	cout << "Costruiamo il graph con i nodi e le loro distanze." << endl;
	int x;
	for (int i = 0; i < n_nodi; i++)
	{
		vector<int> v1;
		for (int j = 0; j < n_nodi; j++) {
			cout << "Distanza da " << vect_nodi[i] << " a " << vect_nodi[j] << ": ";
			cin >> x;
			v1.push_back(x);
		}
		graph.push_back(v1);
	}
	return graph;
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

	while (controllo_presenza_file(rad) == false ) //non c'è la città
	{
		cout << "Citta non fa parte delle città nel database!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
		cin >> rad;
	}
	while (controllo_presenza_vettore(rad , vect_citta) == false) //non c'è la città
	{
		cout << "Citta non fa parte delle città che hai selezionato!" << endl;
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

int prendi_source_nodi(int n_nodi, vector<string> vect_nodi) {
	string rad;
	vector<string>::iterator f;

	cout << "Inserisci il nome del nodo da cui vuoi partire: ";
	cin >> rad;
	for (int u = 0; u < rad.size(); u++)
	{
		rad[u] = tolower(rad[u]);
	}
	while (controllo_presenza_vettore(rad, vect_nodi) == false) //non c'è la città
	{
		cout << "Citta non fa parte del vettore nodi!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
		cin >> rad;
	}
	f = find(vect_nodi.begin(), vect_nodi.end(), rad);
	if (f != vect_nodi.end())
	{
		return (f - vect_nodi.begin());
	}
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
	cout << endl;
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
	string r = " ";
	cout << "Vuoi applicare l'algoritmo a delle citta (""citta"") oppure a nodi personali(""nodi"")?" << endl;
	cin >> r;
	while (r != "citta" && r != "nodi") //non c'è la città
	{
		cout << "Non ho capito." << endl;
		cin.clear();
		cin.ignore(256, '\n');
		cin >> r;
	}

	if (r == "citta") {
		int n_nodi = prendi_n_nodi(r);
		vector<string> vect_citta = crea_vect_citta(n_nodi);
		vector<vector<int>> graph = costruisci_graph(n_nodi, vect_citta);
		int source = prendi_source(n_nodi, vect_citta);
		algoritmo(graph, source, n_nodi, vect_citta);
	}
	else if (r == "nodi") {
		int n_nodi = prendi_n_nodi(r);
		vector<string> vect_nodi = crea_vect_nodi(n_nodi);
		vector<vector<int>> graph = costruisci_graph_nodi(n_nodi, vect_nodi);
		int source = prendi_source_nodi(n_nodi, vect_nodi);
		algoritmo(graph, source, n_nodi, vect_nodi);
	}
	
	
}

bool controllo_presenza_file(string input) {
	ifstream f;
	string nomi;
	f.open("nomidellecitta", ios::in);
	while (getline(f, nomi))
	{
		istringstream iss(nomi); //string stream

		if (!(iss >> nomi)) {
			break; 
		}
		if (input == nomi)
		{
			return true;
		}
	}
	return false;

	f.close();
}

bool controllo_presenza_vettore(string input, vector<string> vect_citta) {
	for (int i = 0; i < vect_citta.size(); i++)
	{
		if (input == vect_citta[i])
		{
			return true;
		}
	}
	return false;
}