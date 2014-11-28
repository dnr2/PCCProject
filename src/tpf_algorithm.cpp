#include "tpf_algorithm.h"
#include "Bitset.h"

// ---- defini??o de fun??es ---- //

int tpf_aho_corasick(char **pats, int pat_amount, char *txt, char **o_results);

// ---- fim de defini??o ---- //

int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count)
{
	// TODO: adicionar distancia de edição como parametro e checar se é maior que o padrão
	int ret = TPF_OK;

	for (string &pat : patterns){
		if (pat.length() < error){
			ret = TPF_ERROR_TOO_LONG;
			return ret;
		}
	}

	if (tpf_type == TPF_EXACT){ // Boyer Moore
	// int tpf_boyer_moore(string &pat, string &textfile, bool count)
		string pat = patterns[0];

		tpf_boyer_moore(pat, textfile, count);

	} else if (tpf_type == TPF_EXACT_PATTERNFILE){ // Aho-Corasick

	} else if (tpf_type == TPF_APPROXIMATE){ // Wu-Manber
	// int tpf_wu_manber(string &pat, string &textfile, int error, bool count)

		for (string &pat : patterns){
			tpf_wu_manber(pat, textfile, error, count);
		}

	} else {
		ret = TPF_ERR_NO_SUCH_OPTION;
	}

	return ret;
}


// ---- Inicio AHO-CORASICK ---- //



#define MAXNODES 1000

int P; // Number of nodes in the prefix tree
char ltt[MAXNODES]; // Character for this node
vector<int> wend[MAXNODES]; // Numbers of the words ending here
int adj[MAXNODES][ALPHABET_SIZE]; // [node][character]: child node (-1 if non-existent)
vector<int> vadj[MAXNODES]; // Numbers of the child nodes

int dec(char c) { // "character -> number", change if the alphabet is alpha numeric
	return c;
}

void init() {
	wend[0].clear();
	fill_n(adj[0], ALPHABET_SIZE, -1);
	vadj[0].clear();
	P = 1;
}

// Add a word to the prefix tree and give it the number w
void add(const char *wort, int w) {
	int c = 0;
	for (int i = 0; wort[i]; i++) {
		int &cs = adj[c][dec(wort[i])]; // Don't forget the ``\verb|&|''
		if (cs == -1) {
			cs = P;
			vadj[c].push_back(P);
			ltt[P] = wort[i];
			wend[P].clear();
			fill_n(adj[P], ALPHABET_SIZE, -1);
			vadj[P].clear();
			P++;
		}
		c = cs;
	}
	wend[c].push_back(w);
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
			while(s != -1 && adj[s][dec(ltt[i])] == -1)
				s = suf[s];
			suf[i] = s == -1 ? 0 : adj[s][dec(ltt[i])];
			preend[i] = wend[suf[i]].size() ? suf[i] : preend[suf[i]];
		}
		for (int k : vadj[i]) {
			par[k] = i;
			dep[k] = dep[i]+1;
			qu.push(k);
		}
	}
}

vector< pair< int,int > > matches; // All pairs (a,b) such that word number b starts at position a

void search(const char *text) {
	matches.clear();
	int c = 0;
	for (int i = 0; text[i]; i++) {
		while(c != -1 && adj[c][dec(text[i])] == -1)
			c = suf[c];
		c = c == -1 ? 0 : adj[c][dec(text[i])];
		int t = c;
		while(t != -1) {
			for (int w : wend[t]){
				matches.push_back(pair<int,int>(i-dep[t]+1, w));
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

int tpf_wu_manber(string &pat, string &textfile, int error, bool count)
{
	int m = pat.length();
	Bitset *C = new Bitset[ALPHABET_SIZE];
	char_mask(pat, C);

	int occ = 0;

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

	ifstream istream(textfile);

	if (! istream.good()){
		return TPF_ERROR_READING_FILE;
	}
	string line;

	cout << "pattern:" << pat << endl;

	while (istream >> line){
		int n = line.length();
		int i = 0;
		bool printed = false;

		for (int j = 0; j < n; j++){
			S1 = S[0];
			S[0] = (S[0] << 1) | C[(int) line[j]];
			for (int q = 1; q <= error; q++){
				S2 = S[q];
				S[q] = 		((S[q] << 1) | C[(int) line[j]])
						&	(S1 << 1)
						&	(S[q-1] << 1)
						&	S1;
				S1 = S2;
			}
			if (S[error] < msb){
				occ += 1;
				
				if (!printed && !count){
					cout << textfile << ":" << line << endl;
					printed = true;
				}
			}
		}
	
	}

	if (count){
		cout << textfile << ":" << occ << endl;
	}

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
	return C;
}

int tpf_boyer_moore(string &pat, string &textfile, bool count)
{
	int m = pat.length();
	vector<int> C = bad_char(pat);
	vector<int> S = good_suffix(pat);
	int occ = 0;
	
	ifstream istream(textfile);

	if (! istream.good()){
		return TPF_ERROR_READING_FILE;
	}
	string line;

	while (istream >> line){
		int n = line.length();
		int i = 0;
		bool printed = false;

		while (i <= n-m){
			int j = m-1;
			while (j >= 0 && line[i+j]==pat[j]){
				j -= 1;
			}
			if (j < 0){
				occ += 1;
				i += S[m+j+1];
				if (!printed && !count){
					cout << textfile << ":" << line << endl;
					printed = true;
				}
			}
			else{
				i += max(j-C[(int) line[i+j]], S[j]);
			}
		}
	
	}

	if (count){
		cout << textfile << ":" << occ << endl;
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
