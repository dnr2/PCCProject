#include "ipmt_algorithm.h"
#include "SuffixTree.h"
#include "SuffixArray.h"

#include <assert.h>

using namespace std;

int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count)
{
	int ret = TPF_OK;

	if (tpf_type == TPF_EXACT){ // Boyer Moore
	// int tpf_boyer_moore(string &pat, string &textfile, bool count)
		string pat = patterns[0];

		tpf_boyer_moore(pat, textfile, count);

	} else if (tpf_type == TPF_EXACT_PATTERNFILE){ // Aho-Corasick
		
		tpf_aho_corasick(patterns, textfile, count);
		
	} else if (tpf_type == TPF_APPROXIMATE){ // Wu-Manber
	// int tpf_wu_manber(string &pat, string &textfile, int error, bool count)		
		
		for (string &pat : patterns){		
			int cur_error = min( error, (int) pat.length());
			if( (error < 10 && pat.length() < 63 ) || (error < 2 && pat.length() > 100) ) 
			// if( (error < 2 && pat.length() > 100) ) 
				tpf_wu_manber(pat, textfile, cur_error, count);
			else 
				tpf_sellers(pat, textfile, cur_error, count);			
		}

	} else {
		ret = TPF_ERR_NO_SUCH_OPTION;
	}

	return ret;
}


// ---- Inicio AHO-CORASICK ---- //

bool aho_corasick_search(string txt, TreeAhoCorasick & tree, vector<string> & pats, bool print_occ, vector<int> & occ) {	
	int c = 0;	
	bool found = false;
	for (int i = 0; txt[i]; i++) {
		while(c != -1 && tree.adj[c][(int)txt[i]] == -1)
			c = tree.suf[c];
		c = c == -1 ? 0 : tree.adj[c][(int)txt[i]];
		int t = c;
		while(t != -1) {
			for (int w : tree.word_end[t]){				
				// printf("palavra %d aparece na posicao %d\n", w, i-(tree.dep[t])+1);
				if( print_occ ) cout << "'" << pats[w] << "', ";
				found = true;
				occ[w]++;
			}
			t = tree.preend[t];
		}
	}
	return found;
}

int tpf_aho_corasick(vector<string> & pats, string &textfile, bool count)
{
	
	ifstream istream(textfile);

	if (! istream.good()){
		return TPF_ERROR_READING_FILE;
	}
	string line;
	
	//inicializar Aho-Corasick
	TreeAhoCorasick tree;
	tree.init();		
	for( int i =0; i < (int) pats.size(); i++){
		tree.add( pats[i], i );
	}
	
	tree.complete_tree();	
	
	vector<int> occ = vector<int>( (int) pats.size());
	
	while ( getline(istream , line) ){
	
		if(!count){
			if( aho_corasick_search( line , tree, pats, true, occ) > 0 ){
				cout << "found at:" << endl;
				cout << textfile << ":" << line << endl;
			}
		} else {
			aho_corasick_search( line , tree, pats, false, occ);
		}
	}
	if (count){
		for( int i =0; i < (int)pats.size();i++){
			cout << "padrao:" << pats[i] << endl;
			cout << textfile << ":" << occ[i] << endl;
		}
	}
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

int optimized_wu_manber(string &pat, string &textfile, int error, bool count);

int tpf_wu_manber(string &pat, string &textfile, int error, bool count)
{
	
	if( pat.length() < 63 ){
		return optimized_wu_manber(pat, textfile, error, count);
	}
	int occ = 0;
	ifstream istream(textfile);

	if (! istream.good()){
		return TPF_ERROR_READING_FILE;
	}
	string line;

	cout << "padrao:" << pat << endl;

	
	//inicializar bitset 
	
	int m = pat.length();
	Bitset *C = new Bitset[ALPHABET_SIZE];
	char_mask(pat, C);
	
	Bitset *S = new Bitset[error+1];
	
	// msb = 2 ** (m-1)
	Bitset msb(m);
	msb.set(m-1);
	
	Bitset S1(m);
	Bitset S2(m);
	
	
	while ( getline(istream, line) ){
		
		S[0] = Bitset(m);
		S[0].set();

		for (int q = 1; q <= error; q++){
			S[q] = S[q-1] << 1;
		}

		int n = line.length();
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
			//cerr  << S[error].bits[0] << " " << msb.bits[0] << endl;
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



// optimized wu mamber using long long instead of bitset

typedef unsigned long long ulong;

int optimized_char_mask(string &pat, ulong *C)
{
	int m = pat.length();

	for (int i = 0; i < ALPHABET_SIZE; i++){
		C[i] = ~0ULL;
	}

	ulong Mk = 1ULL;

	for (int k = 0; k < m; k++){
		C[(int) pat[k]] = C[(int) pat[k]] & ~Mk;
		Mk = Mk << 1;
	}

	return TPF_OK;
}

int optimized_wu_manber(string &pat, string &textfile, int error, bool count)
{
	
	int occ = 0;
	ifstream istream(textfile);

	if (! istream.good()){
		return TPF_ERROR_READING_FILE;
	}
	string line;

	cout << "padrao:" << pat << endl;

	
	//inicializar bitset 
	
	int m = pat.length();
	ulong *C = new ulong[ALPHABET_SIZE];
	optimized_char_mask(pat, C);
	
	ulong *S = new ulong[error+1];
	
	// msb = 2 ** (m-1)
	ulong msb = (~0ULL) << (m-1);	
	
	ulong S1 = 0ULL;
	ulong S2 = 0ULL;
	
	
	while ( getline(istream, line) ){
		
		S[0] = ~0ULL;		

		for (int q = 1; q <= error; q++){
			S[q] = S[q-1] << 1;
		}

		int n = line.length();
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
			//cerr << S[error] << " " << msb << endl;
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

// ---- Inicio SELLERS ---- //


int tpf_sellers(string &pat, string &textfile, int error, bool count)
{
	int m = pat.length();
	int occ = 0;
	ifstream istream(textfile);

	if (! istream.good()){
		return TPF_ERROR_READING_FILE;
	}
	string line;

	cout << "padrao:" << pat << endl;
	
	while ( getline(istream, line) ){
		vector< vector<int> > D;		
		
		bool printed = false;
	
		int n = line.length();
		
		for( int i = 0; i <= m+1; i++){
			D.push_back( vector<int>( 2 ));
		}
		
		for( int i =0; i <=m; i++) 
			D[i][0] = i;
		for( int j =0; j < 2; j++)
			D[0][j] = 0;

		for( int j =1; j <=n; j++){
		
			int cur_j = j % 2; // posicao atual da matriz
			int ant_j = cur_j ^ 1; // posicao anterior da matriz
			
			for( int i = 1; i<=m; i++){
				
				D[i][cur_j] = min( D[i-1][ant_j] + ((pat[i-1] == line[j-1])? 0 : 1), 
					min( D[i][ant_j] + 1, D[i-1][cur_j] + 1 ));
			}
			if( D[m][cur_j] <= error ){
				occ++;
				if( !count && !printed ){
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
// ---- Fim SELLERS ---- //

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

	while ( getline(istream, line) ){
		int n = line.length();
		int i = 0;
		bool printed = false;

		while (i <= n-m){
			int j = m-1;
			while (j >= 0 && line[i+j]==pat[j]){
				j -= 1;
			}
			// line[i+j]==pat[j] para o padrão todo! deu match!
			if (j < 0){
				occ += 1;
				i += S[0];
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
