#ifndef DUNJUN_IMAGE_HPP
#define DUNJUN_IMAGE_HPP

#include <Dunjun/Types.hpp>

namespace Dunjun
{
class Image
{
public:
	enum Format
	{
		Format_Greyscale = 1,
		Format_GreyscaleAlpha = 2,
		Format_RGB = 3,
		Format_RGBA = 4,
	};

	Image();
	Image(u32 width, u32 height, Format format, const u8* pixels = nullptr);
	Image(const Image& other);
	Image& operator=(const Image& other);
	virtual ~Image();

	bool loadFromFile(const char* filename);
	bool loadFromMemory(u32 width, u32 height, Format format, const u8* pixels);

	inline u32 getWidth() const { return m_width; }
	inline u32 getHeight() const { return m_height; }
	inline Format getFormat() const { return m_format; };
	inline u8* getPixelPtr() const { return m_pixels; };

	u8* getPixel(u32 column, u32 row) const;
	void getPixel(u32 column, u32 row, const u32* pixel);

	void flipVertaically();
	// void rotate90CCW();

	// void copyRectFromImage(const Image& src,
	//                       UInt32 srcCol,
	//                       UInt32 srcRow,
	//                       UInt32 destCol,
	//                       UInt32 destRow,
	//                       UInt32 width,
	//                       UInt32 height);

private:
	Format m_format;
	u32 m_width;
	u32 m_height;
	u8* m_pixels;
};
} // namespace Dunjun

#endif
