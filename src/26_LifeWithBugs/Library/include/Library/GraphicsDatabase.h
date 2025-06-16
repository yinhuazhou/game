#ifndef INCLUDED_GRAPHICS_DATABASE_H
#define INCLUDED_GRAPHICS_DATABASE_H

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}
using namespace GameLib; //

class VertexBuffer;
class IndexBuffer;
class Texture;
class Batch;
class Model;

class GraphicsDatabase{
public:
	//从伪xml生成元素
	GraphicsDatabase( GameLib::PseudoXml::Element& );
	//从文件生成
	GraphicsDatabase( const char* filename );
	~GraphicsDatabase();
	//（以const版本返回）
	ConstSharedPtr< VertexBuffer > getVertexBuffer( const char* name ) const;
	ConstSharedPtr< IndexBuffer > getIndexBuffer( const char* name ) const;
	ConstSharedPtr< Texture > getTexture( const char* name ) const;
	ConstSharedPtr< Batch > getBatch( const char* name ) const;
	//生成模型
	Model* createModel( const char* batchName ) const;
private:
	void createFromElement( GameLib::PseudoXml::Element& );

	Array< SharedPtr< VertexBuffer > > mVertexBuffers;
	Array< SharedPtr< IndexBuffer > > mIndexBuffers;
	Array< SharedPtr< Texture > > mTextures;
	Array< SharedPtr< Batch > > mBatches;
};

#endif

