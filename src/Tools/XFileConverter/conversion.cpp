#include "conversion.h"
#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Vector4.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Base/Vector.h"
#include "GameLib/Base/Tank.h"
#include "GameLib/Base/OStringStream.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace GameLib;
using namespace GameLib::PseudoXml;
using namespace GameLib::Math;
using namespace std;
//虽然使用别名的做法未必受欢迎，但因为这些数据类型的使用频率太高了只好这样做
typedef Element E;
typedef Attribute A;
typedef ConstElement CE;
typedef ConstAttribute CA;
//类列表
class XFile;
class Frame;
class FrameTransformMatrix;
class Mesh;
class MeshNormals;
class MeshMaterialList;

//数据类型
//typedef Vector< int > Face;
struct Face{
	int& operator[]( int i ){ return mIndices[ i ]; }
	int mIndices[ 3 ];
	int mFaceId;
};
struct Triangle{
	int& operator[]( int i ){ return mIndices[ i ]; }
	int mIndices[ 3 ];
};
struct IndexPair{
	bool operator<( const IndexPair& a ) const {
		if ( mVertex < a.mVertex ){
			return true;
		}else if ( mVertex > a.mVertex ){
			return false;
		}else if ( mNormal < a.mNormal ){
			return true;
		}else if ( mNormal > a.mNormal ){
			return false;
		}else{
			return ( mPosition < a.mPosition );
		}
	}
	int mPosition;
	int mVertex;
	int mNormal;
};

struct IndexColor{
	int mIndex;
	Vector4 mColor;
};
struct Material{
	Vector4 mFaceColor;
	float mPower;
	Vector3 mSpecularColor;
	Vector3 mEmissiveColor;
	string mTextureFilename;
	string mName;
};
struct Vertex{
	Vertex() : mPosition( 0.f ), mNormal( 0.f ), mUv( 0.f ), mColor( 1.f ){}
	Vector3 mPosition;
	Vector3 mNormal;
	Vector2 mUv;
	Vector4 mColor;
};
struct MaterialIndex{
	bool operator<( const MaterialIndex& a ) const {  //按材质编号排序
		return ( mMaterial < a.mMaterial ); 
	}
	int mMaterial;
	int mFace;
};

//////////方便使用而封装的函数
void getVectorArray( Array< Vector3 >* dst, CE e );
void getCoord2dArray( Array< Vector2 >* dst, CE e );
void getIndexColorArray( Array< IndexColor >* dst, CE e );
void getMeshFaceArray( Array< Face >* dst, CE e );
void dieIf( bool f );
int getInt( CE e );
float getFloat( CE e );
const char* getString( CE e );
void getName( RefString*, CE e );
void getMaterial( Material*, CE e );
void getMatrix( Matrix34*, CE e );
void getIndexColor( IndexColor*, CE e );

template< class T > void release( Vector< T* >* a ){
	for ( int i = 0; i < a->size(); ++i ){
		SAFE_DELETE( ( *a )[ i ] );
	}
}

//异常类
class DataStructureException{
public:
	DataStructureException(){ ASSERT( false ); }
};

//核心代码
class MeshMaterialList{
public:
	//创建一个默认的空数据
	MeshMaterialList( int vn ){
		mIndices.setSize( vn );
		for ( int i = 0; i < vn; ++i ){
			mIndices[ i ].mMaterial = 0;
			mIndices[ i ].mFace = i;
		}
		mMaterials.setSize( 1 );
		mMaterials[ 0 ].mFaceColor.set( 1.f, 1.f, 1.f, 1.f );
		mMaterials[ 0 ].mPower = 1.f;
		mMaterials[ 0 ].mSpecularColor.set( 1.f, 1.f, 1.f );
		mMaterials[ 0 ].mEmissiveColor.set( 1.f, 1.f, 1.f );
		mGroupSizes.setSize( 1 );
		mGroupSizes[ 0 ] = vn;
	}
	MeshMaterialList( CE e ){
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "Material" ) == c.name() ){
				Material m;
				getMaterial( &m, c );
				mMaterials.push( m );
			}else{
				RefString name;
				getName( &name, c );
				if ( name == "faceIndexes" ){ //获取顶点
					dieIf( mIndices.size() > 0 ); //不存在第二个
					mIndices.setSize( c.childNumber() );
					for ( int j = 0; j < c.childNumber(); ++j ){
						CE index = c.child( j );
						mIndices[ j ].mFace = j;
						mIndices[ j ].mMaterial = getInt( index );
					}
				}
			}
		}
		//按照材质编号进行排序
		sort( &mIndices[ 0 ], &mIndices[ 0 ] + mIndices.size() );
		//计算大和和偏差偏移
		int matN = mMaterials.size();
		mGroupSizes.setSize( matN );
		for ( int i = 0;i < mGroupSizes.size(); ++i ){
			mGroupSizes[ i ] = 0;
		}
		int prev = mIndices[ 0 ].mMaterial;
		int count = 1;
		for ( int i = 1; i < mIndices.size(); ++i ){
			if ( prev != mIndices[ i ].mMaterial ){
				mGroupSizes[ prev ] = count;
				count = 0;
				prev = mIndices[ i ].mMaterial;
			}
			++count;
		}
		mGroupSizes[ prev ] = count;
	}
	Array< MaterialIndex > mIndices;
	Vector< Material > mMaterials;
	Array< int > mGroupSizes;
};

