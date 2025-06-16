#include <iostream>
using namespace std;

int main(){
	const int n = 2500 * 10000;
	int* a = new int[ n ];
	//生成适当排序的数据
	for ( int i = 0; i < n; ++i ){
		a[ i ] = i * 5;
	}
	cout << "SEARCH BEGIN" << endl;

	int loopCount = 0;
	//寻找的价值是这样的
	int search = 89012345;
	//
	int first = 0;
	int last = n - 1;
	while ( last > first ){ //1个以上的时候重复
		int middle = ( first + last ) / 2; //中间的下标
		cout << loopCount << "\t" << first << "-" << last << "\tmiddle=" << middle << "\trange=" << last-first << endl;
		++loopCount;
		int t = a[ middle ];
		if ( t < search ){ //
			first = middle + 1; //更换头
		}else if ( t > search ){ //大的在前面
			last = middle - 1; //替换末尾
		}else{
			first = last = middle;
		}
	}
	//此时，first == last。
	if ( a[ first ] == search ){
		cout << "Find! " << first << endl;
	}else{
		cout << "Not Find!" << endl;
	}
	delete[] a;

	//无限循环
	while( true );



	return 0;
}