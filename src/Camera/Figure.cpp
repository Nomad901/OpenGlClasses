#include "Figure.h"

Figure::Figure()
{
	mVertexes.reserve(100);
	mIndexes.reserve(100);
}

auto Figure::initTriangle() -> void
{
	mTypeFigure = Type::TRIANGLE;

	mColorsVertexes =
	{
		std::array<float,3>({ -1.0f, 0.0f, 0.0f, }),
		std::array<float,3>({  0.0f, 1.0f, 0.0f, }),
		std::array<float,3>({  0.0f, 0.0f, 1.0f, }),
	};

	mVertexes =
	{
		// Front face 
		-mWidth, -mHeight,  0.5f,
		 mColorsVertexes[0][0], mColorsVertexes[0][1], mColorsVertexes[0][2],
		 mWidth, -mHeight,  0.5f,
		 mColorsVertexes[1][0], mColorsVertexes[1][1], mColorsVertexes[1][2],
		-mWidth,  mHeight,  0.5f,
		 mColorsVertexes[2][0], mColorsVertexes[2][1], mColorsVertexes[2][2],
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

auto Figure::initQuad() -> void
{
	mTypeFigure = Type::QUAD;

	mColorsVertexes =
	{
		std::array<float,3>({ -1.0f, 0.0f, 0.0f, }),
		std::array<float,3>({  0.0f, 1.0f, 0.0f, }),
		std::array<float,3>({  0.0f, 0.0f, 1.0f, }),
		std::array<float,3>({ -1.0f, 1.0f, 0.0f, })
	};

	mVertexes =
	{
		// Front face 
		-mWidth, -mHeight,  0.5f,
		 mColorsVertexes[0][0], mColorsVertexes[0][1], mColorsVertexes[0][2],
		 mWidth, -mHeight,  0.5f,
		 mColorsVertexes[1][0], mColorsVertexes[1][1], mColorsVertexes[1][2],
		-mWidth,  mHeight,  0.5f,
		 mColorsVertexes[2][0], mColorsVertexes[2][1], mColorsVertexes[2][2],
		 mWidth,  mHeight,  0.5f,
		 mColorsVertexes[3][0], mColorsVertexes[3][1], mColorsVertexes[3][2],
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
	glVertexArrayElementBuffer(mVertexBufferObject, mIndexBufferObject);

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

auto Figure::showFigure(bool pShowFigure) -> void
{
	mFigureIsShown = pShowFigure;
}

auto Figure::figureIsShown() noexcept -> bool&
{
	return mFigureIsShown;
}

auto Figure::setWidth(int32_t pWidth) -> void
{
	mWidth = pWidth;
}

auto Figure::setHeight(int32_t pHeight) -> void
{
	mHeight = pHeight;
}

auto Figure::getWidth() noexcept -> float&
{
	return mWidth;
}

auto Figure::getHeight() noexcept -> float&
{
	return mHeight;
}

auto Figure::setVertexes(std::span<float> pVertexes,
	std::span<int16_t> pIndexes) -> void
{
	mVertexes.assign(pVertexes.begin(), pVertexes.end());
	mIndexes.assign(pIndexes.begin(), pIndexes.end());
}

auto Figure::getVertexes() noexcept -> std::vector<float>&
{
	return mVertexes;
}

auto Figure::setColors(const std::array<std::array<float, 3>, 4>& pColors) -> void
{
	mColorsVertexes = pColors;
}

auto Figure::getColors() noexcept -> std::array<std::array<float, 3>, 4>&
{
	return mColorsVertexes;
}

auto Figure::getIndexes() noexcept -> const std::vector<int16_t>&
{
	return mIndexes;
}

auto Figure::setOffset(float pOffset) -> void
{
	mOffset = pOffset;
}

auto Figure::getOffset() const noexcept -> float
{
	return mOffset;
}

auto Figure::setRotation(float pRotation) -> void
{
	mRotation = pRotation;
}

auto Figure::getRotation() const noexcept -> float
{
	return mRotation;
}

auto Figure::render() -> void
{
	// enable our attributes
	glBindVertexArray(mVertexArrayObject);
	// render the data. 0 the beginning of the array, 3 - the end
	if (mFigureIsShown)
	{
		if (mTypeFigure == Type::TRIANGLE)
		{
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else
		{
			glDrawElements(GL_TRIANGLES,
						   mIndexes.size() * sizeof(GLuint),
						   GL_UNSIGNED_INT,
						   0);
		}
	}
}