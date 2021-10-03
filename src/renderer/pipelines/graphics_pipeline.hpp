/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_GRAPHICS_HPP
#define ZEN_RENDERER_PIPELINES_GRAPHICS_HPP

#include "../pipeline.hpp"
#include "../../math/types/vector2.hpp"
#include "../../components/transform_matrix.hpp"
#include <array>

namespace Zen {

struct VerticeConfig {
	double x;
	double y;
	double lineWidth;
};

class GraphicsPipeline : public Pipeline
{
public:
	GraphicsPipeline (PipelineConfig config);

    /**
     * Pushes a filled rectangle into the vertex batch.
     *
     * Rectangle factors in the given transform matrices before adding to the batch.
     *
     * @since 0.0.0
	 *
     * @param x Horizontal top left coordinate of the rectangle.
     * @param y Vertical top left coordinate of the rectangle.
     * @param width Width of the rectangle.
     * @param height Height of the rectangle.
     * @param currentMatrix The current transform.
     * @param parentMatrix The parent transform.
     */
    void batchFillRect (double x, double y, double width, double height,
			Components::TransformMatrix currentMatrix,
			Components::TransformMatrix parentMatrix);

    /**
     * @overload
     * @since 0.0.0
     */
    void batchFillRect (double x, double y, double width, double height);

    /**
     * Pushes a filled triangle into the vertex batch.
     *
     * Triangle factors in the given transform matrices before adding to the batch.
     *
     * @since 0.0.0
	 *
     * @param x0 Point 0 x coordinate.
     * @param y0 Point 0 y coordinate.
     * @param x1 Point 1 x coordinate.
     * @param y1 Point 1 y coordinate.
     * @param x2 Point 2 x coordinate.
     * @param y2 Point 2 y coordinate.
     * @param currentMatrix The current transform.
     * @param parentMatrix The parent transform.
     */
    void batchFillTriangle (double x0, double y0, double x1, double y1, double x2,
			double y2, Components::TransformMatrix currentMatrix,
			Components::TransformMatrix parentMatrix);

    /**
     * @overload
     * @since 0.0.0
     */
    void batchFillTriangle (double x0, double y0, double x1, double y1, double x2,
			double y2);

    /**
     * Pushes a stroked triangle into the vertex batch.
     *
     * Triangle factors in the given transform matrices before adding to the batch.
     *
     * The triangle is created from 3 lines and drawn using the `batchStrokePath`
	 * method.
     *
     * @since 0.0.0
	 *
     * @paramx0 Point 0 x coordinate.
     * @paramy0 Point 0 y coordinate.
     * @paramx1 Point 1 x coordinate.
     * @paramy1 Point 1 y coordinate.
     * @paramx2 Point 2 x coordinate.
     * @paramy2 Point 2 y coordinate.
     * @paramlineWidth The width of the line in pixels.
     * @param currentMatrix The current transform.
     * @param parentMatrix The parent transform.
     */
    void batchStrokeTriangle (double x0, double y0, double x1, double y1,
			double x2, double y2, double lineWidth,
			Components::TransformMatrix currentMatrix,
			Components::TransformMatrix parentMatrix);

    /**
     * Adds the given path to the vertex batch for rendering.
     *
     * It works by taking the array of path data and then passing it through
	 * Earcut, which creates a list of polygons. Each polygon is then added to the
	 * batch.
     *
     * The path is always automatically closed because it's filled.
     *
     * @since 0.0.0
	 *
     * @param path Collection of points that represent the path.
     * @param currentMatrix The current transform.
     * @param parentMatrix The parent transform.
     */
    void batchFillPath (std::vector<Math::Vector2> path,
			Components::TransformMatrix currentMatrix,
			Components::TransformMatrix parentMatrix);

    /**
     * @overload
     * @since 0.0.0
     */
    void batchFillPath (std::vector<Math::Vector2> path);

    /**
     * Adds the given path to the vertex batch for rendering.
     *
     * It works by taking the array of path data and calling `batchLine` for each
	 * section of the path.
     *
     * The path is optionally closed at the end.
     *
     * @since 0.0.0
     *
     * @param path Collection of points that represent the path.
     * @param lineWidth The width of the line segments in pixels.
     * @param pathOpen Indicates if the path should be closed or left open.
     * @param currentMatrix The current transform.
     * @param parentMatrix The parent transform.
     */
    void batchStrokePath (std::vector<VerticeConfig> path, double lineWidth,
			bool pathOpen, Components::TransformMatrix currentMatrix,
			Components::TransformMatrix parentMatrix);

