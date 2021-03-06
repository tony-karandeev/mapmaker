// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSizeF>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    static Settings *sharedInstance();

    bool parseCommandLine();

    QString mapFilename() const;
    void setMapFilename(const QString &mapFilename);
    void resetMapFilename();

    QByteArray geometry() const;
    void setGeometry(const QByteArray &geometry);

    QString imagesDirectory() const;
    Q_SLOT void setImagesDirectory(const QString &imagesDirectory);

    bool showGrid() const;
    Q_SLOT void setShowGrid(bool showGrid);

    bool autoSizeGrid() const;
    Q_SLOT void setAutoSizeGrid(bool autoSizeGrid);

    // Grid size affected by autoSizeGrid option
    QSizeF finalGridSize() const;
    // User-configured grid size
    QSizeF gridSize() const;
    void setGridSize(const QSizeF &gridSize);
    Q_SLOT void setGridSize(qreal uniformSize);
    Q_SLOT void setGridSize(int uniformSize);

    QSizeF selectedLevelObjectSize() const;
    void setSelectedLevelObjectSize(const QSizeF &selectedLevelObjectSize);

    bool snapToGrid() const;
    Q_SLOT void setSnapToGrid(bool snapToGrid);

    bool showDockPoints() const;
    Q_SLOT void setShowDockPoints(bool showDockPoints);

    int dockingDistance() const;
    Q_SLOT void setDockingDistance(int dockingDistance);

    bool snapToDockPoints() const;
    Q_SLOT void setSnapToDockPoints(bool snapToDockPoints);

    double zoom() const;
    Q_SLOT void setZoom(double zoom);
    Q_SLOT void setZoom100();
    Q_SLOT void setZoom200();
    Q_SLOT void setZoom300();
    Q_SLOT void setZoom400();

signals:
    void mapFilenameChanged(const QString &mapFilename);
    void geometryChanged(const QByteArray &geometry);
    void imagesDirectoryChanged(const QString &imagesDirectory);
    void showGridChanged(bool showGrid);
    bool autoSizeGridChanged(bool autoSizeGrid);
    void gridSizeChanged(const QSizeF &combinedGridSize);
    void finalGridSizeChanged(const QSizeF &combinedGridSize);
    void snapToGridChanged(bool snapToGrid);
    void showDockPointsChanged(bool showDockPoints);
    void dockingDistanceChanged(int dockingDistance);
    void snapToDockPointsChanged(bool snapToDockPoints);
    void zoomChanged(double zoom);

public slots:
    void save();

protected:
    void load();

    struct {
        QString imagesDirectory_;
    } commandline_;

    QSettings settings_;
    QString mapFilename_;
    QByteArray geometry_;

    // General
    QString imagesDirectory_;

    // Grid
    bool showGrid_ = false;
    bool autoSizeGrid_ = false;
    QSizeF gridSize_;
    QSizeF selectedLevelObjectSize_;
    bool snapToGrid_ = false;

    // Dock Points
    bool showDockPoints_ = false;
    int dockingDistance_ = 6;
    bool snapToDockPoints_ = true;

    // Zoom
    double zoom_ = 1.0;
};

#endif // SETTINGS_H
