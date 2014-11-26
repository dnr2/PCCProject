#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include "tpf_algorithm.h"

using namespace std;

void help()
{
	cout << "Uso: tpf [options] [pattern] textfile [textfile...]" << endl << endl;
	cout << "-h, --help\t\tEste guia de ajuda" << endl;
	cout << "-e, --edit d\t\tFaz uma busca aproximada em uma distancia maxima 'd'" << endl;
	cout << "-p, --pattern file\tUtiliza todos os padroes em 'file' como entrada" << endl;
	cout << "-c, --count\t\tImprime apenas a quantidade de ocorrências encontradas por arquivo" << endl;

	exit(0);
}

vector<string> handle_wildcard(string &str)
{
	vector<string> files;
	files.reserve(5);

	unsigned found = str.find_last_of("/\\");
	bool no_path = found==string::npos;
	string path = no_path ? "./" : str.substr(0,found+1);
	string file = no_path ? str : str.substr(found+1);

	// verifica se tem wildcard
	found = file.find_last_of('*');
	// sem wildcard
	if (found == string::npos){
		ifstream check(str);
		if (check.good())
			files.push_back(str);
		else
			cout << "Arquivo de texto << " << str << " >> inválido" << endl;
		return files;
	}
	// se tiver wildcard, lê até antes do asterisco
	file = file.substr(0, found);

  	// le arquivos do diretorio
	DIR *dp;
	struct dirent *ep;

	dp = opendir (path.c_str());
	if (dp != NULL)	{
		while (ep = readdir (dp)){
			string possible_match(ep->d_name);
			if (possible_match.find(file) != string::npos){
				puts (ep->d_name);
				files.push_back(possible_match);
			}
		}
		(void) closedir (dp);
	} else {
		cout << "Diretório << " << path << " >> inválido" << endl;
	}

	return files;
}

int main(int argc, char **argv)
{
	// make COUT and CIN faster
	ios_base::sync_with_stdio(0);cin.tie(0); 

	bool edit = false;
	bool patternfile = false;
	bool count = false;

	int error = 0;
	vector<string> patterns;
	string pat_file;
	vector<string> textfiles;
	
	
	int c;
	int options = 0;

	//Lê as opções de execução da busca	
	while (1){
		static struct option long_options[] =
		{
          {"help",     no_argument,       0, 'h'},
          {"edit",     required_argument, 0, 'e'},
          {"pattern",  required_argument, 0, 'p'},
          {"count",    no_argument, 	  0, 'c'},
          {0, 0, 0, 0}
        };
        
        int option_index = 0;

        c = getopt_long (argc, argv, "he:p:c", long_options, &option_index); 

        if (c == -1) break;

        switch (c){
        	case 'e':
        		edit = true;
        		error = atoi(optarg);
        		options += 2;
        		break;
        	case 'p':
        		patternfile = true;
        		pat_file = optarg;
        		options += 2;
        		break;
        	case 'c':
        		count = true;
        		options += 1;
        		break;
        	case 'h':
        	case '?':
        	default:
        		help();
        		break;
        }
	}

	if (options >= argc 
		|| argc == 1){
		help();
	}

	// se um pattern file não definido, o próximo parâmetro e pattern
	if (!patternfile){
		options += 1;
		string pat(argv[options]);
		patterns.push_back(pat);
	} else{
		ifstream infile(pat_file);

		if (! infile.good()){
			cout << "Arquivo de padrão << " << pat_file << " >> inválido" << endl;
		}

		string pat;
		while (infile >> pat){
			patterns.push_back(pat);
		}
	}

	if (argc <= options+1){
		cout << "Insira um ou mais arquivos de texto para realizar a busca" << endl;
		help();
	}

	// lê os nomes dos arquivos de texto
	for (int i = options+1; i < argc; i++){
		string str = argv[i];
		vector<string> matches = handle_wildcard(str);
		for(string &match : matches){
			textfiles.push_back(match);
		}
	}


	cout << "COUNT\t" << (count?"TRUE":"FALSE") << endl << endl;

	cout << "EDIT\t" << (edit?"TRUE":"FALSE") << endl;
	cout << "Value:\t" << error << endl << endl;

	cout << "PATTERNFILE\t" << (patternfile?"TRUE":"FALSE") << endl;
	cout << "PADROES:"	 << endl;
	for(string &pat : patterns)
		cout << pat << endl;
	cout << endl;

	cout << "ARQUIVOS DE TEXTO:" << endl;
	for(string &file : textfiles)
		cout << file << endl;
	cout << endl;

	// int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count);
	if (! edit){
		int type = patternfile ? TPF_EXACT_PATTERNFILE : TPF_EXACT;
		for (string &file : textfiles)
			int ret = tpf_find(patterns, file, 0, type, count);
	} else {
		for (string &txt : textfiles)
			int ret = tpf_find(patterns, file, error, TPF_APPROXIMATE, count);
	}

}