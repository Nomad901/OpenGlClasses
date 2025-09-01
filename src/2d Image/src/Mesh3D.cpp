#include "Mesh3D.h"

Mesh3D::Mesh3D()
{
	mVertexes.reserve(100);
	mIndexes.reserve(100);
	mColorsStrg.reserve(30);

	mColorsStrg.push_back({ 1.0f, 0.0f, 0.0f });
	mColorsStrg.push_back({ 1.0f, 1.0f, 0.0f });
	mColorsStrg.push_back({ 1.0f, 0.0f, 1.0f });
	mColorsStrg.push_back({ 1.0f, 0.0f, 0.0f });
}

Mesh3D::~Mesh3D()
{
	glDeleteBuffers(1, &mVertexBufferObject);
	glDeleteBuffers(1, &mIndexBufferObject);
	glDeleteVertexArrays(1, &mVertexArrayObject);
}

auto Mesh3D::initTriangle() -> void
{
	mTypeFigure = Type::TRIANGLE;
	
	mVertexes =
	{
		-mWidth, -mHeight, -1.0f,
		 mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
		 mWidth, -mHeight, -1.0f,
		 mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
		-mWidth,  mHeight, -1.0f,
		 mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	};
	
	glCreateVertexArrays(1, &mVertexArrayObject);
	glCreateBuffers(1, &mVertexBufferObject);

	// vertex buffer
	glNamedBufferData(mVertexBufferObject,
		mVertexes.size() * sizeof(GL_FLOAT),
		mVertexes.data(),
		GL_STATIC_DRAW);

	glVertexArrayVertexBuffer(mVertexArrayObject,
		0,
		mVertexBufferObject,
		0,
		6 * sizeof(GLfloat));

	glEnableVertexArrayAttrib(mVertexArrayObject, 0);
	glVertexArrayAttribFormat(mVertexArrayObject,
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0);
	glVertexArrayAttribBinding(mVertexArrayObject, 0, 0);

	glEnableVertexArrayAttrib(mVertexArrayObject, 1);
	glVertexArrayAttribFormat(mVertexArrayObject,
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat));
	glVertexArrayAttribBinding(mVertexArrayObject, 1, 0);
}

auto Mesh3D::initQuad() -> void
{
	mTypeFigure = Type::QUAD;

	mVertexes =
	{
		-mWidth, -mHeight, -1.0f,
		 mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
		 mWidth, -mHeight, -1.0f,
		 mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
		-mWidth,  mHeight, -1.0f,
		 mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
		 mWidth,  mHeight, -1.0f,
		 mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],
	};

	mIndexes =
	{
		0, 1, 2,
		2, 1, 3,
	};

	glCreateVertexArrays(1, &mVertexArrayObject);
	glCreateBuffers(1, &mVertexBufferObject);

	// vertex buffer
	glNamedBufferData(mVertexBufferObject,
		mVertexes.size() * sizeof(GL_FLOAT),
		mVertexes.data(),
		GL_STATIC_DRAW);
	//index buffer
	glCreateBuffers(1, &mIndexBufferObject);
	glNamedBufferData(mIndexBufferObject,
		mIndexes.size() * sizeof(GLuint),
		mIndexes.data(),
		GL_STATIC_DRAW);
	glVertexArrayElementBuffer(mVertexArrayObject, mIndexBufferObject);

	glVertexArrayVertexBuffer(mVertexArrayObject,
		0,
		mVertexBufferObject,
		0,
		6 * sizeof(GLfloat));

	glEnableVertexArrayAttrib(mVertexArrayObject, 0);
	glVertexArrayAttribFormat(mVertexArrayObject,
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0);
	glVertexArrayAttribBinding(mVertexArrayObject, 0, 0);

	glEnableVertexArrayAttrib(mVertexArrayObject, 1);
	glVertexArrayAttribFormat(mVertexArrayObject,
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat));
	glVertexArrayAttribBinding(mVertexArrayObject, 1, 0);
}

