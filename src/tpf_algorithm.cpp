#include "tpf_algorithm.h"
#include "Bitset.h"
#include <assert.h>

// ---- definir a funcoes ---- //

int tpf_aho_corasick(char **pats, int pat_amount, char *txt, char **o_results);

// ---- fim de definicao ---- //


int tpf_find(char **pats, int pat_amount, char *txt, int i_tpf_type, char **o_results)
{
	// TODO: adicionar distancia de edição como parametro e checar se é maior que o padrão
	int ret = TPF_OK;
	switch (i_tpf_type){
		case TPF_EXACT:
			break;
		case TPF_EXACT_PATTERNFILE:
			ret = tpf_aho_corasick(pats, pat_amount, txt, o_results);
			break;
		case TPF_APPROXIMATE:
			break;
		default:
			return TPF_ERR_NO_SUCH_OPTION;
			break;
	}
	return ret;
}


// ---- Inicio AHO-CORASICK ---- //

using namespace std;

#define MAXNODES 1000 
#define INITIAL_MAX_NODES 1000 

int P; // Numero de nos na arvore de prefixo 
vector<char> ltt; // caractere para este no
vector<int> word_end[MAXNODES]; // Numero das palavras terminando aqui
vector<int> adj[ALPHABET_SIZE]; // lista de adjacencia: [no][charactere] = no filho (-1 se nao-existente)
vector<int> vadj[MAXNODES]; // Numero dos nos filhos

int dec(char c) { // mapa "charactere -> numero", mudar se o alfabeto for alfanumerico
	return c;
}

void init() {
	word_end[0].clear();
	
	for( int i = 0; i < ALPHABET_SIZE; i++){
		adj[i].reserve( INITIAL_MAX_NODES );
		adj[i].clear();
		adj[i].push_back(-1);
	}	
	
	vadj[0].clear();
	P = 1;
	
	ltt.reserve( INITIAL_MAX_NODES );
	ltt.clear();
}

// Adicionar a palavra no arvore de prefixos e associa a ela o numero w
void add(const char *word, int w){
	int c = 0;
	for (int i = 0; word[i]; i++) {
		int &cs = adj[dec(word[i])][c]; // Nao esquecer o '&' para referencia
		if (cs == -1){
			cs = P;
			vadj[c].push_back(P);
			ltt.push_back(word[i]);
			word_end[P].clear();
			for( int x = 0; x < ALPHABET_SIZE; x++){
				adj[x].push_back( -1 );
			}			
			vadj[P].clear();
			P++;
		}
		c = cs;
	}
	word_end[c].push_back(w);
}

///aho-corasick
int suf[MAXNODES], preend[MAXNODES], dep[MAXNODES], par[MAXNODES];

void initaho() {
	queue<int> qu;
	qu.push(0);
	par[0] = -1;
	dep[0] = 0;
	while(!qu.empty()) {
		int i = qu.front();
		qu.pop();
		if (i == 0)
			suf[i] = preend[i] = -1;
		else {
			int s = suf[par[i]];
			while(s != -1 && adj[dec(ltt[i])][s] == -1)
				s = suf[s];
			suf[i] = s == -1 ? 0 : adj[dec(ltt[i])][s];
			preend[i] = word_end[suf[i]].size() ? suf[i] : preend[suf[i]];
		}
		for (int k : vadj[i]) {
			par[k] = i;
			dep[k] = dep[i]+1;
			qu.push(k);
		}
	}
}


void search(const char *text) {	
	int c = 0;
	for (int i = 0; text[i]; i++) {
		while(c != -1 && adj[dec(text[i])][c] == -1)
			c = suf[c];
		c = c == -1 ? 0 : adj[dec(text[i])][c];
		int t = c;
		while(t != -1) {
			for (int w : word_end[t]){				
				printf("palavra numero %d aparece na posicao %d\n", w, i-dep[t]+1);
			}
			t = preend[t];
		}
	}
}

int tpf_aho_corasick(char **pats, int pat_amount, char *txt, char **o_results)
{
	
	
	return TPF_OK;
}


// ---- Fim AHO-CORASICK ---- //

// ---- Inicio WU-MANBER ---- //

int char_mask(string &pat, Bitset *C)
{
	int m = pat.length();

	for (int i = 0; i < ALPHABET_SIZE; i++){
		C[i] = Bitset(m);
		C[i].set();
	}

	Bitset Mk(m);
	Mk.set(0);

	for (int k = 0; k < m; k++){
		C[(int) pat[k]] = C[(int) pat[k]] & ~Mk;
		Mk = Mk << 1;
	}

	return TPF_OK;
}

