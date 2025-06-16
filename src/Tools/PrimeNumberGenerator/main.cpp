#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <list>
using namespace std;

int main( int argc, char** argv ){
	string outFileName;
	int m = 0;
	bool help = false;
	//命令行参数解析
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //////////以-开头的是一些参数选项
			char c = argv[ i ][ 1 ];
			if ( c == 'o' && ( ( i + 1 ) < argc ) ){ 
				outFileName = argv[ i + 1 ];
			}else if ( c == 'h' ){ //帮助
				help = true;
			}
		}else{
			istringstream iss( argv[ i ] );
			iss >> m;
			if ( m < 2 ){
				cerr << "MAX_NUMBER must be >= 2.";
				help = true;
			}
		}
	}
	if ( m == 0 ){
		help = true;
	}
	//显示帮助后结束
	if ( help ){
		cout << "Usage: PrimeNumberGenerator.exe [ -h ][ -o outputFile ] MAX_NUMBER" << endl;
		cout << "<Arguments>" << endl;
		cout << "\t-h             : display this help." << endl;
		cout << "\t-o             : specify output file." << endl;
		cout << "\tMAX_NUMBER     : specify maximum number. must be >= 2" << endl;
#ifndef NDEBUG //只有在debug模式下才会执行
	while( true ){;} //为了保证visual studio中执行控制台程序时不会一闪而过
#endif
		return 1;
	}
	ostream* out;
	if ( outFileName.size() > 0 ){
		setlocale( LC_ALL, "" ); //为了确保能够对那些文件名包含中文字符的文件进行处理
		out = new ofstream( outFileName.c_str() );
	}else{
		out = &cout;
	}
	//生成素数
	//1.2和小于max的等级数全部放进表
	list< int > l;
	l.push_back( 2 );
	for ( int i = 3; i <= m; i += 2 ){
		l.push_back( i );
	}
	//2.生成根部maximum
	int n = static_cast< int >( sqrt( static_cast< double >( m ) ) );
	//3处理
	for ( int i = 3; i < n; ++i ){
		list< int >::iterator j = l.begin();
		while ( j != l.end() ){
			list< int >::iterator next = j;
			++next;
			if ( *j != i ){
				if ( ( *j % i ) == 0 ){ 
					l.erase( j );
				}
			}
			j = next;
		}
	}
	//4出口
	*out << "const int gPrimeNumbers[] = {" << endl;
	list< int >::iterator i = l.begin();
	int j = 0;
	while ( i != l.end() ){
		if ( ( j % 8 ) == 0 ){
			*out << "\t";
		}
		*out << *i << ",\t";
		if ( ( j % 8 ) == 7 ){
			*out << endl;
		}
		++i;
		++j;
	}
	if ( ( j % 8 ) != 7 ){
		*out << endl;
	}
	*out << "};" << endl;
	*out << "const int gPrimeNumberTableSize = " << l.size() << ";" << endl;
	*out << "const int gPrimeNumberTableMaximum = " << m << ";" << endl;
#ifndef NDEBUG //只有在debug模式下才会执行
	if ( outFileName.size() == 0 ){
		while( true ){;} //为了保证visual studio中执行控制台程序时不会一闪而过
	}
#endif
	return 0;
}
