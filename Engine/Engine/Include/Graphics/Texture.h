#pragma once

#include "Common/Asset.h"

namespace E3D
{
	class Texture : public Asset
	{
		friend class AssetLoader;
	public:
		int GetWidth() const;
		int GetHeight() const;
		GLubyte* GetPixelBuffer() const;
		GLuint GetHandle() const;

	private:
		Texture(std::string textureFile);
		virtual ~Texture();

		int m_Width;
		int m_Height;
		GLubyte* m_PixelBuffer;
		GLuint m_Handle;
		
	};
};

