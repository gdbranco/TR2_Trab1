#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <map>
#include <set>
#include <queue>
#include <string>

using namespace std;
typedef struct _in
{
	int nome; 
	int peso;
	_in(int _nome, int _peso): nome(_nome), peso(_peso){}
	_in(){}
}intern_node;
typedef struct _en
{
	int nome;
	list<intern_node> vizinhos;
	friend ostream& operator<<(ostream& os,const _en& it)
	{
		os << it.nome << " ";
		for(list<intern_node>::const_iterator i=it.vizinhos.begin();i!=it.vizinhos.end();i++)
		{
			os << i->nome << "[" << i->peso << "] ";
		}
		return os;
	}
}extern_node;

typedef struct _edge{
	string nome;
	int peso;

	bool operator<(const _edge& rhs) const
	{
		return nome < rhs.nome;
	}

	bool operator>(const _edge& rhs) const
	{
		return peso > rhs.peso;
	}

	bool operator==(const _edge& rhs) const
	{
		return nome == rhs.nome;
	}
}Edge;

void add_element(vector<extern_node>& src, int vertex, int vizinho, int peso);
vector<extern_node> utils_tomem(fstream &sc);
void gerar_vertices_arestas(vector<extern_node> grafo,set<int>& vertices,set<Edge>& edges);
vector<extern_node> kruskal(set<int> vertices, set<Edge> edges);
void merge(int a, int b, set<set<int> > &comp);
bool find(int a, int b, set<set<int> > comp);
void add_elemento(vector<extern_node>& lista, int a, int b, int peso)
{
	bool achou = false;
	int flag = 0;
	for(vector<extern_node>::iterator i=lista.begin();i!=lista.end();i++)
	{
		if(i->nome == a)
		{
			achou = true;
			for(list<intern_node>::iterator it = i->vizinhos.begin(); it != i->vizinhos.end(); it++){
				if(it->nome == b){
					flag = 1;
					break;
				}
			}
			if(!flag)
				i->vizinhos.push_back(intern_node(b,peso));
		}
	}
	if(!achou)
	{
		extern_node n;
		n.nome = a;
		n.vizinhos.push_back(intern_node(b,peso));
		lista.push_back(n);
	}
}
vector<extern_node> utils_tomem(fstream &sc)
{
	vector<extern_node> grafo;
	char *pch;
	string s;
	int nome;
	while(getline(sc,s))
	{
		vector<int> lista;
		if(s!="")
		{
			pch = strtok((char*)s.c_str(),"\t -[];");
			while(pch!=NULL)
			{
				lista.push_back(atoi(pch));
				pch = strtok(NULL,"\t -[];");
			}
			nome = lista.at(0);
			for(int i=1;i<(int)lista.size();i+=2)
			{
				add_elemento(grafo,nome,lista.at(i),lista.at(i+1));
				add_elemento(grafo,lista.at(i),nome,lista.at(i+1));
				//n.vizinhos.push_back(intern_node(lista.at(i),lista.at(i+1)));
			}
			//grafo.push_back(n);
		}
	}
	return grafo;
}

//Argumentos: vertice 'a', vertice 'b', e um conjunto de conjuntos de ints
//Funcao: retorna true se 'a' e 'b' estão no mesmo conjunto. Retorna false caso contrario
bool find(int a, int b, set<set<int> > comp){
	set<int> comp_aux;
	int flag;
	for(set<set<int> >::iterator it = comp.begin(); it != comp.end(); it++){
		comp_aux = *it;
		flag = 0;
		for(set<int>::iterator it2 = comp_aux.begin(); it2 != comp_aux.end(); it2++){
			if( (a == *it2) || (b == *it2) )
				flag++;
		}
		if(flag == 2)
			return true;
	}
	return false;
}

//Argumentos: vertices 'a' e 'b', e um conjunto de conjuntos de intern_node
//Funcao: Procura o conjunto aonde 'a' esta, procura o conjunto aonde 'b' esta e faz a uniao dos 2 conjuntos
void merge(int a, int b, set<set<int> > &comp){
	set<int> comp_aux, comp_aux_a, comp_aux_b;
	int flag = 0;
	for(set<set<int> >::iterator it = comp.begin(); it != comp.end(); it++){
		comp_aux = *it;
		for(set<int>::iterator it2 = comp_aux.begin(); it2 != comp_aux.end(); it2++){
			if(a == *it2){
				flag++;
				comp_aux_a = comp_aux;
				break;
			}
			else if(b == *it2){
				flag++;
				comp_aux_b = comp_aux;
				break;
			}
		}
		if(flag == 2)
			break;
	}
	comp.erase(comp_aux_a);
	comp.erase(comp_aux_b);
	comp_aux_a.insert(comp_aux_b.begin(), comp_aux_b.end());
	comp.insert(comp_aux_a);
}