class MeshNormals{
public:
	MeshNormals( CE e ){
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			RefString name;
			getName( &name, c );
			if ( name == "normals" ){ //获取法线
				dieIf( mNormals.size() > 0 ); //没有第二个
				getVectorArray( &mNormals, c );
			}else if ( name == "faceNormals" ){ //获取三角形
				dieIf( mFaces.size() > 0 ); //没有第二个
				getMeshFaceArray( &mFaces, c );
			}
		}
	}
	Array< Vector3 > mNormals;
	Array< Face > mFaces;
};

class Mesh{
public:
	Mesh( CE e ) :
	mMeshNormals( 0 ),
	mMeshMaterialList( 0 ){
		for ( int i = 0; i < e.attributeNumber(); ++i ){
			CA a = e.attribute( i );
			if ( RefString( "name" ) == a.name() ){
				mName = a.value();
			}
		}
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "MeshNormals" ) == c.name() ){
				dieIf( mMeshNormals != 0 );
				mMeshNormals = NEW MeshNormals( c );
			}else if ( RefString( "MeshTextureCoords" ) == c.name() ){
				dieIf( mUVs.size() > 0 );
				for ( int j = 0; j < c.childNumber(); ++j ){
					CE uvs = c.child( j );
					RefString name;
					getName( &name, uvs );
					if ( name == "textureCoords" ){ //获取UV
						getCoord2dArray( &mUVs, uvs );
					}
				}
			}else if ( RefString( "MeshVertexColors" ) == c.name() ){
				dieIf( mColors.size() > 0 );
				for ( int j = 0; j < c.childNumber(); ++j ){
					CE colors = c.child( j );
					RefString name;
					getName( &name, colors );
					if ( name == "vertexColors" ){ //获取颜色
						getIndexColorArray( &mColors, colors );
					}
				}
			}else if ( RefString( "MeshMaterialList" ) == c.name() ){
				dieIf( mMeshMaterialList != 0 );
				mMeshMaterialList = NEW MeshMaterialList( c );
			}else if ( RefString( "VertexDuplicationIndices" ) == c.name() ){
				; //忽略
			}else if ( RefString( "XSkinMeshHeader" ) == c.name() ){
				; //忽略
			}else if ( RefString( "SkinWeights" ) == c.name() ){
				; //忽略
			}else{
				RefString name;
				getName( &name, c );
				if ( name == "vertices" ){ //获取顶点
					dieIf( mVertices.size() > 0 ); //没有第二个
					getVectorArray( &mVertices, c );
				}else if ( name == "faces" ){ //获取三角形
					dieIf( mFaces.size() > 0 ); //没有第二个
					getMeshFaceArray( &mFaces, c );
				}else if ( name == "nFaces" ){
					mOldFaceNumber = getInt( c ); //转换成三角形之前的面数
				}
			}
		}
		if ( !mMeshMaterialList ){ //没有材质时的处理
			mMeshMaterialList = NEW MeshMaterialList( mVertices.size() );
		}
	}
	~Mesh(){
		SAFE_DELETE( mMeshNormals );
		SAFE_DELETE( mMeshMaterialList );
	}
	int countElement(){
		int matN = mMeshMaterialList->mMaterials.size(); //按照材质数量来执行批次
		int textureN = 0;
		for ( int i = 0; i < matN; ++i ){
			if ( mMeshMaterialList->mMaterials[ i ].mTextureFilename.size() > 0 ){
				++textureN; //根据文件名来对其命名
			}
		}
		return matN + textureN + 1 + 1; //vertexBuffer和IndexBuffer为+2
	}
	int toXml( E e, int uniqueId, multimap< Mesh*, string >* batchNameTable, int* elementPos ){
		OStringStream oss; //各种用于变换的stream
		string meshName;
		if ( mName.size() > 0 ){
			meshName = mName;
		}else{
			oss << "batch" << uniqueId;
			++uniqueId;
			oss.get( &meshName );
			oss.clear();
		}

		//存在共用顶点但法线不同的情况，所以要另外复制顶点
		//1.准备参数组
		int newIn = mFaces.size() * 3; //总索引数
		Array< IndexPair > indexPairs( newIn );
		//把顶点方面和法线方面的索引存起来
		for ( int i = 0; i < mFaces.size(); ++i ){
			for ( int j = 0; j < 3; ++j ){
				int t = i * 3 + j;
				IndexPair& ip = indexPairs[ t ];
				ip.mVertex = mFaces[ i ][ j ];
				ip.mNormal = ( mMeshNormals ) ? mMeshNormals->mFaces[ i ][ j ] : -1;
				ip.mPosition = t;
			}
		}
		//先创建顶点颜色对应表
		int oldVn = mVertices.size();
		Array< int > oldVertexToColors( oldVn, -1 );
		int colorN = mColors.size();
		for ( int i = 0; i < colorN; ++i ){
			oldVertexToColors[ mColors[ i ].mIndex ] = i;
		}
		//排序
		sort( &indexPairs[ 0 ], &indexPairs[ 0 ] + newIn );
		//新索引->新点号表
		Array< int > newIndexToNewVertices( newIn ); 
		Array< Vertex > newVertices( newIn ); //顶点数组分配得稍微大一些，后面再resize
		int prevV = -1;
		int prevN = -1;
		int newVertexPos = -1;
		for ( int i = 0; i < newIn; ++i ){
			const IndexPair& ip = indexPairs[ i ];
			if ( ( prevV != ip.mVertex ) || ( prevN != ip.mNormal ) ){
				++newVertexPos;
				prevV = ip.mVertex;
				prevN = ip.mNormal;
				//存放顶点
				Vertex& v = newVertices[ newVertexPos ];
				v.mPosition = mVertices[ ip.mVertex ];
				if ( mMeshNormals ){
					v.mNormal = mMeshNormals->mNormals[ ip.mNormal ];
				}
				if ( mUVs.size() > ip.mVertex ){
					v.mUv = mUVs[ ip.mVertex ]; //通过下标索引来定位
				}
				int colorIndex = oldVertexToColors[ ip.mVertex ];
				if ( colorIndex != -1 ){
					v.mColor = mColors[ colorIndex ].mColor; //需要从顶点编号变换到颜色编号
				}
			}
			newIndexToNewVertices[ ip.mPosition ] = newVertexPos;
		}
		int newVn = newVertexPos + 1;
		newVertices.setSize( newVn );

		//生成根据.x内的面编号转换为三角形化之后的面编号的变换表
		int oldFaceN = mOldFaceNumber;
		int newFaceN = mFaces.size();
		//数组大小等于原始的面数
		Array< int > oldToNewFaceCounts( oldFaceN, 0 ); //原始的面数
		for ( int i = 0; i < newFaceN; ++i ){
			++oldToNewFaceCounts[ mFaces[ i ].mFaceId ];
		}
		//偏移转换
		int offset = 0;
		Array< int > oldToNewFaceOffsets( oldFaceN ); //原始的面数
		for ( int i = 0; i < oldFaceN; ++i ){
			oldToNewFaceOffsets[ i ] = offset;
			offset += oldToNewFaceCounts[ i ];
			oldToNewFaceCounts[ i ] = 0; //初始化
		}
		//按数组顺序执行
		Array< int > oldToNewFace( newFaceN );
		for ( int i = 0; i < newFaceN; ++i ){
			int t = mFaces[ i ].mFaceId;
			int o = oldToNewFaceOffsets[ t ];
			oldToNewFace[ o + oldToNewFaceCounts[ t ] ] = i;
			++oldToNewFaceCounts[ t ];
		}
		//这样，面的编号转换表就完成了

		//生成索引缓冲
		int matN = mMeshMaterialList->mMaterials.size(); //按照材质数量执行批次
		Array< Tank< Triangle > > ib( matN ); //为了扩展，里面存的是Tank

		offset = 0;
		int ibSizeTotal = 0;
		for ( int i = 0; i < matN; ++i ){
			int n = mMeshMaterialList->mGroupSizes[ i ];
			int start = offset;
			offset += n;
			for ( int j = 0; j < n; ++j ){
				int oldFaceId = mMeshMaterialList->mIndices[ start + j ].mFace;
				int newFaceIdOffset = oldToNewFaceOffsets[ oldFaceId ];
				int newFaceIdCount = oldToNewFaceCounts[ oldFaceId ];
				for ( int k = 0; k < newFaceIdCount; ++k ){
					Triangle tri;
					for ( int l = 0; l < 3; ++l ){
						tri[ l ] = newIndexToNewVertices[ ( k + newFaceIdOffset ) * 3 + l ];
					}
					ib[ i ].add( tri );
					++ibSizeTotal;
				}
			}
		}
		//命名要唯一
		int textureN = 0;
		int batchN = 0;
		for ( int i = 0; i < matN; ++i ){
			//批次
			if ( ib[ i ].size() > 0 ){ //如果没有三角形了
				++batchN;
				//纹理。如果没有批次就不要处理
				if ( mMeshMaterialList->mMaterials[ i ].mTextureFilename.size() > 0 ){
					++textureN; //根据文件名生成
				}
			}
		}
		//顶点缓存
		Element vbE = e.child( *elementPos );
		++*elementPos;
		vbE.setName( "VertexBuffer" );
		vbE.setChildNumber( newVn );
		vbE.setAttributeNumber( 1 );
		
		string vbName;
		oss << meshName << "_vb";
		oss.get( &vbName );
		oss.clear();
		vbE.attribute( 0 ).set( "name", vbName.c_str() );
		for ( int i = 0; i < newVn; ++i ){
			Element vE = vbE.child( i );
			vE.setName( "Vertex" );
			const Vertex& v = newVertices[ i ];
			//法线
			bool writeNormal = ( ( v.mNormal.x != 0.f ) || ( v.mNormal.y != 0.f ) || ( v.mNormal.z != 0.f ) );
			bool writeUv = ( ( v.mUv.x != 0.f ) || ( v.mUv.y != 0.f ) );
			bool writeColor = ( ( v.mColor.x != 1.f ) || ( v.mColor.y != 1.f ) || ( v.mColor.z != 1.f ) || ( v.mColor.w != 1.f ) );
			int attrN = 1;
			attrN += ( writeNormal ) ? 1 : 0;
			attrN += ( writeUv ) ? 1 : 0;
			attrN += ( writeColor ) ? 1 : 0;
			vE.setAttributeNumber( attrN );
			int aPos = 0;
			vE.attribute( aPos ).set( "position", &v.mPosition.x, 3 );
			++aPos;
			if ( writeNormal ){
				vE.attribute( aPos ).set( "normal", &v.mNormal.x, 3 );
				++aPos;
			}
			if ( writeUv ){
				vE.attribute( aPos ).set( "uv", &v.mUv.x, 2 );
				++aPos;
			}
			if ( writeColor ){
				vE.attribute( aPos ).set( "color", &v.mColor.x, 4 );
				++aPos;
			}
		}
		//索引缓存
		Element ibE = e.child( *elementPos );
		++*elementPos;
		ibE.setName( "IndexBuffer" );
		ibE.setChildNumber( ibSizeTotal );
		ibE.setAttributeNumber( 1 );
		string ibName;
		oss << meshName << "_ib";
		oss.get( &ibName );
		oss.clear();
		ibE.attribute( 0 ).set( "name", ibName.c_str() );
		int t = 0;
		for ( int i = 0; i < matN; ++i ){
			while ( !ib[ i ].isEnd() ){
				Element iE = ibE.child( t );
				iE.setName( "Triangle" );
				++t;
				iE.setAttributeNumber( 1 );
				iE.attribute( 0 ).set( "indices", ib[ i ].get()->mIndices, 3 );
				ib[ i ].toNext();
			}
		}
		//执行批次
		t = 0;
		for ( int i = 0; i < matN; ++i ){
			if ( ib[ i ].size() == 0 ){ //如果一个三角形也没有则不会生成批次
				continue;
			}
			Element bE = e.child( *elementPos );
			++*elementPos;
			bE.setName( "Batch" );

			const Material& mat = mMeshMaterialList->mMaterials[ i ];

			//生成批次名
			string batchName;
			oss << meshName << '_';
			//如果材质有名字就用它接上
			if ( mat.mName.size() > 0 ){
				oss << mat.mName;
			}else{ //如果没有名字就用索引值
				oss << i;
			}
			oss.get( &batchName );
			oss.clear();
			//然后处理纹理
			const string& texFilename = mat.mTextureFilename;
			if ( texFilename.size() > 0 ){ //name,vb,ib,ibOffset,triangleNumber,diffuseColor,specularColor,phongExponent,texture
				Element tE = e.child( *elementPos );
				++*elementPos;
				tE.setName( "Texture" );
				tE.setAttributeNumber( 2 );
				//把文件名中的文件类型和路径去掉，剩下的值当作名字
				RefString texName( texFilename.c_str() );
				int periodPos = texName.rfind( '.' );
				int slashPos = texName.rfind( '/' );
				int backSlashPos = texName.rfind( '\\' );
				int last = ( periodPos == -1 ) ? ( texName.size() - 1 ) : ( periodPos - 1 );
				int first = ( slashPos > backSlashPos ) ? ( slashPos + 1 ) : ( backSlashPos + 1 );
				if ( first == -1 ){
					first = 0;
				}
				texName.clamp( first, last );
				tE.attribute( 0 ).set( "name", sizeof( "name" ), texName.get(), texName.size() );
				tE.attribute( 1 ).set( "filename", texFilename.c_str() );
				//设置批次中的纹理引用
				bE.setAttributeNumber( 11 );
				bE.attribute( 10 ).set( "texture", sizeof( "texture" ), texName.get(), texName.size() );
			}else{
				bE.setAttributeNumber( 10 );
			}
			batchNameTable->insert( make_pair( this, batchName ) ); //装到批次名称表
			bE.attribute( 0 ).set( "name", batchName.c_str() );
			bE.attribute( 1 ).set( "vertexBuffer", vbName.c_str() );
			bE.attribute( 2 ).set( "indexBuffer", ibName.c_str() );
			bE.attribute( 3 ).set( "bufferOffset", t * 3 ); //所需三角形x 3
			bE.attribute( 4 ).set( "primitiveNumber", ib[ i ].size() );
			bE.attribute( 5 ).set( "diffuseColor", &mat.mFaceColor.x, 3 );
			bE.attribute( 6 ).set( "transparency", mat.mFaceColor.w );
			bE.attribute( 7 ).set( "specularColor", &mat.mSpecularColor.x, 3 );
			bE.attribute( 8 ).set( "specularSharpness", mat.mPower );
			bE.attribute( 9 ).set( "emissionColor", &mat.mEmissiveColor.x, 3 );

			t += ib[ i ].size();
		}
		return uniqueId;
	}
	Array< Vector3 > mVertices;
	Array< Face > mFaces;
	int mOldFaceNumber;
	MeshNormals* mMeshNormals;
	MeshMaterialList* mMeshMaterialList;
	Array< Vector2 > mUVs;
	Array< IndexColor > mColors;
	string mName;
};

