#include "tpf_algorithm.h"
#include "Bitset.h"
#include <assert.h>

/*int tpf_find(char **i_patterns, int i_pattern_amount, char *i_textfile, int i_tpf_type, char **o_results)
{
	// TODO: adicionar distancia de edição como parametro e checar se é maior que o padrão
	int ret = TPF_OK;
	switch (i_tpf_type){
		case TPF_EXACT:
			break;
		case TPF_EXACT_PATTERNFILE:
			ret = tpf_aho_corasick(i_patterns, i_pattern_amount, i_textfile, o_results);
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

int P; // Number of nodes in the prefix tree
char ltt[DOTS]; // Character for this node
vector<int> wend[DOTS]; // Numbers of the words ending here
int adj[DOTS][ALPHABET_SIZE]; // [node][character]: child node (-1 if non-existent)
vector<int> vadj[DOTS]; // Numbers of the child nodes

int dec(char c) { // ``character $\rightarrow$ number'' mapping, for example:
	return c-'a';
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

///ahocorasick
int suf[DOTS], preend[DOTS], dep[DOTS], par[DOTS];

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

vector<pair<int,int>> matches; // All pairs (a,b) such that word number b starts at position a

void search(const char *text) {
	matches.clear();
	int c = 0;
	for (int i = 0; text[i]; i++) {
		while(c != -1 && adj[c][dec(text[i])] == -1)
			c = suf[c];
		c = c == -1 ? 0 : adj[c][dec(text[i])];
		int t = c;
		while(t != -1) {
			for (int w : wend[t])
				matches.push_back(pair<int,int>(i-dep[t]+1, w));
			t = preend[t];
		}
	}
}

int tpf_aho_corasick(char **i_patterns, int i_pattern_amount, char *i_textfile, char **o_results)
{
	
	return TPF_OK;
}*/

// ---- Fim AHO-CORASICK ---- //

// ---- Inicio WU-MANBER ---- //

int char_mask(char *i_pattern, Bitset *C)
{
	int pattern_len = strlen(i_pattern);

	for (int i = 0; i < ALPHABET_SIZE; i++){
		C[i] = Bitset(pattern_len);
		C[i].set();
	}

	Bitset Mk(pattern_len);
	Mk.set(0);

	for (int k = 0; k < pattern_len; k++){
		C[(int) i_pattern[k]] = C[(int) i_pattern[k]] & ~Mk;
		Mk = Mk << 1;
	}

	return TPF_OK;
}

int tpf_wu_manber(char *i_pattern, char *i_textfile, int i_distance)
{
	int text_len = strlen(i_textfile);//TEMPORARIO

	int results = 0;
	int pattern_len = strlen(i_pattern);
	Bitset *C = new Bitset[ALPHABET_SIZE];
	char_mask(i_pattern, C);

	// msb = 2 ** (m-1)
	Bitset msb(pattern_len);
	msb.set(pattern_len-1);

	Bitset *S = new Bitset[i_distance+1];
	S[0] = Bitset(pattern_len);
	S[0].set();

	for (int q = 1; q <= i_distance; q++){
		S[q] = S[q-1] << 1;
	}

	Bitset S1(pattern_len);
	Bitset S2(pattern_len);

	for (int j = 0; j < text_len; j++){
		S1 = S[0];
		S[0] = (S[0] << 1) | C[(int) i_textfile[j]];
		for (int q = 1; q <= i_distance; q++){
			S2 = S[q];
			S[q] = 		((S[q] << 1) | C[(int) i_textfile[j]])
					&	(S1 << 1)
					&	(S[q-1] << 1)
					&	S1;
			S1 = S2;
		}
		if (S[i_distance] < msb){
			cout << "*****	RESULT_MATCH 	*****" << endl ;
			results += 1;

			string str(i_textfile);
			int begin =  max(0, j-pattern_len-i_distance);
			cout << "substring("<<begin<<", "<<j+1<<")" <<endl;
			cout << str.substr(begin, pattern_len+i_distance+1) << endl;
		}
	}

	cout << "Results: " << results << endl<< endl;

	return TPF_OK;
}

// ---- Fim WU-MANBER ---- //

// ---- Inicio BOYER-MOORE ---- //

vector<int> border(char *i_pattern)
{
	int pattern_len = strlen(i_pattern);
	vector<int> B(pattern_len+1);
	fill(B.begin()+1, B.end(), 0);
	B[0] = -1;

	int i = 1;

	while (i < pattern_len){
		int j = 0;
		while(i+j<pattern_len && i_pattern[i+j]==i_pattern[j]){
			j += 1;
			B[i+j] = j;
		}
        i += j - B[j];
	}
	
	cout << "[";
	for (i = 0; i < pattern_len; i++){
		cout << B[i] << ", ";
	}
	cout << B[pattern_len] << "]" << endl;
	return B;
}

vector<int> good_suffix(char *i_pattern)
{
	int pattern_len = strlen(i_pattern);
	vector<int> Pi = border(i_pattern);
	vector<int> PiR = border(i_pattern);
	vector<int> S(pattern_len+1);
	fill(S.begin(), S.end(), pattern_len-Pi[pattern_len]);

	for (int l = 1; l <= pattern_len; l++){
		int j = pattern_len - 1 - PiR[l];
		assert(j<pattern_len && j>=0);
		if (i_pattern[j]!=i_pattern[pattern_len-l-1] && S[j] > l-PiR[l]) {
			S[j] = l-PiR[l];
		}
	}

	cout << "[";
	for (int i = 0; i < pattern_len; i++){
		cout << S[i] << ", ";
	}
	cout << S[pattern_len] << "]" << endl;
	return S;

}

vector<int> bad_char(char *i_pattern)
{
	int pattern_len = strlen(i_pattern);
	vector<int> C(ALPHABET_SIZE);
	fill(C.begin(), C.end(), -1);

	for (int j = 0; j < pattern_len; j++){
		C[(int) i_pattern[j]] = j;
	}

	cout << "[";
	for (int i = 0; i < pattern_len; i++){
		cout << C[i] << ", ";
	}
	cout << C[pattern_len] << "]" << endl;
	return C;
}

int tpf_boyer_moore(char *i_pattern, char *i_textfile, char **o_results)
{
	return TPF_OK;
}

// ---- Fim BOYER-MOORE ---- //


// ignore

int main(){
	
	/*char padrao[9] = "bao";
	char texto[85] = "baobabaobabaobabaobabaobabaoba baoba";
	int distancia = 0;

	tpf_wu_manber(padrao, texto, distancia);
*/
	char padrao[30] = "boraefjsbor";
	vector<int> S = good_suffix(padrao);


	return 0;
}
