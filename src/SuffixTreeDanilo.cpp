#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>		
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

#define DB( x ) cerr << #x << " = " << x << endl
#define _ << " " <<

class SuffixTree
{
		
	public:		
				
		static const int MAXSIZE = 10000;
		
		struct Node;
		struct Edge;
		struct Suffix;
		
		//TODO colocar estruturas em outros locais
		
		struct Edge
		{
			int l,r;
			int noInicial, noFinal;
			Edge( int l = 0, int r = 0, int noInicial = -1, int noFinal = -1) : l(l), r(r) , noInicial(noInicial), 
				noFinal(noFinal) {}

		};
		
		struct Node
		{
			int index; //TODO necessario? remover!
			int suffixLink;
			Node( int index = -1, int suffixLink = -1 ) : index( index ), suffixLink(suffixLink) {}
		};
		
		//tambem chamado active node no Ukkonen's
		struct Suffix 
		{
			int node; // no pai do suffixo, ou o no ativo que esta sendo avaliado
			int l, r; // indices left e right do sufixo
			Suffix( int node = -1, int l = -1, int r = -1 ) : node(node), l(l), r(r) {}			
		};
		
		
		Node nodesArr[MAXSIZE * 2];
		Edge edgesArr[MAXSIZE * 2];
		
		// mapeia <nodeIndex, char> em edgeIndex
		// indica o indice da aresta de "saida" de um no pelo char em edgesArr
		map< pair<int,char> , int > mapEdges; //TODO use unordered_map?
		
		string str;
		int nodeCount;
		int stringSize;
		Node rootNode;			
		
		void insertNode(Node & node )
		{
			nodesArr[node.index] = node;
		}
		
		void insertEdge(Edge & edge)
		{
			//cada aresta esta associada a um unico no final, 
			//entao podemos usar o index desse no como index da aresta
			int edgeIndex = edge.noFinal;			
			// DB( edge.l _ edge.r  _ edge.noInicial _ edgeIndex  );
			edgesArr[edgeIndex] = edge; 
			mapEdges[ make_pair( edge.noInicial, str[edge.l] ) ] = edgeIndex;
		}
		
		Suffix canonize(Suffix active )
		{			
			//TODO!! entra no if se for < ou >= ?			
			
			DB( active.node _ active.l _ active.r );
			if( active.l <= active.r ){	
				//no implicito
				//localiza aresta u -> v tal que o char de saidada seja = str[active.l]
				int edgeIndex = mapEdges[ make_pair( active.node, str[ active.l ] )];
				Edge edge = edgesArr[edgeIndex];								
				while( (edge.r - edge.l) <= (active.r - active.l)){
					active.node = edge.noFinal;
					active.l += (edge.r - edge.l) + 1;
					if( active.l <= active.r ){
						edgeIndex = mapEdges[make_pair( active.node, str[ active.l ] ) ];
						edge = edgesArr[edgeIndex];
					}
				}
			}			
			return active;
		}
		
		pair<bool, int> testAndSplit( Suffix active, int pos)
		{
			if( active.l <= active.r ){ 
				// no implicito, entao existe aresta com char str[active.l]
				int edgeIndex = mapEdges[ make_pair( active.node, str[ active.l ] )];
				Edge edge = edgesArr[edgeIndex];
				if( str[ edge.l + (active.r - active.l) + 1 ] == str[pos]){					
					// é no terminal
					return make_pair( true, edge.noInicial );
				} else {
					// split edge
					Node newNode(nodeCount++, active.node);
					insertNode( newNode );				
					// DB( newNode.index _ newNode.suffixLink );
					//TODO qual o suffix link de new node?															
					mapEdges.erase( make_pair( edge.noInicial , str[ edge.l ]) );					
					
					Edge newEdge;
					newEdge = Edge( edge.l , edge.l + (active.r - active.l ), active.node , newNode.index );								
					insertEdge( newEdge );
					
					nodesArr[ newNode.index ].suffixLink = active.node;
					
					newEdge =  Edge( edge.l + (active.r - active.l ) + 1, edge.r, newNode.index, edge.noFinal );			
					insertEdge( newEdge );
					
					
					return make_pair( false, newNode.index );
				}
			} else {
				
				//testar se existe aresta u -> v com rotulo inicial para X[i]				
				if( mapEdges.count( make_pair(active.node , str[pos] ) ) > 0){
					return make_pair( true, active.node );
				} else {
					return make_pair( false, active.node );
				}
			}
		}
		
		Suffix update(Suffix active, int pos)
		{			
			int w2 = -1;
			pair<bool, int> ret = testAndSplit( active, pos );
			bool is_term = ret.first;
			int w1 = ret.second;				
			while( !is_term ){			
				//TODO!!
				
				// DB( "aqui" _ active.node _ active.l _ active.r );
				Node node(nodeCount++, active.node); //TODO criar no v sem suffix link?
				insertNode( node );
				Edge edge( pos, stringSize, w1, node.index); // criar aresta w -> v (i,strinSize)
				insertEdge( edge );
				if( w2 != -1){
					nodesArr[w2].suffixLink = w1;
				}		
				w2 = w1;				
				
				//Caso especial para raiz
				if( active.node == rootNode.index ){
					active.l++;
				} else {
					// DB( "aqui lol" _ active.node _ nodesArr[active.node].suffixLink);
					active.node = nodesArr[active.node].suffixLink;
					
				}
				active = canonize( Suffix( active.node, active.l, active.r )  );				
				ret = testAndSplit( active, pos );
				is_term = ret.first;
				w1 = ret.second;
			}
			if( w2 != rootNode.index){
				nodesArr[w2].suffixLink = w1;
			}
			active.r++; // proximo ponto ativo
			return active;
		}	

		void initialize()
		{
			nodeCount = 0;			
			mapEdges.clear();			
			rootNode = Node(nodeCount++, -1 );
			insertNode( rootNode );
			stringSize = str.size();
		}
		
		SuffixTree(const string & _str) : str(_str) 
		{			
			//Utiliza algoritmo do Ukkonen
			initialize();			
			Suffix active( rootNode.index, 0, -1 ); // a string str e 0-based
			for( int i = 0; i < stringSize; i++){
				active = update( active, i );				
				active = canonize( active );
			}
			
		}
		
		char * getByteRepresentation(){
			return NULL;
		}
		
		void find_occurrence(string & patt ){
			
		}

};

int main(int argc, char** argv)
{	
	string str = "GATAGACA";
    SuffixTree t(str);	
    return 0;
}
