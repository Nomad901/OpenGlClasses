#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <array>
#include <span>

#include "ErrorHandler.h"

#include "SDL3/SDL.h"
#include "glad/glad.h"

class Mesh3D
{
public:
	Mesh3D();
	~Mesh3D();

	auto initTriangle() -> void;
	auto initQuad() -> void;
	auto initQuadImg() -> void;
	auto initCube() -> void;
	auto initCubeImg() -> void;

	auto showFigure(bool pShowFigure) -> void;
	auto figureIsShown() noexcept -> bool&;

	auto setWidth(int32_t pWidth) -> void;
	auto setHeight(int32_t pHeight) -> void;
	auto getWidth() noexcept -> float&;
	auto getHeight() noexcept -> float&;

	auto setVertexes(std::span<float> pVertexes,
					 std::span<int16_t> pIndexes) -> void;
	auto getVertexes() noexcept -> std::vector<float>&;
	auto setColors(const std::vector<std::array<float,3>>& pColors) -> void;
	auto getColors() noexcept -> std::vector<std::array<float, 3>>&;
	auto getIndexes() noexcept -> const std::vector<int16_t>&;

	auto setOffset(float pOffset) -> void;
	auto getOffset() const noexcept -> float;
	auto setRotation(float pRotation) -> void;
	auto getRotation() const noexcept -> float;

	auto render() -> void; 
	auto clean() -> void;

public:
	enum class Type
	{
		TRIANGLE = 0,
		QUAD = 1,
		CUBE = 2
	};

protected:
	bool mFigureIsShown{ true };

	std::vector<std::array<float,3>> mColorsStrg;

	float mOffset{};
	float mRotation{};

	float mWidth{ 0.5f };
	float mHeight{ 0.5f };

	Type mTypeFigure{ Type::TRIANGLE };
	std::unordered_map<SDL_Keycode, bool> mStrgKeyCodes;

private:
	std::vector<GLfloat> mVertexes;
	std::vector<int16_t> mIndexes;

	GLuint mVertexArrayObject{ 0 };
	GLuint mVertexBufferObject{ 0 };
	GLuint mIndexBufferObject{ 0 };

	friend class UI;
};

