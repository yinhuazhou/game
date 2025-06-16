#include <iostream>
#include <cstdlib> //rand(),atoi()
#include <limits>
#include <ctime> //clock()
using namespace std;

//位置结构体
struct Position{
	double x;
	double y;
};

//递归搜索主体
void search( 
int* minPath, 
int* path, 
int cn, 
int n, 
const Position* p, 
double currentLength, 
const Position& currentPos );

//最短距离
double gMinLength = numeric_limits< double >::max(); //用大数字初始化

int main( int argc, char** argv ){
	if ( argc < 2 ){
		return 1; //错误
	}
	int n = atoi( argv[ 1 ] );

	//初始化家的位置
	Position* positions = new Position[ n ];
	for ( int i = 0; i < n; ++i ){
		positions[ i ].x = rand();
		positions[ i ].y = rand();
		cout << i << " : " << positions[ i ].x << "," << positions[ i ].y << endl;
	}
	//开始递归调用
	int* minPath = new int[ n ];
	int* path = new int[ n ];
	Position currentPos;
	currentPos.x = currentPos.y = 0.0; //初始位置
	double t0 = clock(); //开始时间
	search( minPath, path, 0, n, positions, 0.0, currentPos );
	double t1 = clock(); //结束时间

	cout << "Length : " << gMinLength << endl;
	for ( int i = 0; i < n; ++i ){
		cout << i << " " << minPath[ i ] << endl;
	}
	cout << "Calculation Time : " << ( t1 - t0 ) / CLOCKS_PER_SEC << endl;
	while(1);
	return 0;
}

void search( 
int* minPath, 
int* path, 
int cn, 
int n, 
const Position* p, 
double currentLength, 
const Position& currentPos ){
	for ( int i = 0; i < n; ++i ){
		//
		bool found = false;
		for ( int j = 0; j < cn; ++j ){
			if ( path[ j ] == i ){
				found = true;
				break;
			}
		}
		if ( !found ){ //
			path[ cn ] = i;
			double dx = currentPos.x - p[ i ].x;
			double dy = currentPos.y - p[ i ].y;
			double l = currentLength + sqrt( dx * dx + dy * dy );
			if ( cn == n - 1 ){ //
				if ( l < gMinLength ){
					gMinLength = l;
					//保存路线
					for ( int j = 0; j < n; ++j ){
						minPath[ j ] = path[ j ];
					}
					//显示当前解
					cout << l << " : ";
					for ( int j = 0; j < n; ++j ){
						cout << path[ j ] << ',';
					}
					cout << endl;
				}
			}else{
				search( minPath, path, cn + 1, n, p, l, p[ i ] );
			}
		}
	}
}