class Frame{
public:
	Frame( CE e ){
		mMatrix.setIdentity();
		//获取名字
		for ( int i = 0; i < e.attributeNumber(); ++i ){
			CA a = e.attribute( i );
			if ( RefString( "name" ) == a.name() ){
				mName = a.value();
			}
		}
		//子结点
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "FrameTransformMatrix" ) == c.name() ){
				dieIf( c.childNumber() == 0 );
				getMatrix( &mMatrix, c.child( 0 ) ); //传递Matrix4x4
			}else if ( RefString( "Mesh" ) == c.name() ){
				mMeshes.push( NEW Mesh( c ) );
			}else if ( RefString( "Frame" ) == c.name() ){
				mFrames.push( NEW Frame( c ) );
			}
		}
	}
	~Frame(){
		release( &mFrames );
		release( &mMeshes );
	}
	void collectMesh( Vector< Mesh* >* meshes ){
		for ( int i = 0; i < mMeshes.size(); ++i ){
			meshes->push( mMeshes[ i ] );
		}
		for ( int i = 0; i < mFrames.size(); ++i ){
			mFrames[ i ]->collectMesh( meshes );
		}
	}
	int toXml( E e, int nodeId, multimap< Mesh*, string >* batchNameTable ){
		typedef multimap< Mesh*, string > Table;
		typedef Table::iterator It;
		//获取自身网格所属的批次名
		vector< string > batchNames;
		for ( int i = 0; i < mMeshes.size(); ++i ){
			pair< It, It > range = batchNameTable->equal_range( mMeshes[ i ] );
			for ( It j = range.first; j != range.second; ++j ){
				batchNames.push_back( j->second );
			}
		}
		//首先设置自己的信息
		e.setName( "Node" );
		Matrix34 identity;
		identity.setIdentity();
		bool writeMatrix = ( identity != mMatrix );
		int childNumber = ( writeMatrix ) ? 2 : 1;
		int batchN = static_cast< int >( batchNames.size() );
		childNumber += batchN; //增加批次数

		e.setAttributeNumber( childNumber ); //如果有name，则transform

		int aPos = 0;
		if ( mName.size() > 0 ){
			e.attribute( aPos ).set( "name", mName.c_str() );
			++aPos;
		}else{
			e.attribute( aPos ).set( "name", nodeId );
			++aPos;
			++nodeId;
		}
		if ( writeMatrix ){
			e.attribute( aPos ).set( "transform", &mMatrix.m00, 12 );
			++aPos;
		}
		for ( int i = 0; i < batchN; ++i ){
			e.attribute( aPos ).set( "batch", batchNames[ i ].c_str() );
			++aPos;
		}
		//处理子元素
		e.setChildNumber( mFrames.size() ) ;
		for ( int i = 0; i < mFrames.size(); ++i ){
			E c = e.child( i );
			nodeId = mFrames[ i ]->toXml( c, nodeId, batchNameTable );
		}
		return nodeId;
	}
	Matrix34 mMatrix;
	Vector< Mesh* > mMeshes;
	Vector< Frame* > mFrames;
	string mName;
};


