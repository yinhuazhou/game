#include "GameLib/Framework.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib;

PseudoXml::Document gDocument;
int gCount;


namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){
		setFrameRate( 60 );
		if ( !gDocument ){
			//从文件生成
			gDocument = PseudoXml::Document::create( "data2.txt" );

			//1试着做出来吐出来。
			using namespace PseudoXml;
			Document doc = Document::create();
			Element root = doc.root();
			//在根上添加一个
			root.setChildNumber( 1 );
			Element cameraData = root.child( 0 );
			cameraData.setName( "Data" );
			//插入两个数据
			cameraData.setChildNumber( 2 );

			Element e;
			//第一个
			e = cameraData.child( 0 );
			e.setName( "SomeElement0" );
			e.setAttributeNumber( 3 );
			e.attribute( 0 ).set( "name", "value" ); //字串值
			e.attribute( 1 ).set( "intValue", 1 ); //int値
			e.attribute( 2 ).set( "floatValue", 2.32f ); //float值
			//第二个
			e = cameraData.child( 1 );
			e.setName( "SomeElement1" );
			e.setAttributeNumber( 2 );
			int ia[ 5 ];
			for ( int i = 0; i < 5; ++i ){
				ia[ i ] = i;
			}
			e.attribute( 0 ).set( "intArray", ia, 5 ); //整数数组
			float fa[ 3 ];
			for ( int i = 0; i < 3; ++i ){
				fa[ i ] = 1.f / static_cast< float >( i + 1 );
			}
			e.attribute( 1 ).set( "floatArray", fa, 3 ); //float数组
			//cout输出
			string s;
			doc.convertToString( &s );
			cout << s.c_str() << endl;
			//写入文件
			doc.write( "dataCreated.txt", false );
		}else if ( gDocument.isReady() ){ //阅读结束
			//将其转换为字符串并通过cout输出。
			string s;
			gDocument.convertToString( &s );
			cout << s.c_str() << endl;
			//尝试输出到文件。看看是否相同？
			gDocument.write( "dataOut.txt" );
			gDocument.release(); //已经读取完了，不需要。
			requestEnd(); //结束
		}
	}
}
