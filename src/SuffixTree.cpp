#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>		
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <list>
#include <cstdio>

using namespace std;


#define DB( x ) cerr << #x << " = " << x << endl
#define _ << " " <<

int tot = 0;

class SuffixTree
{
		
	public:	
		
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
			int siffixPos; // se no folha, valor da posicao que o sufixo representa
			int l, r; // indices left e right do sufixo			
			Suffix( int node = -1, int l = -1, int r = -1 ) : node(node), l(l), r(r) {}			
		};
		
		list<char> * outEdges; //usado para fazer busca
		Node * nodesArr;
		Edge * edgesArr;
		
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
			edgesArr[edgeIndex] = edge; 
			mapEdges[ make_pair( edge.noInicial, str[edge.l] ) ] = edgeIndex;
		}
		
		Suffix canonize(Suffix active )
		{			
			//TODO!! entra no if se for < ou >= ?			
			
			// DB( active.node _ active.l _ active.r );
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
	
		void createOutEdgesFromMap(){
			outEdges = new list<char>[nodeCount + 1];
			for( map< pair<int,char> , int >::iterator it = mapEdges.begin(); it != mapEdges.end(); it++){
				Edge edge = edgesArr[ it->second ];
				outEdges[edge.noInicial].push_back( str[edge.l] );
				// DB( edge.noInicial _ edge.noFinal _ str[edge.l] );
			}
		}
		
		void initialize()
		{
			nodeCount = 0;	
			stringSize = str.size();
			
			nodesArr = new Node[stringSize * 2 + 10];
			edgesArr = new Edge[stringSize * 2 + 10];
			
			mapEdges.clear();
			rootNode = Node(nodeCount++, -1 );
			insertNode( rootNode );
		}
		
		//Utiliza algoritmo do Ukkonen
		void buildTree(){
			initialize();
			Suffix active( rootNode.index, 0, -1 ); // a string str e 0-based
			for( int i = 0; i < stringSize; i++){
				active = update( active, i );				
				active = canonize( active );
			}
			createOutEdgesFromMap();
		}
		
		//TODO usar ponteiro ao inves de copiar string para a arvore
		SuffixTree(const string & _str )
		{			
			str = _str;
			buildTree();
		}
		
		SuffixTree(const string & _str, char * buffer )
		{			
			str = _str;
			constructFromByteRepresentation( buffer );
		}
		
		
		
		void insertIntToBuffer( int val, int & pos , char * buffer)
		{
			int byteMask = (1 << 8) - 1;
			for( int i = 0;i < 4; i++){
				buffer[pos++] = val & byteMask;								
				val >>= 8;
			}
		}
		
		int getIntFromBuffer(int & pos, char * buffer )
		{			
			int ret = 0;
			for( int i = 0; i < 4; i++){
				int aux = (unsigned char) buffer[pos++];
				ret += (aux << (8 * i));				
			}
			return ret;
		}
	
		//TODO: Lembrar de dar free no buffer apos usar-lo!!
		char * getByteRepresentation()
		{			
			char * buffer = new char[ (4 * nodeCount * sizeof(int)) + 100];
			int pos = 0;			
			insertIntToBuffer( nodeCount, pos, buffer);			
			// DB( nodeCount );
			for(int edgeIndex = 1; edgeIndex < nodeCount; edgeIndex++){
				Edge edge = edgesArr[edgeIndex];
				insertIntToBuffer( edge.l , pos, buffer);
				insertIntToBuffer( edge.r , pos, buffer);
				insertIntToBuffer( edge.noInicial , pos, buffer);
				insertIntToBuffer( edge.noFinal , pos, buffer);
			}
			return buffer;
		}
		
		void constructFromByteRepresentation(char * buffer)
		{
			initialize();			
			int pos = 0;			
			nodeCount = getIntFromBuffer( pos, buffer );
			// DB( nodeCount );
			outEdges = new list<char>[nodeCount + 1];
			for(int edgeIndex = 1; edgeIndex < nodeCount; edgeIndex++){
				Edge edge;
				edge.l = getIntFromBuffer( pos, buffer);
				edge.r = getIntFromBuffer( pos, buffer);
				edge.noInicial = getIntFromBuffer( pos, buffer);
				edge.noFinal = getIntFromBuffer( pos, buffer);
				insertEdge( edge );
				// DB( edge.noInicial _ edge.noFinal _ str[edge.l]);
				outEdges[edge.noInicial].push_back( str[edge.l] );
			}
			
		}
		
		
		//findOccurrences deve ser chamado apos constructFromByteRepresentation. 
		void findOccurrences( const string & patt ){
			
			// printf("procurando ocorrencias de: %s\n", patt.c_str() );
			if( (int) patt.size() == 0 ) return ;
			
			int iteratorPatt = 0;			
			Edge edge;	
			int currentSuffixSize = 0;
			int currentNode = rootNode.index;			
			bool searching = true;			
			while( searching ){				
				bool matching = true;
				if( mapEdges.count( make_pair( currentNode, patt[iteratorPatt] ) ) > 0){
					edge = edgesArr[ mapEdges[make_pair( currentNode, patt[iteratorPatt] )] ];
				} else {
					break;
				}
				for( int iteratorText = edge.l ; iteratorText <= edge.r && iteratorText < stringSize && 
					iteratorPatt < (int) patt.size(); ++iteratorText , ++iteratorPatt){					
					if( patt[iteratorPatt] != str[iteratorText]){						
						matching = false; 
						break;
					}
				}
								
				if( matching ){				
					
					currentSuffixSize += edge.r - edge.l + 1; 
					if( edge.r == stringSize ) currentSuffixSize--; //TODO ta certo isso?					
					if( iteratorPatt == (int) patt.size()){
						// matching exato encontrado						
						findOccurrenceHelper( edge.noFinal, currentSuffixSize  );
						searching = false;
					}
					currentNode = edge.noFinal;
				} else {
					searching = false;
				}
			}
		}
	
	
		void findOccurrenceHelper(int nodeIndex , int currentSuffixSize){			
			if( outEdges[nodeIndex].size() == 0){
				tot++;
				// no folha
				// printf("ocorrencia na posicao: %d\n", stringSize - currentSuffixSize );
				// for(int x = 0; x < 20; x++){
					// cout << str[stringSize - currentSuffixSize + x] ;;
				// }
				// cout << endl;
				
			}
			list<char> & lista = outEdges[nodeIndex];
			for( list<char>::iterator it = lista.begin(); it != lista.end(); it++){
				Edge edge = edgesArr[ mapEdges[make_pair( nodeIndex, *it)] ];
				int span = edge.r - edge.l + 1;				
				if( edge.r == stringSize ) span--;
				findOccurrenceHelper(edge.noFinal, currentSuffixSize + span );
			}			
		}
};

int main(int argc, char** argv)
{
	string str, strbuf; 
	while( getline( cin, strbuf)){
		str += strbuf;
		str += "\n";
	}
	
	// DB( str.size());
    SuffixTree aux(str);
	
	// aux.findOccurrences("for");
	
	char * buff = aux.getByteRepresentation();
	SuffixTree tree( str , buff );
	DB( str.size());
	DB( tree.nodeCount );	
	tree.findOccurrences("it");	
	tree.findOccurrences("to");	
	tree.findOccurrences("the");	
	tree.findOccurrences("indicate");
	DB(tot);
    return 0;
}