auto Mesh3D::initQuadImg() -> void 
{
	mTypeFigure = Type::QUAD;

	mVertexes =
	{
		-mWidth, -mHeight, -1.0f,
		 mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
		 0.0f, 0.0f,
		 mWidth, -mHeight, -1.0f,
		 mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
		 1.0f, 0.0f,
		-mWidth,  mHeight, -1.0f,
		 mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
		 0.0f, 1.0f,
		 mWidth,  mHeight, -1.0f,
		 mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],
		 1.0f, 1.0f
	};

	mIndexes =
	{
		0, 1, 2,
		2, 1, 3,
	};

	glCreateVertexArrays(1, &mVertexArrayObject);
	glCreateBuffers(1, &mVertexBufferObject);

	// vertex buffer
	glNamedBufferData(mVertexBufferObject,
					  mVertexes.size() * sizeof(GL_FLOAT),
					  mVertexes.data(),
					  GL_STATIC_DRAW);
	//index buffer
	glCreateBuffers(1, &mIndexBufferObject);
	glNamedBufferData(mIndexBufferObject,
					  mIndexes.size() * sizeof(GLuint),
					  mIndexes.data(),
					  GL_STATIC_DRAW);
	glVertexArrayElementBuffer(mVertexArrayObject, mIndexBufferObject);

	glVertexArrayVertexBuffer(mVertexArrayObject,
							  0,
							  mVertexBufferObject,
							  0,
							  8 * sizeof(GLfloat));

	glEnableVertexArrayAttrib(mVertexArrayObject, 0);
	glVertexArrayAttribFormat(mVertexArrayObject,
							  0,
							  3,
							  GL_FLOAT,
							  GL_FALSE,
							  0);
	glVertexArrayAttribBinding(mVertexArrayObject, 0, 0);

	glEnableVertexArrayAttrib(mVertexArrayObject, 1);
	glVertexArrayAttribFormat(mVertexArrayObject,
							  1,
							  3,
							  GL_FLOAT,
							  GL_FALSE,
							  3 * sizeof(GLfloat));
	glVertexArrayAttribBinding(mVertexArrayObject, 1, 0);
	
	glEnableVertexArrayAttrib(mVertexArrayObject, 2);
	glVertexArrayAttribFormat(mVertexArrayObject,
							  2,
							  2,
							  GL_FLOAT,
							  GL_FALSE,
							  6 * sizeof(GLfloat));
	glVertexArrayAttribBinding(mVertexBufferObject, 2, 0);
}

auto Mesh3D::initCube() -> void
{
	mTypeFigure = Type::CUBE;

	mVertexes =
	{
	  -mWidth, -mHeight, -1.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -1.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -1.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -1.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth, -mHeight, -2.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -2.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -2.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -2.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth, -mHeight, -2.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	  -mWidth, -mHeight, -1.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -2.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	  -mWidth,  mHeight, -1.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	   mWidth, -mHeight, -1.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -2.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	   mWidth,  mHeight, -1.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -2.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth,  mHeight, -1.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth,  mHeight, -1.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -2.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -2.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth, -mHeight, -2.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -2.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth, -mHeight, -1.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth, -mHeight, -1.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],
	};

	mIndexes =
	{
		0, 1, 2,
		2, 1, 3,

		4, 6, 5,
		5, 6, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23
	};

	glCreateVertexArrays(1, &mVertexArrayObject);
	glCreateBuffers(1, &mVertexBufferObject);

	// vertex buffer
	glNamedBufferData(mVertexBufferObject,
		mVertexes.size() * sizeof(GL_FLOAT),
		mVertexes.data(),
		GL_STATIC_DRAW);
	//index buffer
	glCreateBuffers(1, &mIndexBufferObject);
	glNamedBufferData(mIndexBufferObject,
		mIndexes.size() * sizeof(GLuint),
		mIndexes.data(),
		GL_STATIC_DRAW);
	glVertexArrayElementBuffer(mVertexArrayObject, mIndexBufferObject);

	glVertexArrayVertexBuffer(mVertexArrayObject,
		0,
		mVertexBufferObject,
		0,
		6 * sizeof(GLfloat));

	glEnableVertexArrayAttrib(mVertexArrayObject, 0);
	glVertexArrayAttribFormat(mVertexArrayObject,
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0);
	glVertexArrayAttribBinding(mVertexArrayObject, 0, 0);

	glEnableVertexArrayAttrib(mVertexArrayObject, 1);
	glVertexArrayAttribFormat(mVertexArrayObject,
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat));
	glVertexArrayAttribBinding(mVertexArrayObject, 1, 0);
}

