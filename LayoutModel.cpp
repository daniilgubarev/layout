#include "LayoutModell.h"

#include "GridCalculator.h"

LayoutModel::LayoutModel(QObject* parent)
	: QAbstractListModel(parent)
{
}

int LayoutModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return 0;

	return Windows.size();
}

QVariant LayoutModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	switch (role)
	{
		case IdRole:
		{
			return Windows[index.row()].id;
		}

		case RectRole:
		{
			return Windows[index.row()].rect;
		}
	}

	return QVariant();
}

QHash<int, QByteArray> LayoutModel::roleNames() const
{
	QHash<int, QByteArray> names;
	names[IdRole] = "id";
	names[RectRole] = "rect";

	return names;
}

void LayoutModel::setUsersNum(int num)
{
	UserNum = num;

	CalculateLayout();
}

void LayoutModel::setScreenSize(const QSize& screen)
{
	ScreenSize = screen;

	emit screenSizeChanged();

	CalculateLayout();
}

QSize LayoutModel::screenSize() const
{
	return ScreenSize;
}

void LayoutModel::CalculateLayout()
{
	auto rects = CalculateGrid(ScreenSize, UserNum, 16.f / 9.f, { 5, 2 }, { 2, 2 }, ScreenSize);

	beginResetModel();

	Windows.clear();

	for (auto r : rects)
		Windows.push_back({ QString::number(Windows.size()), r });

	endResetModel();
}
