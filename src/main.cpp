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


int ExtractingSubtours(int last_node, hungarian_problem_t &p){     //funcao para ajudar a extrair os subtours 

	int out= -1;

	for(int j = 0; j < p.num_cols; j++){

		if(p.assignment[last_node][j] == 1){

			out = j;
		}
	} 
	
	return out;
}

int ExcludingNode(vector<int> &nodes, int node_to_exclude){

	int out= -1; 

	for(int i = 0; i < nodes.size(); i++){

		if(nodes[i] == node_to_exclude){

			out= 1;
			nodes.erase(nodes.begin()+i);
		}
	}

	return out;
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

	vector<int> tours;
	vector<int> total_nodes; 
	vector<vector<int>> total_subtours(p.num_rows);

	for (int i = 0; i < p.num_rows; i++){                          
		
		total_nodes.push_back(i);
	}


	while(1){

		last_node = total_nodes[0];                            //o ultimo no

		cout << "last node" << last_node;

		for (int i = 0; i < p.num_rows; i++){        

			for (int j = 0; j < p.num_cols; j++){                         

				if(p.assignment[i][j] == 1 && (i == last_node) ){
					
					cout << "i " << i << " j " << j;

					tours.push_back(i+1);
					last_node =  j;
					
					// cout << "last node " << j;
					out= ExcludingNode(total_nodes, i);
	
				}
			}
			
			total_subtours(tours);                               //adicionando os subtours;

			if(out == -1){
						
				break; 
			}
			
		}

		if(total_nodes.empty()){
			break;
		}
	
	}
	
	


	//----------- deletando memória ------------------//

	hungarian_free(&p);
	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;
	//------------------------------------------------//

	return 0;
}
