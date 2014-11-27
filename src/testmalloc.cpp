#include<cstdio>
#include<iostream>
#include<cstdlib>

using namespace std;

typedef long long ll;

int main(){
	ll mb = 1LL<<20;
	ll hi = ((1LL) << 14) * mb; // 16 GB
	ll lo = ((1LL) << 4) * mb; // 16 MB
	cerr << (hi/mb) << " " << (lo/mb) << endl;
	while( lo + 1 < hi){
		ll mid = (hi+lo)/2;
		cerr << (mid/mb) << endl;
		char * s = (char * ) malloc( mid );
		if( s == 0){
			hi = mid;
		} else {
			lo = mid;
		}
		free( s );
	}
	cout << (lo/mb) << endl;
	return 0;
}