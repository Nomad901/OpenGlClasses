#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <array>
#include <span>

#include "SDL3/SDL.h"
#include "glad/glad.h"

class Figure
{
public:
	Figure();
	~Figure();

	auto initTriangle() -> void;
	auto initQuad() -> void;
	auto initCube() -> void;

	auto showFigure(bool pShowFigure) -> void;
	auto figureIsShown() noexcept -> bool&;

	auto setWidth(int32_t pWidth) -> void;
	auto setHeight(int32_t pHeight) -> void;
	auto getWidth() noexcept -> float&;
	auto getHeight() noexcept -> float&;

	auto setVertexes(std::span<float> pVertexes,
					 std::span<int16_t> pIndexes) -> void;
	auto getVertexes() noexcept -> std::vector<float>&;
	auto setColors(const std::array<std::array<float, 3>, 4>& pColors) -> void;
	auto getColors() noexcept -> std::array<std::array<float, 3>, 4>&;
	auto getIndexes() noexcept -> const std::vector<int16_t>&;
	
	auto setMultiplier(int32_t pMultiplier) -> void;
	auto getMultyplier() const noexcept -> int32_t;

	auto setOffset(float pOffset) -> void;
	auto getOffset() const noexcept -> float;
	auto setRotation(float pRotation) -> void;
	auto getRotation() const noexcept -> float;

	auto render() -> void;

public:
	enum class Type
	{
		TRIANGLE = 0,
		QUAD = 1,
		CUBE = 2
	};

private:
	bool mFigureIsShown{ true };

	int32_t mMultiplier{};
	float mOffset{};
	float mRotation{};

	// width and height of the Quad
	float mWidth{ 0.5f };
	float mHeight{ 0.5f };

	Type mTypeFigure{ Type::TRIANGLE };

	// VAO
	GLuint mVertexArrayObject{ 0 };
	// VBO
	GLuint mVertexBufferObject{ 0 };
	GLuint mIndexBufferObject{ 0 };

	std::vector<float> mVertexes;
	std::vector<int16_t> mIndexes;
	std::array<std::array<float, 3>, 4> mColorsVertexes;
};

