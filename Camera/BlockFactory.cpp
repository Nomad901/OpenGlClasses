#include "BlockFactory.h"

BlockFactory::BlockFactory()
{
	mFigures.reserve(50);
}

void BlockFactory::pushFigure(Figure::Type pType)
{
	switch (pType)
	{
	case Figure::Type::TRIANGLE:
		mFigures.emplace_back().initTriangle();
		break;
	case Figure::Type::QUAD:
		mFigures.emplace_back().initQuad();
		break;
	case Figure::Type::CUBE:
		mFigures.emplace_back().initCube();
		break;
	default:
		break;
	}
}

void BlockFactory::pushFigures(Figure::Type pType, int32_t pNumber)
{
	switch (pType)
	{
	case Figure::Type::TRIANGLE:
		for (int32_t i = 0; i < pNumber; i++)
		{
			mFigures.emplace_back().initTriangle();
		}
		break;
	case Figure::Type::QUAD:
		for (int32_t i = 0; i < pNumber; i++)
		{
			mFigures.emplace_back().initQuad();
		}
		break;
	case Figure::Type::CUBE:
		for (int32_t i = 0; i < pNumber; i++)
		{
			mFigures.emplace_back().initCube();
		}
		break;
	default:
		break;
	}
}

void BlockFactory::popFigure()
{
	mFigures.pop_back();
}

void BlockFactory::eraseFigure(size_t pIndex)
{
	mFigures.erase(mFigures.begin() + pIndex);
}

void BlockFactory::setAllFiguresShown(bool pShown)
{
	for (auto& i : mFigures)
	{
		i.showFigure(pShown);
	}
}

void BlockFactory::setCurrent(size_t pCurrentNum)
{
	pCurrentNum = std::clamp(int32_t(pCurrentNum), 0, int32_t(mFigures.size()));
	mCurrentNum = pCurrentNum;
}

size_t BlockFactory::getCurrentNum() const noexcept
{
	return mCurrentNum;
}

Figure& BlockFactory::getFigure(size_t pIndex) noexcept
{
	SDL_assert(pIndex < mFigures.size());
	return mFigures[pIndex];
}

std::vector<Figure>& BlockFactory::getStorage() noexcept
{
	return mFigures;
}

void BlockFactory::render()
{
	for (auto& i : mFigures)
	{
		i.render();
	}
}
