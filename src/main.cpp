#include <iostream>
#include <vector>
#include <iterator>
#include <list>
#include <limits>
#include "data.h"
#include "hungarian.h"

using namespace std;

struct Node {     //estrutura para cada nó da árvore

	std::vector<pair<int, int>> forbidden_arcs;     //lista de arcos proibidos
	std::vector<std::vector<int>> subtours;          //contem os subtours fkdokfdokfod

	double lower_bound;             //custo total da solucao do algoritmo hungaro
	int chosen;                     // indice do menor subtour (que será escolhido)
	bool feasible;                  //indica se a solucao do AP é viável pra o TSP 
 };


void ExcludingNodes(vector<int> &nodes, vector<int> tour_to_exclue){      //funcao auxiliar 

	for(int i = 0; i < nodes.size(); i++){

		for(int j = 0; j < tour_to_exclue.size(); j++){

			if(nodes[i] == tour_to_exclue[j]-1){

				nodes.erase(nodes.begin()+i);
			}
		}
	}
}

vector<vector<int>> GetSubtours(hungarian_problem_t pointer){            //funcao para separar os subtours

	int out;
	int last_node;
	vector<int> total_nodes; 
	vector<vector<int>> total_subtours;

	for (int i = 0; i < pointer.num_rows; i++){                          
		
		total_nodes.push_back(i);
	}

	while(1){                                                 

		vector<int> tour;

		last_node = total_nodes[0];                //definindo o nó inicial

		for(int z = 0; z < total_nodes.size(); z++){

			int i = total_nodes[z]; 

			for (int j = 0; j < pointer.num_cols; j++){                         

				if(pointer.assignment[i][j] == 1 and (last_node == i) ){

					tour.push_back(i+1);

					if(j == tour[0]-1){                  //se a estação atual for igual a estação inicial, significa q o tour terminou
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
			total_subtours.push_back(tour);              //adicionando o tour a lista de subtours
			ExcludingNodes(total_nodes, tour);
		}

		//getchar();
	}

	return total_subtours;
}

void PrintInformationNode(Node &some_node){         //funcao para printar as informacoes node


	for(int i = 0; i < some_node.subtours.size(); i++){     //pritando os subtours

		cout << "SUBTOUR " << i << " :" << endl;

		for(int j = 0; j < some_node.subtours[i].size(); j++){

			cout << some_node.subtours[i][j] << " ";
		}

		cout << endl;
	}

	cout << "LOWER BOUND: " << 	some_node.lower_bound << endl;
	cout << "FEASIBLE: " << some_node.feasible << endl;

}

auto chooseNode(std::list<Node> &tree){

	return tree.begin();
}

int chooseSubtour (std::vector<std::vector<int>> current){

	int possible_choice;
	int first_smaller;
	int index;

	possible_choice = current[0].size();       //o tamanho do primeiro subtour vai servir pra comparação

	for(int i = 1; i < current.size(); i++){    

		if(current[i].size() < possible_choice){       
			possible_choice = current[i].size();     //vai armazenar o tam do(s) menor(es) subtour(s) 
		}
	}

	index= 0;
	first_smaller = -1;          //flag 
	for(int i = 0; i < current.size(); i++){    

		if(current[i].size() == possible_choice){       //se o subtour for do tamanho menor

			if(first_smaller == -1){
				first_smaller = current[i][0];       //vai pegar a primeira cidade do primeiro tour analisado
				index = i;

			}else{
				if(current[i][0] < first_smaller){
					first_smaller = current[i][0];
					index = i;
				}
			}
		}
	}

	return index;
}


void getSolutionHungarian(Node &node, double dimension, double **cost){

	hungarian_problem_t p;

	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	//hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); 
	hungarian_init(&p, cost, dimension, dimension, mode); 

	double obj_value = hungarian_solve(&p);
	//cout << "Obj. value: " << obj_value << endl;

	//cout << "Assignment" << endl;
	hungarian_print_assignment(&p);

	node.subtours = GetSubtours(p);       //extrair os subtours da solucao
	node.lower_bound = obj_value;

	if(node.subtours.size() == 1){
		
		node.feasible = true;            //solucao é viavel para o tsp
	}else{
		node.feasible = false;          //solucao nao é viavel para o tsp
	}

	hungarian_free(&p);
}


void BnB (Data *data, double **cost){

	Node root;

	//getSolutionHungarian(root, data->getDimension(), cost);         
	
	std::list<Node> tree;           
	tree.push_back(root);           //criando a arvore e adicionando a raiz

	double upper_bound = std::numeric_limits<double>::infinity();

	while (!tree.empty()){

		auto node = chooseNode(tree);

		getSolutionHungarian(*node, data->getDimension(), cost);
	
		Node current_node = *node;

		if (current_node.lower_bound > upper_bound){    //se a solucao menos restritar for maior que o UB, é descartada
			
			tree.erase(node);
			continue;
		}

		if (current_node.feasible){

			if(current_node.lower_bound < upper_bound){

				upper_bound = current_node.lower_bound;
			}
		}
		
		current_node.chosen = chooseSubtour(current_node.subtours);      //funcao que determina qual subtour será escolhido 

		/* Gerando os filhos do no raiz */
		for(int i = 0; i < current_node.subtours[current_node.chosen].size() - 1; i++){  // *tentar tirar duvida disso aqui 

			Node n;
			n.arcos_proibidos = raiz.forbidden_arc;

			std::pair<int, int> forbidden_arc = {current_node.subtours[current_node.chosen][i], current_node.subtours[current_node.chosen][i+1]};

			n.forbidden_arcs.push_back(forbidden_arc);
			tree.push_back(n);
		}
		
		tree.erase(node);
	}
}



int main(int argc, char** argv) {      

	//-------- chamando o algoritmo hungaro -----------//

	Data *data = new Data(argc, argv[1]);
	data->readData();

	double **cost = new double*[data->getDimension()];

	cout << "this is dimension " << data->getDimension();

	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}


	BnB(data, cost);                    //chamando o algoritmo Branch and Bound




	//PrintSubtours(total_subtours);          //funcao para imprimir os subtours
		
	

	//----------- deletando memória ------------------//

	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;
	//------------------------------------------------//

	return 0;
}