class XFile{
public:
	XFile( CE e ){
		for ( int i = 0; i < e.childNumber(); ++i ){
			CE c = e.child( i );
			if ( RefString( "Frame" ) == c.name() ){
				mFrames.push( NEW Frame( c ) );
			}else if ( RefString( "Mesh" ) == c.name() ){
				mMeshes.push( NEW Mesh( c ) );
			}
		}
	}
	Document toXml() const {
		Vector< Mesh* > meshes;
		for ( int i = 0; i < mMeshes.size(); ++i ){
			meshes.push( mMeshes[ i ] );
		}
		for ( int i = 0; i < mFrames.size(); ++i ){
			mFrames[ i ]->collectMesh( &meshes );
		}
		//Document
		Document d = Document::create();
		Element root = d.root();
		root.setChildNumber( 1 );
		Element graphicsContainer = root.child( 0 );
		graphicsContainer.setName( "Container" );
		//创建 网格指针->批次名 的multimap
		multimap< Mesh*, string > batchNameTable;
		//元素
		int elementN = 0;
		for ( int i = 0; i < meshes.size(); ++i ){
			elementN += meshes[ i ]->countElement();
		}
		elementN += mFrames.size();
		graphicsContainer.setChildNumber( elementN );

		//网格的XML转换
		int elementPos = 0;
		int id = 0;
		for ( int i = 0; i < meshes.size(); ++i ){
			id = meshes[ i ]->toXml( graphicsContainer, id, &batchNameTable, &elementPos );
		}
		//然后创建树
		//根节点的Frame是Tree
		for ( int i = 0; i < mFrames.size(); ++i ){
			E tree = graphicsContainer.child( elementPos );
			++elementPos;
			tree.setName( "Tree" );
			tree.setAttributeNumber( 1 );
			tree.attribute( 0 ).set( "name", mFrames[ i ]->mName.c_str() );
			tree.setChildNumber( 1 ); //根节点
			E rootNode = tree.child( 0 );
			mFrames[ i ]->toXml( rootNode, 0, &batchNameTable );
		}
		
		//TODO:
		return d;
	}
	~XFile(){
		release( &mFrames );
		release( &mMeshes );
	}
	Vector< Frame* > mFrames;
	Vector< Mesh* > mMeshes;
};

