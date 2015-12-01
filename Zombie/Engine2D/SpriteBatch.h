#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Vertex.h"
#include <vector>
#include <algorithm>

namespace Engine2D {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph {
	public:
		Glyph() {}
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint a_texture, float a_depth, const ColorRGBA8& color);
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint a_texture, float a_depth, const ColorRGBA8& color, float angle);
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex topRight;
		Vertex bottomLeft;
		Vertex bottomRight;

	private:
		glm::vec2 rotatePoint(glm::vec2, float);
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint a_offset, GLuint a_numVertices, GLuint a_texture) :
			offset(a_offset),
			numVertices(a_numVertices),
			texture(a_texture) { }

		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch {
	public:
		SpriteBatch();
		~SpriteBatch();

		void Init();

		void Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void End();

		void Draw(const glm::vec4& destRect, const  glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		void Draw(const glm::vec4& destRect, const  glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
		void Draw(const glm::vec4& destRect, const  glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);
		void RenderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyph();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;

		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphPtrs;
		std::vector<Glyph> _glyph;
		std::vector<Engine2D::RenderBatch> _renderBatches;
	};
}
