#include "ipmt_algorithm.h"

void help()
{
	cout << "Uso: ipmt search/index [opcoes] textfile" << endl << endl;
	cout << "-h, --help\tEste guia de ajuda" << endl;
	cout << "-p, --patternfile\tIndica um arquivo com padrões para a busca" << endl;
	cout << "-a, --array \tIndexa o arquivo textfile utilizando estrutura de array de sufixos" << endl;	
	exit(0);
}

int main(int argc, char **argv)
{
	// make COUT and CIN faster
	ios_base::sync_with_stdio(0);cin.tie(0); 

	bool patternfile = false;
	bool array = false;
	bool search;

	int error = 0;
	vector<string> patterns;
	string pat_file;
	vector<string> textfiles;
	
	
	int c;
	int options = 1;

	if (argc < 3)
		help();

	if (! strcmp(argv[options], "search")){
		search = true;
	} else if (! strcmp(argv[options], "index")){
		search = false;
	} else{
		help();
	}

	//Lê as opções de execução da busca	
	while (1){
		static struct option long_options[] =
		{
          {"help",     no_argument,       0, 'h'},
          {"pattern",  required_argument, 0, 'p'},
          {"array",    	no_argument, 	  0, 'a'},
          {0, 0, 0, 0}
        };
        
        int option_index = 0;

        c = getopt_long (argc, argv, "hp:a", long_options, &option_index); 

        if (c == -1) break;

        switch (c){
        	case 'p':
        		patternfile = true;
        		pat_file = optarg;
        		options += 2;
        		break;
        	case 'a':
        		array = true;
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

	if (search){
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
	}
	

	// lê os nomes dos arquivos de texto
	for (int i = options+1; i < argc; i++){
		string str = argv[i];
		vector<string> matches = handle_wildcard(str);
		for(string &match : matches){
			textfiles.push_back(match);
		}
	}

	
/*	cout << "COUNT\t" << (count?"TRUE":"FALSE") << endl << endl;

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
	cout << endl;*/

	// int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count);
	if (search){
		for (string &file : textfiles)
			ipmt_search(patterns, file);
	} else {
		for (string &file : textfiles){
			if (array){
				ipmt_index_array(file);
			} else{
				ipmt_index_tree(file);
			}
		}
	}
	exit(0);
	return 0;
}