//方便使用而封装的函数
void getVectorArray( Array< Vector3 >* dst, CE e ){
	dst->setSize( e.childNumber() );
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE v = e.child( i ); //Vector
		Vector3& newV = ( *dst )[ i ];

		bool xOk, yOk, zOk;
		xOk = yOk = zOk = false;
		for ( int j = 0; j < v.childNumber(); ++j ){
			CE f = v.child( j );
			RefString name;
			getName( &name, f );
			float value = getFloat( f ); 
			if ( name == "x" ){
				newV.x = value;
				xOk = true;
			}else if ( name == "y" ){
				newV.y = value;
				yOk = true;
			}else if ( name == "z" ){
				newV.z = value;
				zOk = true;
			}
		}
		dieIf( !xOk || !yOk || !zOk );
	}
}

void getCoord2dArray( Array< Vector2 >* dst, CE e ){
	dst->setSize( e.childNumber() );
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE v = e.child( i ); //Vector

		Vector2& newV = ( *dst )[ i ];
		bool uOk, vOk;
		uOk = vOk = false;
		for ( int j = 0; j < v.childNumber(); ++j ){
			CE f = v.child( j );
			RefString name;
			getName( &name, f );
			float value = getFloat( f );
			if ( name == "u" ){
				newV.x = value;
				uOk = true;
			}else if ( name == "v" ){
				newV.y = value;
				vOk = true;
			}
		}
		dieIf( !uOk || !vOk );
	}
}

