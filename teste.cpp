#include <iostream>
#include <vector>
#include <iterator>
#include <list>
#include <limits>


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
																		 //issaq ta bichado!!!!!!1
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

int main(){

	
}