#include <iostream>
#include <getopt.h>
#include <stdlib.h>

using namespace std;

int dist = 0;
char *pattern;
bool patternfile = false;


void help()
{
	cout << "Uso: tpf [options] [pattern] textfile [textfile...]" << endl << endl;
	cout << "-h, --help\t\tEste guia de ajuda" << endl;
	cout << "-e, --edit d\t\tFaz uma busca aproximada em uma distancia maxima 'd'" << endl;
	cout << "-p, --pattern file\tUtiliza todos os padroes em 'file' como entrada" << endl;

	exit(0);
}

int main(int argc, char **argv)
{

	int c;
	int options = 0;

	//Lê as opções de execução da busca	
	while (1)
	{
		static struct option long_options[] =
        {
          {"help",     no_argument,       0, 'h'},
          {"edit",     required_argument, 0, 'e'},
          {"pattern",  required_argument, 0, 'p'},
          {0, 0, 0, 0}
        };
        
        int option_index = 0;

        c = getopt_long (argc, argv, "he:p:", long_options, &option_index);

        if (c == -1)	break;

        options += 2;

        switch (c)
        {
        	case 'e':
        		dist = atoi(optarg);
        		cout << "distancia igual a " << dist << endl;
        		break;
        	case 'p':
        		patternfile = true;
        		pattern = optarg;
        		cout << "patternfile " << pattern << endl;
        		break;
        	case 'h':
        	case '?':
        	default:
        		help();
        		break;
        }
	}

	if (options >= argc 
		|| argc == 1)
	{
		help();
	}

	if (!patternfile)
	{
		pattern = argv[1];
		options += 1;
		cout << "pattern: " << pattern << endl;
	}

	cout << "Files:" << endl;
	int textFiles = argc - options;
	string files[textFiles];
	for (int i = options+1; i < argc; i++)
	{
		files[i-options-1] = argv[i];
		cout << argv[i] << endl;
	}



}