vector<extern_node> kruskal(set<int> vertices, set<Edge> edges){
	priority_queue<Edge, vector<Edge>, greater<Edge> > p_edges;
	int ncomp = vertices.size();
	set<set<int> > components;
	set<int> aux;
	Edge e;
	char* aux_s;
	int a, b;
	vector<extern_node> lista;

	//Components eh um conjunto de 'conjunto de ints'.
	for(set<int>::iterator it = vertices.begin(); it != vertices.end(); it++){
		aux.insert(*it);
		components.insert(aux);
		aux.clear();
	}
	//Colocamos as arestas com seus respectivos pesos na priority_queue (que ira ordenar de acordo com o peso)
	for(set<Edge>::iterator iter=edges.begin(); iter != edges.end(); ++iter){
		p_edges.push(*iter);
	}
	
	//Grafo final tera vertices-1 arestas
	while(ncomp > 1){
		e = p_edges.top();
		p_edges.pop();
		aux_s = (char*)e.nome.c_str();
		aux_s = strtok(aux_s, ",");
		a = stoi(aux_s);
		aux_s = strtok(NULL, "\n\0");
		b = stoi(aux_s);
		if(!find(a, b, components)){
			merge(a,b,components);
			ncomp--;
			cout << "ACEITOU -> NOME: " << a << "," << b << "/ PESO: " << e.peso << endl;
			add_elemento(lista, a,b,e.peso);
			add_elemento(lista, b,a,e.peso);
		}
		else
		{
			cout << "CICLO DESCARTADO -> NOME: " << a << "," << b << "/ PESO: " << e.peso << endl;
		}
	}
	return lista;
}

void gerar_vertices_arestas(vector<extern_node> grafo,set<int>& vertices,set<Edge>& edges)
{
	int tam = grafo.size();
	for(int i=0;i<tam;i++)
	{
		//Inserimos os vertices no conjunto de vertices
		vertices.insert(grafo.at(i).nome);
		list<intern_node> viz_aux = grafo.at(i).vizinhos;
		list<intern_node>::iterator it = viz_aux.begin();

		//For para andar pela lista de vizinhos do vertice que acabou de ser inserido no conjunto
		for(;it != viz_aux.end();it++)
		{
			string nome;
			Edge aux;

			nome.clear();

			//nome da aresta eh sempre uma string 'MENOR,MAIOR' onde MENOR e MAIOR sao numeros (por enquanto)
			if(grafo.at(i).nome <= it->nome){
				nome.append(to_string(grafo.at(i).nome) );
				nome += ',';
				nome.append(to_string(it->nome));
			}
			else{
				nome.append(to_string(it->nome));
				nome += ',';
				nome.append(to_string(grafo.at(i).nome) );
			}
			aux.nome = nome;
			aux.peso = it->peso;

			//Inserimos a aresta no conjunto de arestas. Caso o nome da aresta ja exista, o conjunto nao insere nada
			//if(edges.find(aux) == edges.end() )
			edges.insert(aux);
		}
	}
}

int main(int argc,char**argv)
{
	fstream sc;
	ofstream output;
	string file_name;
	
	if(argc < 2)
	{
		cerr << "Usage: ./teste <nome_arq>.<ext>"<< endl;
		return 1;
	}

	sc.open(argv[1]);
	if (!sc.is_open()){
		cout << "Arquivo inexistente\n";
		return 1;
	}
	
	
	vector<extern_node> grafo = utils_tomem(sc);
	sc.close();
	for(int i=0;i<(int) grafo.size();i++)
	{
		cout << grafo[i] << endl;
	}

	set<int> vertices;
	set<Edge> edges;
	gerar_vertices_arestas(grafo,vertices,edges);

	grafo = kruskal(vertices, edges);
	
	file_name = "saida_";
	file_name.append(argv[1]);
	output.open(file_name);
	for(int i=0;i<(int) grafo.size();i++)
	{
		cout << grafo[i] << endl;
		output << grafo[i] << endl;
	}
	output.close();
	return 0;

}
