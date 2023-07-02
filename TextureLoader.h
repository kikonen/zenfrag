#include "includes.h"

class TextureLoader
{
public:
	
	~TextureLoader();
	static TextureLoader* instance();
	unsigned int load(const char *textureFilename);
    
	
  private:



	  TextureLoader();
      static TextureLoader* _instance;

	  map<string,int> cache ;
};