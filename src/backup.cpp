#include "ipmt_algorithm.h"


#define WINDOW_SIZE		4095
#define BUFFER_SIZE		127

using namespace std;

void intToHex( int val, char * buffer)
{
	sprintf(buffer, "%3x", val);
	//cout << val << '\t' << buffer << endl;
}

int hexToInt(char * buffer)
{			
	int ret = 0;

	sscanf(buffer, "%x", &ret);
	return ret;
}

string tuple_encode(int pos, int size, char lit)
{
    ostringstream oss;
    char str[4];
    intToHex(pos, str);
    oss << str;
    intToHex(size, str);
    oss << str;
    oss << lit << '$';

	return oss.str();
}

int lz77_encode(string &text, string &ret)
{
	ret = "";

	uint i = 0;

	string window, buffer;
	int f_pos, f_size;
	char f_lit;

	while (i < text.length()){
		int begin = (i > WINDOW_SIZE)? i-WINDOW_SIZE : 0;

		window = text.substr(begin, i-begin);
		buffer = text.substr(i, BUFFER_SIZE);

		f_pos = 0;
		f_size = 0;
		f_lit = text[i];

		for (int size = buffer.length(); size>0; size--){
			int index = window.rfind(buffer.substr(0, size));
			if (index >= 0){
				char lit = '\0';
				if (i + size < text.length()){
					lit = text[i+size];
				}
				f_pos = index;
				f_size = size;
				f_lit = lit;
				break;
			}
		}
		i = i + f_size + 1;
		ret += tuple_encode(f_pos, f_size, f_lit);
		//cout << i <<'\t' << text.length()<< endl;
	}

	return 0;
}

int lz77_decode(string &encoded, string &ret)
{
	ret = "";
	int pos = 0;
	int size = 0;
	char lit;


	char *pch = strtok(const_cast<char*>(encoded.c_str()), "$");

	while (pch != NULL){
		//cout << pch <<endl;
		
		char buffer[4];
		buffer[0] = pch[0];
		buffer[1] = pch[1];
		buffer[2] = pch[2];
		buffer[3] = '\0';
		pos = hexToInt(buffer);

		if (ret.length() > WINDOW_SIZE){
			pos = ret.length() - WINDOW_SIZE + pos;
		}
		pch = pch +3;
		buffer[0] = pch[0];
		buffer[1] = pch[1];
		buffer[2] = pch[2];
		buffer[3] = '\0';

		size = hexToInt(buffer);
		lit = pch[3];

		ret += ret.substr(pos, size);
		ret += lit;

		pch = strtok(NULL, "$");
	}

	return 0;
}

int main(int argc, char** argv) {
	string str, strbuf; 
	while( getline( cin, strbuf)){
		str += strbuf;
		str += "\n";
	}

	string encoded, decoded;
	lz77_encode(str, encoded);
	
	//cout << "Encoded\t" << encoded << endl;
	lz77_decode(encoded, decoded);
	
	//cout << "Decoded\t" << decoded << endl;

	cout << str.length() << endl;
	cout << decoded.length() << endl;
	cout << encoded.length() << endl;
	
	return 0;
}

/*int main(int argc, char** argv)
{
	string str, strbuf; 
	while( getline( cin, strbuf)){
		str += strbuf;
		str += "\n";
	}
	
	// DB( str.size());
    SuffixTree aux(str);
	
	// aux.findOccurrences("for");
	string buff, buff_encoded;
	aux.getByteRepresentation(buff);

	//lz77_encode(buff, buff_encoded);
	DB(buff.length());
	//DB(buff_encoded.length());

	SuffixTree tree( str , buff );
	DB( str.size());
	DB( tree.nodeCount );	
	//tree.findOccurrences("it");	
	//tree.findOccurrences("to");	
	//tree.findOccurrences("the");	
	//tree.findOccurrences("indicate");

    return 0;
}*/