void getMeshFaceArray( Array< Face >* dst, CE e ){
	//先放入Tank
	Tank< Face > tank;
	Face face; //临时的
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE meshFaceE = e.child( i );
		for ( int j = 0; j < meshFaceE.childNumber(); ++j ){
			CE indicesE = meshFaceE.child( j );
			if ( RefString( "Array" ) == indicesE.name() ){ //如果数组就是它
				face.mFaceId = i;
				face.mIndices[ 0 ] = getInt( indicesE.child( 0 ) );
				face.mIndices[ 1 ] = getInt( indicesE.child( 1 ) );
				for ( int k = 2; k < indicesE.childNumber(); ++k ){
					face.mIndices[ 2 ] = getInt( indicesE.child( k ) );
					tank.add( face );
					face.mIndices[ 1 ] = face.mIndices[ 2 ]; //准备处理下一个
				}
			}
		}
	}
	//拷贝到数组
	if ( tank.size() > 0 ){
		dst->setSize( tank.size() );
		tank.copyTo( &( *dst )[ 0 ] );
	}
}

void getIndexColorArray( Array< IndexColor >* dst, CE e ){
	dst->setSize( e.childNumber() );
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE t = e.child( i );
		getIndexColor( &( ( *dst )[ i ] ), t );
	}
}

