#pragma once
#include <vector>
#include <array>
#include <span>
#include <algorithm>

#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "Figure.h"

class BlockFactory
{
public:
	BlockFactory();

	void pushFigure(Figure::Type pType);
	void pushFigures(Figure::Type pType, int32_t pNumber);
	void popFigure();
	void eraseFigure(size_t pIndex);
	void setAllFiguresShown(bool pShown);

	void setCurrent(size_t pCurrentNum);
	size_t getCurrentNum() const noexcept;

	Figure& getFigure(size_t pIndex) noexcept;
	std::vector<Figure>& getStorage() noexcept;

	void render();

private:
	size_t mCurrentNum{};
	std::vector<Figure> mFigures;

};

