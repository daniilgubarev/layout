#pragma once

#include <vector>

#include <QRect>

struct TilingParams
{
	QSize grid;
	QSize window;
};

TilingParams CalculateTiling(QSize screen, int windows, float aspectRatio, QSize spacing, QSize multiplicity, QSize maxSize)
{
	if (screen.height() - spacing.height() * 2 <= 0
		|| screen.width() - spacing.width() * 2 <= 0)
		throw "not enough space";

	QSize grid{ 0, 0 };
	QSize window{ 0, 0 };

	while (grid.width() * grid.height() < windows
		|| window.width() > maxSize.width()
		|| window.height() > maxSize.height())
	{
		grid.rheight()++;

		window.setHeight((screen.height() - spacing.height() * (grid.height() + 1)) / grid.height());
		window.rheight() -= window.height() % multiplicity.height();
		window.setWidth(window.height() * aspectRatio);

		grid.setWidth((screen - spacing).width() / (window + spacing).width());
	}

	window.rwidth() -= window.width() % multiplicity.width();

	return { grid, window };
}

struct OptimalTilingParams
{
	QSize mainGrid;
	QSize extraGrid;
	QSize window;
};

OptimalTilingParams CalculateOptimalTiling(QSize screen, int windows, float aspectRatio, QSize spacing, QSize multiplicity, QSize maxSize)
{
	TilingParams tpVertical = CalculateTiling(screen, windows, aspectRatio, spacing, multiplicity, maxSize);

	TilingParams tpHorizontal = CalculateTiling(
		screen.transposed(),
		windows,
		1.f / aspectRatio,
		spacing.transposed(),
		multiplicity.transposed(),
		maxSize.transposed());

	tpHorizontal.window.transpose();
	tpHorizontal.grid.transpose();

	TilingParams tp =
		tpVertical.window.width() * tpVertical.window.height() > tpHorizontal.window.width() * tpHorizontal.window.height() ?
		tpVertical : tpHorizontal;

	QSize extraGrid{ 0, 0 };

	if (tp.grid.width() * tp.grid.height() > windows)
	{
		tp.grid.setHeight(windows / tp.grid.width());

		extraGrid.setWidth(windows - tp.grid.width() * tp.grid.height());
		extraGrid.setHeight(extraGrid.width() ? 1 : 0);
	}

	return { tp.grid, extraGrid, tp.window };
}

std::vector<QRect> CalculateGrid(QSize screen, int windows, float aspectRatio, QSize spacing, QSize multiplicity, QSize maxSize)
{
	OptimalTilingParams tiling = CalculateOptimalTiling(screen, windows, aspectRatio, spacing, multiplicity, maxSize);

	QPoint gridOffset(
		(screen.width() - (tiling.mainGrid.width() * (tiling.window + spacing).width() + spacing.width())) / 2,
		(screen.height() - ((tiling.mainGrid + tiling.extraGrid).height() * (tiling.window + spacing).height() + spacing.height())) / 2);
	QPoint spacingOffset(spacing.width(), spacing.height());

	std::vector<QRect> result;

	for (int row = 0; row < tiling.mainGrid.height(); row++)
	{
		QPoint rowOffset(0, (tiling.window + spacing).height() * row);

		for (int col = 0; col < tiling.mainGrid.width(); col++)
		{
			QPoint colOffset((tiling.window + spacing).width() * col, 0);

			QRect rect(gridOffset + rowOffset + colOffset + spacingOffset, tiling.window);

			result.push_back(rect);
		}
	}

	QPoint extraGridOffset(
		(screen.width() - (tiling.extraGrid.width() * (tiling.window + spacing).width() + spacing.width())) / 2,
		gridOffset.y() + tiling.mainGrid.height() * (tiling.window + spacing).height());

	for (int row = 0; row < tiling.extraGrid.height(); row++)
	{
		QPoint rowOffset(0, (tiling.window + spacing).height() * row);

		for (int col = 0; col < tiling.extraGrid.width(); col++)
		{
			QPoint colOffset((tiling.window + spacing).width() * col, 0);

			QRect rect(extraGridOffset + rowOffset + colOffset + spacingOffset, tiling.window);

			result.push_back(rect);
		}
	}

	return result;
}