void getColor4( Vector4* dst, CE e ){
	bool rOk, gOk, bOk, aOk;
	rOk = gOk = bOk = aOk = false;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		RefString name;
		getName( &name, c );
		float value = getFloat( c );

		if ( name == "red" ){
			dst->x = value;
			rOk = true;
		}else if ( name == "green" ){
			dst->y = value;
			gOk = true;
		}else if ( name == "blue" ){
			dst->z = value;
			bOk = true;
		}else if ( name == "alpha" ){
			dst->w = value;
			aOk = true;
		}
	}
	dieIf( !rOk || !gOk || !bOk || !aOk );
}

void getColor3( Vector3* dst, CE e ){
	bool rOk, gOk, bOk;
	rOk = gOk = bOk = false;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		RefString name;
		getName( &name, c );
		float value = getFloat( c );

		if ( name == "red" ){
			dst->x = value;
			rOk = true;
		}else if ( name == "green" ){
			dst->y = value;
			gOk = true;
		}else if ( name == "blue" ){
			dst->z = value;
			bOk = true;
		}
	}
	dieIf( !rOk || !gOk || !bOk );
}


void getIndexColor( IndexColor* dst, CE e ){
	bool indexOk;
	indexOk = false;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		if ( RefString( "DWORD" ) == c.name() ){
			dst->mIndex = getInt( c );
			indexOk = true;
		}else{
			getColor4( &dst->mColor, c );
		}
	}
	dieIf( !indexOk );
}