    /**
     * Creates a line out of 4 quads and adds it to the vertex batch based on the
	 * given line values.
     *
     * @since 0.0.0
     *
     * @param ax x coordinate of the start of the line.
     * @param ay y coordinate of the start of the line.
     * @param bx x coordinate of the end of the line.
     * @param by y coordinate of the end of the line.
     * @param aLineWidth Width of the start of the line.
     * @param bLineWidth Width of the end of the line.
     * @param index If this line is part of a multi-line draw, the index of the
	 * line in the draw.
     * @param closePath Does this line close a multi-line path?
     * @param currentMatrix The current transform.
     * @param parentMatrix The parent transform.
     */
    void batchLine (double ax, double ay, double bx, double by, double aLineWidth,
			double bLineWidth, double lineWidth, int index, bool closePath,
			Components::TransformMatrix currentMatrix,
			Components::TransformMatrix parentMatrix);

	/**
	 * @overload
	 * @since 0.0.0
	 */
    void batchLine (double ax, double ay, double bx, double by, double aLineWidth,
			double bLineWidth, double lineWidth, int index, bool closePath);

    /**
     * Adds a single vertex to the current vertex buffer and increments the
     * `vertexCount` property by 1.
     *
     * This method is called directly by `batchTri` and `batchQuad`.
     *
     * It does not perform any batch limit checking itself, so if you need to call
     * this method directly, do so in the same way that `batchQuad` does, for
	 * example.
     *
     * @since 0.0.0
     *
     * @param x The vertex x position.
     * @param y The vertex y position.
     * @param tint The tint color value.
     */
    void batchVert (double x, double y, int tint);

    /**
     * Adds the vertices data into the batch and flushes if full.
     *
     * Assumes 6 vertices in the following arrangement:
     *
     * ```
     * 0----3
     * |\  B|
     * | \  |
     * |  \ |
     * | A \|
     * |    \
     * 1----2
     * ```
     *
     * Where tx0/ty0 = 0, tx1/ty1 = 1, tx2/ty2 = 2 and tx3/ty3 = 3
     *
     * @since 0.0.0
	 *
     * @param x0 The top-left x position.
     * @param y0 The top-left y position.
     * @param x1 The bottom-left x position.
     * @param y1 The bottom-left y position.
     * @param x2 The bottom-right x position.
     * @param y2 The bottom-right y position.
     * @param x3 The top-right x position.
     * @param y3 The top-right y position.
     * @param tintTL The top-left tint color value.
     * @param tintTR The top-right tint color value.
     * @param tintBL The bottom-left tint color value.
     * @param tintBR The bottom-right tint color value.
	 *
     * @return `true` if this method caused the batch to flush, otherwise `false`.
     */
    bool batchQuad (double x0, double y0, double x1, double y1, double x2,
			double y2, double x3, double y3, int tintTL, int tintTR, int tintBL,
			int tintBR);

    /**
     * Adds the vertices data into the batch and flushes if full.
     *
     * Assumes 3 vertices in the following arrangement:
     *
     * ```
     * 0
     * |\
     * | \
     * |  \
     * |   \
     * |    \
     * 1-----2
     * ```
     *
     * @since 0.0.0
	 *
     * @param x1 The bottom-left x position.
     * @param y1 The bottom-left y position.
     * @param x2 The bottom-right x position.
     * @param y2 The bottom-right y position.
     * @param x3 The top-right x position.
     * @param y3 The top-right y position.
     * @param tintTL The top-left tint color value.
     * @param tintTR The top-right tint color value.
     * @param tintBL The bottom-left tint color value.
	 *
     * @return `true` if this method caused the batch to flush, otherwise `false`.
     */
    bool batchTri (double x0, double y0, double x1, double y1, double x2,
			double y2, int tintTL, int tintTR, int tintBL);

	/**
	 * A temporary Transform Matrix, re-used internally during batching by the
	 * Shape Game Objects.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix calcMatrix;

	/**
	 * Used internally to draw stroked triangles (x, y, width).
	 *
	 * @since 0.0.0
	 */
	std::vector<VerticeConfig> tempTriangle = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	/**
	 * Cached stroke tint (TL, TR, BL, BR).
	 *
	 * @since 0.0.0
	 */
	int strokeTint[4] = {0};

	/**
	 * Cached fill tint (TL, TR, BL, BR).
	 *
	 * @since 0.0.0
	 */
	int fillTint[4] = {0};

	/**
	 * Internal texture frame reference (u0, v0, u1, v1).
	 *
	 * @since 0.0.0
	 */
	int currentFrame[4] = {0};

	/**
	 * Internal path quad cache.
	 *
	 * @since 0.0.0
	 */
	double firstQuad[5] = { 0, 0, 0, 0, 0 };

	/**
	 * Internal path quad cache.
	 *
	 * @since 0.0.0
	 */
	double prevQuad[5] = { 0, 0, 0, 0, 0 };

	/**
	 * Used internally for triangulating a polygon.
	 *
	 * @since 0.0.0
	 */
	std::vector<double> polygonCache;
};

}	// namespace Zen

#endif
