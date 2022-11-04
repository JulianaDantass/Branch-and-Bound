#include <iostream>
#include <vector>
#include <iterator>
#include <list>
#include "data.h"
#include "hungarian.h"

using namespace std;

struct Node {     //estrutura para cada nó da árvore

	std::vector<pair<int, int>> forbidden_arcs;     //lista de arcos proibidos
	std::vector<std::vector<int>> subtours;          //contem os subtours da solucao

	double lower_bound;             //custo total da solucao do algoritmo hungaro
	int chosen_subtour;             // indice do menor subtour (que será escolhido)
	bool feasible;                  //indica se a solucao do AP é viável
 };


// void BnB (){

// 	tree = std::list<Node>;

// 	tree.push_back(root);

// 	double upper_bound = std::numeric_limits::infinity<double>();

// 	while (!tree.empty()){

// 		auto node = chooseNode();
// 		vector<vector<int>> subtour = getSolutionHungarian(*node);
	
// 		if (node->lower_bound > upper_bound){
// 			tree.erase(node);
// 			continue;
// 		}

// 		if (node->feasible){
// 			upper_bound = min(upper_bound, node->lower_bound);
// 		}
		
// 		/* Gerando os filhos do no raiz */
// 		for(int i = 0; i < node.subtour[root.chosen].size() - 1; i++){ // iterar por todos os arcos do subtour escolhido
// 			Node n;
// 			n.arcos_proibidos = raiz.arcos_proibidos;

// 			std::pair<int, int> forbidden_arc= {node.subtour[root.chosen][i], node.subtour[root.chosen][i+1]};

// 			n.forbidden_arcs.push_back(forbidden_arc);
// 			arvore.push_back(n);
// 		}
		
// 		tree.erase(node);
// 	}
	
// }


int ExtractingSubtours(int last_node, int inicial_node, hungarian_problem_t p){     //funcao para ajudar a extrair os subtours 

	int out;

	for(int j = 0; j < p.num_cols; j++){

		if(p.assignment[last_node][j] == 1){

			if(j == inicial_node){                  //se o ultimo j for o igual ao nó inicial retorna a flag -1 
				
				out = -1;
			}else{

				out = j;
			}
			
		}
	} 
}

void ExcludingNodes(vector<int> &nodes, vector<int> tour_to_exclue){

	int count = 0;

	for(int i = 0; i < nodes.size(); i++){

		for(int j = 0; j < tour_to_exclue.size(); j++){

			count++;

			if(nodes[i] == tour_to_exclue[j]-1){

				nodes.erase(nodes.begin()+i);
			}
		}
	}
}


int main(int argc, char** argv) {      

	std::list<Node> tree;     //declaracao da variavel da arvore
	Node root;

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

	//----------------------------------------------//


	//cout << "ola " << p.assignment[1][0];
	//exit(0);
	int out;
	int last_node;

	
	vector<int> total_nodes; 
	vector<vector<int>> total_subtours;

	for (int i = 0; i < p.num_rows; i++){                          
		
		total_nodes.push_back(i);
	}


	while(1){

		vector<int> tour;
		int flag = 0;

		last_node = total_nodes[0];

		for(int z = 0; z < total_nodes.size(); z++){

			int i = total_nodes[z]; 

			for (int j = 0; j < p.num_cols; j++){                         

				if(p.assignment[i][j] == 1 and (last_node == i) ){
					
					//cout << i << " " << j << endl;
					//getchar();

					tour.push_back(i+1);

					if(j == tour[0]-1){
						tour.push_back(tour[0]);
					}

					last_node = j;

					break;
				}
			}
    	}

		if( total_nodes.empty() ){             //se nao tem mais nós a pecorrer
				break;

		}else{
			total_subtours.push_back(tour);
			ExcludingNodes(total_nodes, tour);
		}

		//getchar();
	}
	

		for(int i= 0; i < total_subtours.size(); i++){
			cout << "imprimindo tour: " << endl;

			for(int j= 0; j < total_subtours[i].size(); j++){

				cout << total_subtours[i][j] << " ";
			}
			cout << endl;
		}


	//----------- deletando memória ------------------//

	hungarian_free(&p);
	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;
	//------------------------------------------------//

	return 0;
}
