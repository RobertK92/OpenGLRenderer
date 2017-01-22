#include "EnginePCH.h"
#include "Graphics/Texture.h"
#include "Common/AssetLoader.h"

namespace E3D
{
	Texture::Texture(std::string textureFile)
		: Asset(textureFile)
	{
		E3DDebugPrint((std::string("Loading texture: ").append(textureFile)).c_str());

		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(textureFile.c_str());
		FIBITMAP* bitmap = FreeImage_Load(format, textureFile.c_str(), NULL);
		if (!bitmap)
		{
			E3DDebugPrint(((std::string)"Failed to load texture: ").append(textureFile).c_str());
			delete this;
			return;
		}

		bitmap = FreeImage_ConvertTo32Bits(bitmap);
		m_Width = FreeImage_GetWidth(bitmap);
		m_Height = FreeImage_GetHeight(bitmap);

		m_PixelBuffer = new GLubyte[4 * m_Width * m_Height];
		BYTE* tempBuffer = FreeImage_GetBits(bitmap);

		for (int i = 0; i < (m_Width*m_Height); ++i)
		{
			m_PixelBuffer[i * 4 + 0] = tempBuffer[i * 4 + 2];
			m_PixelBuffer[i * 4 + 1] = tempBuffer[i * 4 + 1];
			m_PixelBuffer[i * 4 + 2] = tempBuffer[i * 4 + 0];
			m_PixelBuffer[i * 4 + 3] = tempBuffer[i * 4 + 3];
		}

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int Texture::GetWidth() const
	{
		return m_Width;
	}

	int Texture::GetHeight() const
	{
		return m_Height;
	}

	GLubyte* Texture::GetPixelBuffer() const
	{
		return m_PixelBuffer;
	}


	GLuint Texture::GetHandle() const
	{
		return m_Handle;
	}

	Texture::~Texture()
	{
		delete m_PixelBuffer;
	}
};