int tpf_wu_manber(string &pat, char *txt, int error)
{
	int text_len = strlen(txt);//TEMPORARIO

	int results = 0;
	int m = pat.length();
	Bitset *C = new Bitset[ALPHABET_SIZE];
	char_mask(pat, C);

	// msb = 2 ** (m-1)
	Bitset msb(m);
	msb.set(m-1);

	Bitset *S = new Bitset[error+1];
	S[0] = Bitset(m);
	S[0].set();

	for (int q = 1; q <= error; q++){
		S[q] = S[q-1] << 1;
	}

	Bitset S1(m);
	Bitset S2(m);

	for (int j = 0; j < text_len; j++){
		S1 = S[0];
		S[0] = (S[0] << 1) | C[(int) txt[j]];
		for (int q = 1; q <= error; q++){
			S2 = S[q];
			S[q] = 		((S[q] << 1) | C[(int) txt[j]])
					&	(S1 << 1)
					&	(S[q-1] << 1)
					&	S1;
			S1 = S2;
		}
		if (S[error] < msb){
			cout << "*****	RESULT_MATCH 	*****" << endl ;
			results += 1;

			string str(txt);
			int begin =  max(0, j-m-error);
			cout << "substring("<<begin<<", "<<j+1<<")" <<endl;
			cout << str.substr(begin, m+error+1) << endl;
		}
	}

	cout << "Results: " << results << endl<< endl;

	return TPF_OK;
}

// ---- Fim WU-MANBER ---- //

// ---- Inicio BOYER-MOORE ---- //

vector<int> border(string &pat)
{
	int m = pat.length();
	vector<int> B(m+1);
	fill(B.begin()+1, B.end(), 0);
	B[0] = -1;

	int i = 1;

	while (i < m){
		int j = 0;
		while(i+j<m && pat[i+j]==pat[j]){
			j += 1;
			B[i+j] = j;
		}
        i += j - B[j];
	}

	cout << "[";
	for (i = 0; i < m; i++){
		cout << B[i] << ", ";
	}
	cout << B[m] << "]" << endl;
	return B;
}

vector<int> good_suffix(string &pat)
{
	int m = pat.length();
	vector<int> Pi = border(pat);
	vector<int> PiR = border(pat);
	vector<int> S(m+1);
	fill(S.begin(), S.end(), m-Pi[m]);

	for (int l = 1; l <= m; l++){
		int j = m - 1 - PiR[l];
		assert(j<m && j>=0);
		if (pat[j]!=pat[m-l-1] && S[j] > l-PiR[l]) {
			S[j] = l-PiR[l];
		}
	}

	cout << "[";
	for (int i = 0; i < m; i++){
		cout << S[i] << ", ";
	}
	cout << S[m] << "]" << endl;
	return S;

}

vector<int> bad_char(string &pat)
{
	int m = pat.length();
	vector<int> C(ALPHABET_SIZE);
	fill(C.begin(), C.end(), -1);

	for (int j = 0; j < m; j++){
		C[(int) pat[j]] = j;
	}

	cout << "[";
	for (int i = 0; i < m; i++){
		cout << C[i] << ", ";
	}
	cout << C[m] << "]" << endl;
	return C;
}

int tpf_boyer_moore(string &pat, char *txt)
{
	int m = pat.length();
	int n = strlen(txt);
	vector<int> C = bad_char(pat);
	vector<int> S = good_suffix(pat);
	int occ = 0;
	int i = 0;

	while (i <= n-m){
		int j = m-1;
		while (j >= 0 && txt[i+j]==pat[j]){
			j -= 1;
		}
		if (j < 0){
			occ += 1;
			string str(txt);
			cout << "substring("<<i<<", "<<i+m<<")" <<endl;
			cout << str.substr(i, m) << endl;
			i += S[m+j+1];
		}
		else{
			i += max(j-C[(int) txt[i+j]], S[j]);
		}
	}
	return TPF_OK;
}

// ---- Fim BOYER-MOORE ---- //


// ============ ignore ============ // 


//TESTE AHO-CORASICK



int i_pattern_amount;
char i_patterns[100][100];
char i_text[1000];
char str_aux[1000];


void teste_aho_corasick()
{	
	
	while( gets( str_aux ) ){
		if( str_aux[0] == '%' && str_aux[1] == '%') break;
		strcat(i_text, str_aux);
	}
	
	int pos =0, sz = strlen( i_text);
	while( pos < sz){
		printf("pos %3d - ", pos );
		for(int i = 0; i < 25; i++){
			printf("%c", i_text[pos++]);
		}
		cout << endl;
	}
	
	i_pattern_amount = 0;
	
	while( cin >> i_patterns[i_pattern_amount] ){
		printf("%d - %s\n", i_pattern_amount + 1, i_patterns[i_pattern_amount] );
		i_pattern_amount++;
	}
	
	init();
	for( int i =0; i < i_pattern_amount; i++){
		add( i_patterns[i], i+1 );
	}
	initaho();
	search( i_text );	
}

//TESTE BOYER-MOORE

void teste_boyer_moore()
{
			
	string padrao("bao");
	char texto[85] = "abaobabaobab";
	//int distancia = 0;
	
	
	tpf_boyer_moore(padrao, texto);
	
	/*
	char padrao[30] = "boraefjsbor";
	vector<int> S = good_suffix(padrao);
	*/
}

int main(){

	teste_aho_corasick();
	
	return 0;
}
