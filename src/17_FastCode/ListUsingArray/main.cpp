#include "List.h"
#include <iostream>
using namespace std;

int main(){
	List a( 10 );
	int p0 = a.addTail( 0.0 ); //0
	int p1 = a.addTail( 1.0 ); //0,1
	int p2 = a.addHead( 2.0 ); //2,0,1
	int p3 = a.addBefore( p1, 3.0 ); //2,0,3,1
	int p4 = a.addAfter( p0, 4.0 ); //2,0,4,3,1
	int p5 = a.addBefore( p2, 5.0 ); //5,2,0,4,3,1
	int p6 = a.addAfter( p1, 6.0 ); //5,2,0,4,3,1,6
	a.remove( p3 ); //5,2,0,4,1,6
	a.removeTail(); //5,2,0,4,1
	a.removeHead(); //2,0,4,1

	int p;
	//从头开始
	//2,0,4,1的顺序
	p = a.first();
	while ( p != -1 ){
		cout << a.value( p ) << endl;
		p = a.next( p );
	}

	//从末尾访问
	//1,4,0,2的顺序
	p = a.last();
	while ( p != -1 ){
		cout << a.value( p ) << endl;
		p = a.previous( p );
	}

	return 0;
}