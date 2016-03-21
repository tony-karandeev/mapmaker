// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QMetaMethod>
#include "LevelObject.h"
#include "Utils/FileUtils.h"

LevelObject::LevelObject(QObject *parent)
    : QObject(parent)
    , size_(0, 0)
{
    // TODO: automate?
    connect(this, SIGNAL(positionChanged(QPointF)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(sizeChanged(QSizeF)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(flipXChanged(bool)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(flipYChanged(bool)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(customPropertyChanged(QString,QString)),
            this, SIGNAL(modified()));
}

LevelObject *LevelObject::clone(QObject *parent)
{
    LevelObject *obj = new LevelObject(parent);
    obj->name_ = name_;
    obj->filename_ = filename_;
    obj->image_ = image_;
    obj->position_ = position_;
    obj->size_ = size_;
    obj->flipX_ = flipX_;
    obj->flipY_ = flipY_;
    obj->customProperties_ = customProperties_;
    return obj;
}

QString LevelObject::name() const
{
    return name_;
}

void LevelObject::setName(const QString &name)
{
    if (name_ != name) {
        name_ = name;
        emit nameChanged(name);
    }
}

QString LevelObject::filename() const
{
    return filename_;
}

void LevelObject::setFilename(const QString &filename)
{
    filename_ = filename;
    if (name_.isEmpty())
        name_ = nameFromFilename(filename);
}

QImage LevelObject::image() const
{
    return image_;
}

void LevelObject::setImage(const QImage &image)
{
    image_ = image;
    size_ = image.size();
}

QPointF LevelObject::position() const
{
    return position_;
}

qreal LevelObject::x() const
{
    return position_.x();
}

qreal LevelObject::y() const
{
    return position_.y();
}

void LevelObject::setPosition(const QPointF &pos)
{
    if (position_ != pos) {
        position_ = pos;
        emit positionChanged(pos);
    }
}

void LevelObject::setPosition(qreal x, qreal y)
{
    setPosition(QPointF(x, y));
}

void LevelObject::setX(qreal x)
{
    if (position_.x() != x) {
        position_.setX(x);
        emit positionChanged(position_);
    }
}

void LevelObject::setY(qreal y)
{
    if (position_.y() != y) {
        position_.setY(y);
        emit positionChanged(position_);
    }
}

QSizeF LevelObject::size() const
{
    return size_;
}

void LevelObject::setSize(const QSizeF &size)
{
    if (size_ != size) {
        emit willChangeSize(size);
        size_ = size;
        emit sizeChanged(size_);
    }
}

void LevelObject::setWidth(qreal width)
{
    if (size_.width() != width) {
        setSize(QSize(width, size_.height()));
    }
}

void LevelObject::setHeight(qreal height)
{
    if (size_.height() != height) {
        setSize(QSize(size_.width(), height));
    }
}

bool LevelObject::flipX() const
{
    return flipX_;
}

void LevelObject::setFlipX(bool flipX)
{
    if (flipX_ != flipX) {
        flipX_ = flipX;
        emit flipXChanged(flipX);
    }
}

bool LevelObject::flipY() const
{
    return flipY_;
}

void LevelObject::setFlipY(bool flipY)
{
    if (flipY_ != flipY) {
        flipY_ = flipY;
        emit flipYChanged(flipY);
    }
}

QString LevelObject::customProperty(const QString &key)
{
    Properties::iterator i = customProperties_.find(key);
    if (i == customProperties_.end())
        return QString();
    return i.value();
}

void LevelObject::setCustomProperty(const QString &key, const QString &value)
{
    customProperties_[key] = value;
    emit customPropertyChanged(key, value);
}

void LevelObject::resetCustomProperty(const QString &key)
{
    customProperties_.remove(key);
}

LevelObject::Properties LevelObject::customProperties()
{
    return customProperties_;
}
