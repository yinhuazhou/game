#include <algorithm> //对于random_shuffle（）
#include <iostream>
#include <ctime> //
using namespace std;

int main( int argc, char** ){
	cout << "PREPARING... " << endl;

	const int N = 1000 * 1000 * 20; //10MB分钟
	int* a = new int[ N ]; 
	int** p0 = new int*[ N ];
	int** p1 = new int*[ N ];
	for ( int i = 0; i < N; ++i ){
		a[ i ] = i;
		p0[ i ] = &a[ i ];
		p1[ i ] = &a[ i ];
	}
	//将p1分开
	random_shuffle( p1, p1 + N );
	//请参阅argc并选择要使用的指针数组
	int** p = ( argc == 1 ) ? p0 : p1;

	double t0 = clock();	//时间起点
	//100次访问
	for ( int j = 0; j < 100; ++j ){
		for ( int i = 0; i < N; ++i ){
			++( *p[ i ] );
		}
		cout << j << endl;
	}
	double t1 = clock(); //时间终点

	cout << "TIME: " << ( t1 - t0 ) / CLOCKS_PER_SEC << endl; //计算秒数

	while( true ); //无限循环

	return 0;
}
