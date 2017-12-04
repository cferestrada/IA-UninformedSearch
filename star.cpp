#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <cstdlib>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <utility>

using namespace std;

struct node{
	std::vector<stack <char> > state;
	struct node *parent;
	std::pair<int,int>  movement;
	int g, h;
};

int goalA(vector<stack <char> > thisnode, vector<stack <char> > goal){
	int i, j, aux = 1;
	for(i = 0; i < goal.size(); i++){
		if( goal[i].size() > 0 && goal[i].top() == 'X'){
			goal[i].pop();
			continue;
		}else if(thisnode[i].size() != goal[i].size()){
			return 0;
		}

		while( goal[i].size() > 0){
			if(thisnode[i].top() != goal[i].top()){
				return 0;
			}
			thisnode[i].pop();
			goal[i].pop();
		}
	}
	return aux;
}

struct consistent{
	bool operator()(node* const& a, node* const& b){
		return (a->g + a->h) > ( b->h + b->g );
	}
};

int move(struct node **child, struct node *parent, pair<int,int> movement, int z){
	char aux;

	if( !(parent->state[movement.first].empty()) && (parent->state[movement.second].size() < z)){
		(*child) = new node;
		(*child)->parent = parent;
		(*child)->state = parent->state;
		(*child)->movement = movement;
		aux = (*child)->state[movement.first].top();
		(*child)->state[movement.second].push( aux );
		(*child)->state[movement.first].pop();
		(*child)->g = parent->g + (1 + abs(movement.first - movement.second));
		return 1;
	}
	return 0;
}

int main(int arg, char** argv){

	int  heigth;
	string init,goal;
	stack<char>temp;
	vector<stack <char> > gs;
	pair<int,int>  movement;
	stack<pair <int, int> > aux;
	priority_queue <node *,vector<node *>, consistent> last;
	map < vector<stack <char> >, int > isLast;
	set < vector<stack <char> > > visited;
	set < vector<stack <char> > >::iterator itVisited;
	map < vector<stack <char> >, int >::iterator itLast;

	struct node *root = new node;
	struct node *thisNode = new node;
	struct node *childNode = new node;

	cin >> heigth;
	getline(cin,init);
	getline(cin,init);
	getline(cin,goal);

	if(root != NULL){
		for(int k =0;k < init.length(); k++){
			if(init[k]=='('){
				while(!temp.empty())
					temp.pop();
			}else if(init[k]==')'){
				root->state.push_back(temp);
			}else if(init[k]>=65 && init[k]<=90){
				temp.push(init[k]);
			}
		}
		while(!temp.empty()){
                temp.pop();
        }

		root->parent = NULL;
		root->g = 0;
	}

	for(int k =0;k < goal.length(); k++){
		if(goal[k]=='X'){
			temp.push(goal[k]);
			gs.push_back(temp);
		}else if(goal[k]=='(' || goal[k]==';'){
			while(!temp.empty()){temp.pop();}
		}else if(goal[k]==')'){
			gs.push_back(temp);
		}else if(goal[k]>=65 && goal[k]<=90){
			temp.push(goal[k]);
		}
	}
	while(!temp.empty()){
		temp.pop();
	}
    //root->hCost = heuristic(root->state, gs);
	root->h = root->g*2;
	last.push( root );
	isLast.insert( map< vector<stack <char> >, int >::value_type(root->state, root->g + root->h) );
	visited.clear();

	do{
		if(isLast.empty()){
			cout << "No solution found";
			return 0;
		}

		do{
			thisNode = last.top();
			last.pop();
		}while(isLast.erase(thisNode->state) != 1);

		if(goalA(thisNode->state, gs) == 1){
			cout << thisNode->g << endl;
			while(thisNode->parent != NULL){
				aux.push(thisNode->movement);
				thisNode = thisNode->parent;
			}

			while(!(aux.empty()) ){
				cout << "(" << aux.top().first << ", " << aux.top().second << ")";
				aux.pop();
				if (!aux.empty()) {
					cout << "; ";
				}
			}
			return 0;
		}

		visited.insert(thisNode->state);
		int i, j;
		for(i = 0; i < gs.size(); i++){
			for(j = 0; j < gs.size(); j++){
				if(i != j){
					movement = make_pair(i, j);
					if(move(&childNode, thisNode, movement, heigth) == 1){
						//childNode->h = heuristic(childNode->state, gs);
						childNode->hCost = childNode->gCost*2;
						itVisited = visited.find(childNode->state);
						itLast = isLast.find(childNode->state);
						if(itVisited == visited.end()){
							last.push(childNode);
							isLast.insert( map< vector<stack <char> >, int >::value_type(childNode->state, childNode->g + childNode->h) );
						}
						if( itLast != isLast.end() && itLast->second > (childNode->g + childNode->h)){
							isLast.erase(itLast);
							last.push(childNode);
							isLast.insert( map< vector<stack <char> >, int >::value_type(childNode->state, childNode->g + childNode->h) );

						}
					}
				}
			}
		}
	}while(last.size() > 0);

	cout << "No solution found";
	return 0;
}