void getMaterial( Material* dst, CE e ){
	//获取名字
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "name" ) == a.name() ){
			dst->mName = a.value();
		}
	}
	//还没有放入的
	dst->mFaceColor.x = -1e10f;
	dst->mFaceColor.y = -1e10f;
	dst->mFaceColor.z = -1e10f;
	dst->mFaceColor.w = -1e10f;
	dst->mPower = -1e10f;
	dst->mSpecularColor.x = -1e10f;
	dst->mSpecularColor.y = -1e10f;
	dst->mSpecularColor.z = -1e10f;
	dst->mEmissiveColor.x = -1e10f;
	dst->mEmissiveColor.y = -1e10f;
	dst->mEmissiveColor.z = -1e10f;
	for ( int i = 0; i < e.childNumber(); ++i ){
		CE c = e.child( i );
		if ( RefString( "TextureFilename" ) == c.name() ){
			dieIf( c.childNumber() < 1 );
			dst->mTextureFilename = getString( c.child( 0 ) );
		}else{
			RefString name;
			getName( &name, c );
			if ( name == "faceColor" ){
				getColor4( &dst->mFaceColor, c );
			}else if ( name == "power" ){
				dst->mPower = getFloat( c );
			}else if ( name == "specularColor" ){
				getColor3( &dst->mSpecularColor, c );
			}else if ( name == "emissiveColor" ){
				getColor3( &dst->mEmissiveColor, c );
			}
		}
	}
	//不稳定的处理
	if ( dst->mPower <= 0.f ){ //
		dst->mPower = 1.f;
		dst->mSpecularColor.set( 0.f, 0.f, 0.f );
	}
	dieIf(
		( dst->mFaceColor.x == -1e10f ) ||
		( dst->mFaceColor.y == -1e10f ) ||
		( dst->mFaceColor.z == -1e10f ) ||
		( dst->mFaceColor.w == -1e10f ) ||
		( dst->mPower == -1e10f ) ||
		( dst->mSpecularColor.x == -1e10f ) ||
		( dst->mSpecularColor.y == -1e10f ) ||
		( dst->mSpecularColor.z == -1e10f ) ||
		( dst->mEmissiveColor.x == -1e10f ) ||
		( dst->mEmissiveColor.y == -1e10f ) ||
		( dst->mEmissiveColor.z == -1e10f ) );
}

//抛出异常
void dieIf( bool f ){
	if ( f ){
		throw DataStructureException();
	}
}

//value值是int
int getInt( CE e ){
	int r = 0;
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "value" ) == a.name() ){
			r = a.getIntValue();
			found = true;
		}
	}
	dieIf( !found );
	return r;
}

//value值是Float
float getFloat( CE e ){
	float r = 0;
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "value" ) == a.name() ){
			r = a.getFloatValue();
			found = true;
		}
	}
	dieIf( !found );
	return r;
}

const char* getString( CE e ){
	const char* r = 0;
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "value" ) == a.name() ){
			r = a.value();
			found = true;
		}
	}
	dieIf( !found );
	return r;
}

//获取name值
void getName( RefString* dst, CE e ){
	bool found = false;
	for ( int i = 0; i < e.attributeNumber(); ++i ){
		CA a = e.attribute( i );
		if ( RefString( "name" ) == a.name() ){
			*dst = a.value();
			found = true;
		}
	}
	dieIf( !found );
}

void getMatrix( Matrix34* dst, CE e ){
	dieIf( e.childNumber() == 0 ); 
	e = e.child( 0 ); //生成Array

	dieIf( e.childNumber() < 16 );
	for ( int i = 0; i < 4; ++i ){ //转换后存储
		for ( int j = 0; j < 3; ++j ){
			CE c = e.child( i * 4 + j ); //j==3等于0所以跳过
			float t = getFloat( c );
			if ( -t == 0.f ){ //-0变成+0
				t = 0.f;
			}
			( *dst )( j, i ) = t; 
		}
	}
	//保险起见检查一下
	dieIf( getFloat( e.child( 3 ) ) != 0.f );
	dieIf( getFloat( e.child( 7 ) ) != 0.f );
	dieIf( getFloat( e.child( 11 ) ) != 0.f );
	dieIf( getFloat( e.child( 15 ) ) != 1.f );
}

//根结点函数
//#include <fstream>
Document convert( ConstDocument src ){
#if 0
	string s;
	src.convertToString( &s );
	setlocale( LC_ALL, "" ); //为了确保能够对那些文件名包含中文字符的文件进行处理
	ofstream out( "converted.txt", ofstream::binary );
	out.write( s.c_str(), s.size() );
	out.close();
#endif
	XFile xFile( src.child( 0 ) ); //XFile
	return xFile.toXml();
}