auto Mesh3D::initCubeImg() -> void
{
	mTypeFigure = Type::CUBE;

	mVertexes =
	{
	  -mWidth, -mHeight, -1.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -1.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -1.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -1.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth, -mHeight, -2.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -2.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -2.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -2.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth, -mHeight, -2.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	  -mWidth, -mHeight, -1.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -2.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	  -mWidth,  mHeight, -1.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	   mWidth, -mHeight, -1.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -2.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	   mWidth,  mHeight, -1.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -2.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth,  mHeight, -1.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth,  mHeight, -1.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth,  mHeight, -2.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth,  mHeight, -2.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],

	  -mWidth, -mHeight, -2.0f,
	   mColorsStrg[0][0], mColorsStrg[0][1], mColorsStrg[0][2],
	   mWidth, -mHeight, -2.0f,
	   mColorsStrg[1][0], mColorsStrg[1][1], mColorsStrg[1][2],
	  -mWidth, -mHeight, -1.0f,
	   mColorsStrg[2][0], mColorsStrg[2][1], mColorsStrg[2][2],
	   mWidth, -mHeight, -1.0f,
	   mColorsStrg[3][0], mColorsStrg[3][1], mColorsStrg[3][2],
	};

	mIndexes =
	{
		0, 1, 2,
		2, 1, 3,

		4, 6, 5,
		5, 6, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23
	};

	glCreateVertexArrays(1, &mVertexArrayObject);
	glCreateBuffers(1, &mVertexBufferObject);

	// vertex buffer
	glNamedBufferData(mVertexBufferObject,
		mVertexes.size() * sizeof(GL_FLOAT),
		mVertexes.data(),
		GL_STATIC_DRAW);
	//index buffer
	glCreateBuffers(1, &mIndexBufferObject);
	glNamedBufferData(mIndexBufferObject,
		mIndexes.size() * sizeof(GLuint),
		mIndexes.data(),
		GL_STATIC_DRAW);
	glVertexArrayElementBuffer(mVertexArrayObject, mIndexBufferObject);

	glVertexArrayVertexBuffer(mVertexArrayObject,
		0,
		mVertexBufferObject,
		0,
		6 * sizeof(GLfloat));

	glEnableVertexArrayAttrib(mVertexArrayObject, 0);
	glVertexArrayAttribFormat(mVertexArrayObject,
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0);
	glVertexArrayAttribBinding(mVertexArrayObject, 0, 0);

	glEnableVertexArrayAttrib(mVertexArrayObject, 1);
	glVertexArrayAttribFormat(mVertexArrayObject,
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat));
	glVertexArrayAttribBinding(mVertexArrayObject, 1, 0);

}

auto Mesh3D::showFigure(bool pShowFigure) -> void
{
	mFigureIsShown = pShowFigure;
}

auto Mesh3D::figureIsShown() noexcept -> bool&
{
	return mFigureIsShown;
}

auto Mesh3D::setWidth(int32_t pWidth) -> void
{
	mWidth = pWidth;
}

auto Mesh3D::setHeight(int32_t pHeight) -> void
{
	mHeight = pHeight;
}

auto Mesh3D::getWidth() noexcept -> float&
{
	return mWidth;
}

auto Mesh3D::getHeight() noexcept -> float&
{
	return mHeight;
}

auto Mesh3D::setVertexes(std::span<float> pVertexes,
	std::span<int16_t> pIndexes) -> void
{
	mVertexes.assign(pVertexes.begin(), pVertexes.end());
	mIndexes.assign(pIndexes.begin(), pIndexes.end());
}

auto Mesh3D::getVertexes() noexcept -> std::vector<float>&
{
	return mVertexes;
}

auto Mesh3D::setColors(const std::vector<std::array<float, 3>>& pColors) -> void
{
	mColorsStrg = pColors;
}

auto Mesh3D::getColors() noexcept -> std::vector<std::array<float, 3>>&
{
	return mColorsStrg;
}

auto Mesh3D::getIndexes() noexcept -> const std::vector<int16_t>&
{
	return mIndexes;
}

auto Mesh3D::setOffset(float pOffset) -> void
{
	mOffset = pOffset;
}

auto Mesh3D::getOffset() const noexcept -> float
{
	return mOffset;
}

auto Mesh3D::setRotation(float pRotation) -> void
{
	mRotation = pRotation;
}

auto Mesh3D::getRotation() const noexcept -> float
{
	return mRotation;
}

auto Mesh3D::render() -> void
{
	if (!mFigureIsShown) return;

	glBindVertexArray(mVertexArrayObject);
	if (mFigureIsShown)
	{
		if (mTypeFigure == Type::TRIANGLE)
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else
		{
			glDrawElements(GL_TRIANGLES,
						   mIndexes.size(),
						   GL_UNSIGNED_SHORT,
						   0);
		}
	}
	glBindVertexArray(0);
}

auto Mesh3D::clean() -> void
{
	mVertexes.clear();
	mIndexes.clear();

	glDeleteBuffers(1, &mVertexBufferObject);
	glDeleteBuffers(1, &mIndexBufferObject);
	glDeleteVertexArrays(1, &mVertexArrayObject);

	mVertexes.reserve(100);
	mIndexes.reserve(100);
}
