// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSizeF>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    static Settings *sharedInstance();

    bool parseCommandLine();

    QString mapFilename() const;
    void setMapFilename(const QString &mapFilename);

    QString imagesDirectory() const;
    Q_SLOT void setImagesDirectory(const QString &imagesDirectory);

    bool showGrid() const;
    Q_SLOT void setShowGrid(bool showGrid);

    QSizeF gridSize() const;
    void setGridSize(const QSizeF &gridSize);
    Q_SLOT void setGridSize(qreal uniformSize);
    Q_SLOT void setGridSize(int uniformSize);

    bool snapToGrid() const;
    Q_SLOT void setSnapToGrid(bool snapToGrid);

signals:
    void mapFilenameChanged(const QString &mapFilename);
    void showGridChanged(bool showGrid);
    void gridSizeChanged(const QSizeF &gridSize);

public slots:

protected:
    QString mapFilename_;
    QString imagesDirectory_;
    bool showGrid_ = false;
    QSizeF gridSize_;
    bool snapToGrid_ = false;
};

#endif // SETTINGS_H
