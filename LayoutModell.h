#ifndef LAYOUTMODEL_H
#define LAYOUTMODEL_H

#include <QAbstractListModel>
#include <QRect>

class LayoutModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit LayoutModel(QObject *parent = nullptr);
	
	enum {
		IdRole = Qt::UserRole,
		RectRole
	};

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void setUsersNum(int num);

private:
	struct UserWindow
	{
		QString id;
		QRect rect;
	};

	std::vector<UserWindow> Windows;
};

#endif // LAYOUTMODEL_H
