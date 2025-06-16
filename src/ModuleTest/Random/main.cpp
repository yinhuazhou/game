#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Random.h"
#include <algorithm>
#include <cmath>
using namespace std;
using namespace GameLib;
using namespace GameLib::Math;

//测试随机性
//随机放入32*32*32的箱子中，确认是否是随机的。
Random gRandom;

const int N = 1000*1000;
const int M = 32;
const int M3 = M * M * M;
double gK = 0.0;
double gC = 0.0;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync( false );
	}
	void Framework::update(){
		if ( !gRandom ){
			gRandom = Random::create();
		}
		int* b = new int[ M3 ];
		for ( int i = 0; i < M3; ++i ){
			b[ i ] = 0;
		}
		for ( int i = 0; i < N; ++i ){
			int v[ 3 ];
			for ( int j = 0; j < 3; ++j ){
#if 0 //1设为标准rand（），设为0则Math:Random
				v[ j ] = rand();
#else
				v[ j ] = gRandom.getInt();
#endif
				v[ j ] &= 0x1f;
			}
			++b[ ( ( ( v[ 0 ] * M ) + v[ 1 ] ) * M ) + v[ 2 ] ];
		}
		double s = 0.0;
		double a = static_cast< double >( N ) / M3;
		for ( int i = 0; i < M3; ++i ){
			s += ( b[ i ] - a ) * ( b[ i ] - a );
		}
		s /= M3;
		s = std::sqrt( s );
		double sIdeal = std::sqrt( a );
		SAFE_DELETE_ARRAY( b );

		gK += s - sIdeal;
		gC += 1.0;

		//按顺序计算的标准偏差，理想标准偏差，与理想值的差，与理想值的差（累积值）
		DebugScreen sc;
		sc << s << " " << sIdeal << " " << s - sIdeal << " " << gK/gC;
	}
}
