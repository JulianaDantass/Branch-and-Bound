#include <iostream>
#include <vector>
#include <iterator>
#include <list>
using namespace std;

#include "data.h"
#include "hungarian.h"


struct Node {     //estrutura para cada nó da árvore

	std::vector<pair<int, int>> forbidden_arcs;     //lista de arcos proibidos
	std::vector<std::vector<int>> subtour;          //contem os subtours da solucao

	double lower_bound;             //custo total da solucao do algoritmo hungaro
	int chosen_subtour;             // indice do menor subtour (que será escolhido)
	bool feasible;                  //indica se a solucao do AP é viável
 };


void BnB (){

	tree = std::list<Node>;
	tree.push_back(root);
	double upper_bound = std::numeric_limits::infinity<double>();

	while (!tree.empty()){

		auto node = chooseNode();
		vector<vector<int>> subtour = getSolutionHungarian(*node);
	
		if (node->lower_bound > upper_bound){
			tree.erase(node);
			continue;
		}

		if (node->feasible){
			upper_bound = min(upper_bound, node->lower_bound);
		}
		
		/* Gerando os filhos do no raiz */
		for(int i = 0; i < node.subtour[root.chosen].size() - 1; i++){ // iterarpor todos os arcos do subtour escolhido
			Node n;
			n.arcos_proibidos = raiz.arcos_proibidos;

		std::pair<int, int> forbidden_arc= {node.subtour[root.chosen][i], node.subtour[root.chosen][i+1]};

		n.forbidden_arcs.push_back(forbidden_arc);
		arvore.push_back(n);
		}
		
		tree.erase(node);
	}
	
}


int main(int argc, char** argv) {      

	//-------- chamando o algoritmo hungaro -----------//

	Data * data = new Data(argc, argv[1]);
	data->readData();

	double **cost = new double*[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}

	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); // Carregando o problema

	double obj_value = hungarian_solve(&p);
	cout << "Obj. value: " << obj_value << endl;

	cout << "Assignment" << endl;
	hungarian_print_assignment(&p);

	hungarian_free(&p);
	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	//----------------------------------------------//


	return 0;
}
