#ifndef INCLUDED_TEXTURE_H
#define INCLUDED_TEXTURE_H

namespace GameLib{
	class Texture;
}
namespace PseudoXml{
	class Element;
}

class Texture{
public:
	Texture( const PseudoXml::Element* );
	~Texture();
	void set() const; //设置设备
	const string* name() const;
private:
	GameLib::Texture* mTexture;
	string mName;
};

#endif
