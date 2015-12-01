#include "SpriteBatch.h"

#include <algorithm>

namespace Engine2D {
	Glyph::Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint a_texture, float a_depth, const ColorRGBA8 & color) :
		texture(a_texture),
		depth(a_depth) {

		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}
	Glyph::Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint a_texture, float a_depth, const ColorRGBA8 & color, float angle) :
		texture(a_texture),
		depth(a_depth) {

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		// Get points centered at origin
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);

		// Rotate the points
		tl = rotatePoint(tl, angle) + halfDims;
		bl = rotatePoint(bl, angle) + halfDims;
		br = rotatePoint(br, angle) + halfDims;
		tr = rotatePoint(tr, angle) + halfDims;

		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle) {
		glm::vec2 newV;
		newV.x = pos.x * cos(angle) - pos.y * sin(angle);
		newV.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newV;
	}

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
	{
	}
	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::Init() {
		createVertexArray();
	}

	void SpriteBatch::Begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
		_sortType = sortType;
		_renderBatches.clear();
		// Have to delete any glyphs that remain so we don't have memory leaks!
		_glyph.clear();
	}

	void SpriteBatch::End() {
		_glyphPtrs.resize(_glyph.size());
		for (int i = 0; i < _glyph.size(); i++) {
			_glyphPtrs[i] = &_glyph[i];
		}
		sortGlyph();
		createRenderBatches();
	}

	void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
		_glyph.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::Draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, float angle) {
		_glyph.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::Draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, const glm::vec2& dir) {
		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(right, dir));
		if (dir.y < 0) angle = -angle;

		_glyph.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::RenderBatch() {

		// Bind our VAO. This sets up the opengl state we need, including the 
		// vertex attribute pointers and it binds the VBO
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		// This will store all the vertices that we need to upload
		std::vector <Vertex> vertices;
		// Resize the buffer to the exact size we need so we can treat
		// it like an array
		vertices.resize(_glyph.size() * 6);

		if (_glyph.empty()) {
			return;
		}

		int offset = 0; // current offset
		int cv = 0; // current vertex

					//Add the first batch
		_renderBatches.emplace_back(offset, 6, _glyphPtrs[0]->texture);
		vertices[cv++] = _glyphPtrs[0]->topLeft;
		vertices[cv++] = _glyphPtrs[0]->bottomLeft;
		vertices[cv++] = _glyphPtrs[0]->bottomRight;
		vertices[cv++] = _glyphPtrs[0]->bottomRight;
		vertices[cv++] = _glyphPtrs[0]->topRight;
		vertices[cv++] = _glyphPtrs[0]->topLeft;
		offset += 6;

		//Add all the rest of the glyphs
		for (int cg = 1; cg < _glyph.size(); cg++) {

			// Check if this glyph can be part of the current batch
			if (_glyphPtrs[cg]->texture != _glyphPtrs[cg - 1]->texture) {
				// Make a new batch
				_renderBatches.emplace_back(offset, 6, _glyphPtrs[cg]->texture);
			}
			else {
				// If its part of the current batch, just increase numVertices
				_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = _glyphPtrs[cg]->topLeft;
			vertices[cv++] = _glyphPtrs[cg]->bottomLeft;
			vertices[cv++] = _glyphPtrs[cg]->bottomRight;
			vertices[cv++] = _glyphPtrs[cg]->bottomRight;
			vertices[cv++] = _glyphPtrs[cg]->topRight;
			vertices[cv++] = _glyphPtrs[cg]->topLeft;
			offset += 6;
		}

		// Bind our VBO
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// Orphan the buffer (for speed)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		// Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void SpriteBatch::createVertexArray() {

		// Generate the VAO if it isn't already generated
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}

		// Bind the VAO. All subsequent opengl calls will modify it's state.
		glBindVertexArray(_vao);

		//G enerate the VBO if it isn't already generated
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//Tell opengl what attribute arrays we need
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

	}

	void SpriteBatch::sortGlyph() {

		switch (_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPtrs.begin(), _glyphPtrs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPtrs.begin(), _glyphPtrs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPtrs.begin(), _glyphPtrs.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}

	

}