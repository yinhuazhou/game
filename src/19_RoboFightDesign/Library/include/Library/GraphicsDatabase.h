#ifndef INCLUDED_GRAPHICS_DATABASE_H
#define INCLUDED_GRAPHICS_DATABASE_H

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}

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
	//
	const VertexBuffer* vertexBuffer( const char* name ) const;
	const IndexBuffer* indexBuffer( const char* name ) const;
	const Texture* texture( const char* name ) const;
	const Batch* batch( const char* name ) const;
	//生成模型
	Model* createModel( const char* batchName ) const;
private:
	void createFromElement( GameLib::PseudoXml::Element& );

	VertexBuffer** mVertexBuffers;
	IndexBuffer** mIndexBuffers;
	Texture** mTextures;
	Batch** mBatches;

	int mVertexBufferNumber;
	int mIndexBufferNumber;
	int mTextureNumber;
	int mBatchNumber;
};

#endif

