#include <iostream>
#include <stack>
#include <queue>
#include <vector>

using namespace std;

struct holi
{
	int a;
	int b;
	int c;	
};

int hayAldeano(const vector<int> &v){
	for(int i=0 ; i<v.size() ; i++)
		if(v[i] == 6)
			return i;

	return -1;
}

int main(){
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(6);
	v.push_back(5);


	if(hayAldeano(v) == -1)
		cout << "no hay na chorra" << endl;
	else
		cout << "hay un aldeano en" << hayAldeano(v) << endl;

	return 0;
}
