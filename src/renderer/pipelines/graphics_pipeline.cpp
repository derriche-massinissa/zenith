/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "graphics_pipeline.hpp"
#include "../renderer.hpp"
#include "../../systems/transform_matrix.hpp"
#include <earcut/earcut.hpp>

namespace Zen {

extern Renderer g_renderer;

GraphicsPipeline::GraphicsPipeline (PipelineConfig config) : Pipeline(config)
{
}

void GraphicsPipeline::batchFillRect (double x, double y, double width,
		double height, Components::TransformMatrix currentMatrix,
		Components::TransformMatrix parentMatrix)
{
	// Multiply and store result in calcMatrix, only if the parentMatrix is set
	calcMatrix = parentMatrix;
	Multiply(&calcMatrix, currentMatrix);

	batchFillRect(x, y, width, height);
}

void GraphicsPipeline::batchFillRect (double x, double y, double width,
		double height)
{
	g_renderer.pipelines.set(name);

	// Use whatever values are already in the calcMatrix

	double xw = x + width;
	double yh = y + height;

	double x0 = GetX(calcMatrix, x, y);
	double y0 = GetY(calcMatrix, x, y);

	double x1 = GetX(calcMatrix, x, yh);
	double y1 = GetY(calcMatrix, x, yh);

	double x2 = GetX(calcMatrix, xw, yh);
	double y2 = GetY(calcMatrix, xw, yh);

	double x3 = GetX(calcMatrix, xw, y);
	double y3 = GetY(calcMatrix, xw, y);

	batchQuad(x0, y0, x1, y1, x2, y2, x3, y3, fillTint[0], fillTint[1],
			fillTint[2], fillTint[3]);
}

void GraphicsPipeline::batchFillTriangle (double x0, double y0, double x1,
		double y1, double x2, double y2, Components::TransformMatrix currentMatrix,
		Components::TransformMatrix parentMatrix)
{
	// Multiply and store result in calcMatrix, only if the parentMatrix is set
	calcMatrix = parentMatrix;
	Multiply(&calcMatrix, currentMatrix);

	batchFillTriangle(x0, y0, x1, y1, x2, y2);
}

void GraphicsPipeline::batchFillTriangle (double x0, double y0, double x1,
		double y1, double x2, double y2)
{
	g_renderer.pipelines.set(name);

	double tx0 = GetX(calcMatrix, x0, y0);
	double ty0 = GetY(calcMatrix, x0, y0);

	double tx1 = GetX(calcMatrix, x1, y1);
	double ty1 = GetY(calcMatrix, x1, y1);

	double tx2 = GetX(calcMatrix, x2, y2);
	double ty2 = GetY(calcMatrix, x2, y2);

	batchTri(tx0, ty0, tx1, ty1, tx2, ty2, fillTint[0], fillTint[1], fillTint[2]);
}

void GraphicsPipeline::batchStrokeTriangle (double x0, double y0, double x1,
		double y1, double x2, double y2, double lineWidth,
		Components::TransformMatrix currentMatrix,
		Components::TransformMatrix parentMatrix)
{
	tempTriangle.clear();

	tempTriangle.emplace_back(x0, y0, lineWidth);
	tempTriangle.emplace_back(x1, y1, lineWidth);
	tempTriangle.emplace_back(x2, y2, lineWidth);
	tempTriangle.emplace_back(x0, y0, lineWidth);

	batchStrokePath(tempTriangle, lineWidth, false, currentMatrix, parentMatrix);
}

void GraphicsPipeline::batchFillPath (std::vector<Math::Vector2> path,
		Components::TransformMatrix currentMatrix,
		Components::TransformMatrix parentMatrix)
{
	calcMatrix = parentMatrix;
	Multiply(&calcMatrix, currentMatrix);

	batchFillPath(path);
}

std::vector<std::vector<std::array<double, 2>>> setupPolylines (std::vector<double> vec)
{
	std::vector<std::vector<std::array<double, 2>>> out;

	// Add polyline
	out.emplace_back();

	// Fill polyline
	std::array<double, 2> p;
	for (size_t i = 0; i < vec.size(); i += 2) {
		p = {vec[i], vec[i+1]};
		out[0].emplace_back(p);
	}

	return out;
}

void GraphicsPipeline::batchFillPath (std::vector<Math::Vector2> path)
{
	g_renderer.pipelines.set(name);

	std::vector<std::uint32_t> polygonIndexArray;

	for (auto p : path) {
		polygonCache.push_back(p.x);
		polygonCache.push_back(p.y);
	}

	polygonIndexArray = mapbox::earcut(setupPolylines(polygonCache));

	for (size_t i = 0; i < polygonIndexArray.size(); i++) {
		size_t p0 = polygonIndexArray[i + 0] * 2;
		size_t p1 = polygonIndexArray[i + 1] * 2;
		size_t p2 = polygonIndexArray[i + 2] * 2;

		double x0 = polygonCache[p0 + 0];
		double y0 = polygonCache[p0 + 1];
		double x1 = polygonCache[p1 + 0];
		double y1 = polygonCache[p1 + 1];
		double x2 = polygonCache[p2 + 0];
		double y2 = polygonCache[p2 + 1];

		double tx0 = GetX(calcMatrix, x0, y0);
		double ty0 = GetY(calcMatrix, x0, y0);

		double tx1 = GetX(calcMatrix, x1, y1);
		double ty1 = GetY(calcMatrix, x1, y1);

		double tx2 = GetX(calcMatrix, x2, y2);
		double ty2 = GetY(calcMatrix, x2, y2);

		batchTri(tx0, ty0, tx1, ty1, tx2, ty2, fillTint[0], fillTint[1],
				fillTint[2]);
	}

	polygonCache.clear();
}

void GraphicsPipeline::batchStrokePath (std::vector<VerticeConfig> path,
		double lineWidth, bool pathOpen, Components::TransformMatrix currentMatrix,
		Components::TransformMatrix parentMatrix)
{
	g_renderer.pipelines.set(name);

	// Reset the closePath booleans
	*prevQuad = {0};
	*firstQuad = {0};

	for (size_t i = 0; i < path.size()-1; i++) {
		VerticeConfig point0 = path[i];
		VerticeConfig point1 = path[i + 1];

		batchLine(
			point0.x,
			point0.y,
			point1.x,
			point1.y,
			point0.lineWidth / 2,
			point1.lineWidth / 2,
			lineWidth,
			i,
			!pathOpen && (i == path.size() - 1),
			currentMatrix,
			parentMatrix
		);
	}
}

void GraphicsPipeline::batchLine (double ax, double ay, double bx, double by,
		double aLineWidth, double bLineWidth, double lineWidth, int index,
		bool closePath, Components::TransformMatrix currentMatrix,
		Components::TransformMatrix parentMatrix)
{
	g_renderer.pipelines.set(name);

	calcMatrix = parentMatrix;
	Multiply(&calcMatrix, currentMatrix);

	batchLine(ax, ay, bx, by, aLineWidth, bLineWidth, lineWidth, index, closePath);
}

void GraphicsPipeline::batchLine (double ax, double ay, double bx, double by,
		double aLineWidth, double bLineWidth, double lineWidth, int index,
		bool closePath)
{
	double dx = bx - ax;
	double dy = by - ay;

	double len = std::sqrt(dx * dx + dy * dy);
	double al0 = aLineWidth * (by - ay) / len;
	double al1 = aLineWidth * (ax - bx) / len;
	double bl0 = bLineWidth * (by - ay) / len;
	double bl1 = bLineWidth * (ax - bx) / len;

	double lx0 = bx - bl0;
	double ly0 = by - bl1;
	double lx1 = ax - al0;
	double ly1 = ay - al1;
	double lx2 = bx + bl0;
	double ly2 = by + bl1;
	double lx3 = ax + al0;
	double ly3 = ay + al1;

	//  tx0 = bottom right
	double brX = GetX(calcMatrix, lx0, ly0);
	double brY = GetY(calcMatrix, lx0, ly0);

	//  tx1 = bottom left
	double blX = GetX(calcMatrix, lx1, ly1);
	double blY = GetY(calcMatrix, lx1, ly1);

	//  tx2 = top right
	double trX = GetX(calcMatrix, lx2, ly2);
	double trY = GetY(calcMatrix, lx2, ly2);

	//  tx3 = top left
	double tlX = GetX(calcMatrix, lx3, ly3);
	double tlY = GetY(calcMatrix, lx3, ly3);

	int tintTL = strokeTint[0];
	int tintTR = strokeTint[1];
	int tintBL = strokeTint[2];
	int tintBR = strokeTint[3];

	//  TL, BL, BR, TR
	batchQuad(tlX, tlY, blX, blY, brX, brY, trX, trY, tintTL, tintTR, tintBL,
			tintBR);

	if (lineWidth <= 2)
	{
		//  No point doing a linejoin if the line isn't thick enough
		return;
	}

	if (index > 0 && prevQuad[4])
	{
		batchQuad(tlX, tlY, blX, blY, prevQuad[0], prevQuad[1], prevQuad[2],
				prevQuad[3], tintTL, tintTR, tintBL, tintBR);
	}
	else
	{
		firstQuad[0] = tlX;
		firstQuad[1] = tlY;
		firstQuad[2] = blX;
		firstQuad[3] = blY;
		firstQuad[4] = 1;
	}

	if (closePath && firstQuad[4])
	{
		//  Add a join for the final path segment
		batchQuad(brX, brY, trX, trY, firstQuad[0], firstQuad[1], firstQuad[2],
				firstQuad[3], tintTL, tintTR, tintBL, tintBR);
	}
	else
	{
		//  Store it
		prevQuad[0] = brX;
		prevQuad[1] = brY;
		prevQuad[2] = trX;
		prevQuad[3] = trY;
		prevQuad[4] = 1;
	}
}

void GraphicsPipeline::batchVert (double x, double y, int tint)
{
	float *vertexViewF32 = reinterpret_cast<float*>(vertexData.data());
	std::uint32_t *vertexViewU32 =
		reinterpret_cast<std::uint32_t*>(vertexData.data());

	size_t vertexOffset = (vertexCount * currentShader->vertexComponentCount) - 1;

	vertexViewF32[++vertexOffset] = x;
	vertexViewF32[++vertexOffset] = y;
	vertexViewU32[++vertexOffset] = tint;

	vertexCount++;
}

bool GraphicsPipeline::batchQuad (double x0, double y0, double x1, double y1,
		double x2, double y2, double x3, double y3, int tintTL, int tintTR,
		int tintBL, int tintBR)
{
	bool hasFlushed = false;

	if (shouldFlush(6)) {

		flush();
		hasFlushed = true;
	}

	batchVert(x0, y0, tintTL);
	batchVert(x1, y1, tintBL);
	batchVert(x2, y2, tintBR);
	batchVert(x0, y0, tintTL);
	batchVert(x2, y2, tintBR);
	batchVert(x3, y3, tintTR);

	return hasFlushed;
}

bool GraphicsPipeline::batchTri (double x0, double y0, double x1, double y1,
		double x2, double y2, int tintTL, int tintTR, int tintBL)
{
	bool hasFlushed = false;

	if (shouldFlush(3)) {
		flush();

		hasFlushed = true;
	}

	batchVert(x0, y0, tintTL);
	batchVert(x1, y1, tintTR);
	batchVert(x2, y2, tintBL);

	return hasFlushed;
}

}	// namespace Zen
