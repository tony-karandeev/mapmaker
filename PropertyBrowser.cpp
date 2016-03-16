// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QMouseEvent>
#include <QMenu>
#include <ThirdParty/qtpropertybrowser/src/QtStringPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtIntPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtBoolPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtGroupPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtCheckBoxFactory>
#include <ThirdParty/qtpropertybrowser/src/QtSpinBoxFactory>
#include "PropertyBrowser.h"
#include "Data/LevelObject.h"
#include "Dialogs/NewPropertyDialog/NewPropertyDialog.h"
#include "Utils/Utils.h"

PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QtTreePropertyBrowser(parent)
{
    groupManager_ = new QtGroupPropertyManager(this);
    intManager_ = new QtIntPropertyManager(this);
    connect(intManager_, SIGNAL(valueChanged(QtProperty*,int)),
            this, SLOT(onIntValueChanged(QtProperty*,int)));
    boolManager_ = new QtBoolPropertyManager(this);
    connect(boolManager_, SIGNAL(valueChanged(QtProperty*,bool)),
            this, SLOT(onBoolValueChanged(QtProperty*,bool)));
    stringManager_ = new QtStringPropertyManager(this);
    connect(stringManager_, SIGNAL(valueChanged(QtProperty*,QString)),
            this, SLOT(onStringValueChanged(QtProperty*,QString)));
    customPropertyManager_ = new QtStringPropertyManager(this);
    connect(customPropertyManager_, SIGNAL(valueChanged(QtProperty*,QString)),
            this, SLOT(onCustomPropertyValueChanged(QtProperty*,QString)));

    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
    QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);

    setFactoryForManager(boolManager_, checkBoxFactory);
    setFactoryForManager(intManager_, spinBoxFactory);
    setFactoryForManager(stringManager_, lineEditFactory);
    setFactoryForManager(customPropertyManager_, lineEditFactory);

    // Standard properties
    standardGroup_ = groupManager_->addProperty("Standard");

    QtProperty *name = stringManager_->addProperty("name");
    name->setToolTip("Object name");
    name->setEnabled(false);
    standardGroup_->addSubProperty(name);

    x_ = intManager_->addProperty("x");
    x_->setToolTip("X coordinate");
    standardGroup_->addSubProperty(x_);

    y_ = intManager_->addProperty("y");
    y_->setToolTip("Y coordinate");
    standardGroup_->addSubProperty(y_);

    flipX_ = boolManager_->addProperty("flipX");
    flipX_->setToolTip("Flips the object horizontally");
    standardGroup_->addSubProperty(flipX_);

    flipY_ = boolManager_->addProperty("flipY");
    flipY_->setToolTip("Flips the object vertically");
    standardGroup_->addSubProperty(flipY_);

    // Custom properties
    customGroup_ = groupManager_->addProperty("Custom");

    // Final setup
    //addProperty(standardGroup);
    //addProperty(customGroup);

    QAction *addPropertyAction = new QAction(tr("&Add property"), this);
    addPropertyAction->setEnabled(false);
    connect(addPropertyAction, SIGNAL(triggered(bool)),
            this, SLOT(onAddProperty()));
    connect(this, SIGNAL(hasLevelObject(bool)),
            addPropertyAction, SLOT(setEnabled(bool)));
    addAction(addPropertyAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void PropertyBrowser::setLevelObject(LevelObject *object)
{
    if (levelObject_)
        disconnectFromPropertiesOf(levelObject_);

    levelObject_ = object;

    // Remove all properties from browser
    clear();

    // Delete custom properties because they're recreated
    // for different levelObjects
    foreach (QtProperty *property, customGroup_->subProperties()) {
        customGroup_->removeSubProperty(property);
        delete property;
    }

    if (levelObject_)
        connectToPropertiesOf(levelObject_);

    emit hasLevelObject(levelObject_ != nullptr);
}

void PropertyBrowser::resetLevelObject()
{
    setLevelObject(nullptr);
}

void PropertyBrowser::onAddProperty()
{
    NewPropertyDialog *dialog = new NewPropertyDialog(this);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setWindowFlags((dialog->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    if (QDialog::Accepted == dialog->exec() && levelObject_) {
        QtProperty *property = customPropertyManager_->addProperty(
                    dialog->key());
        customGroup_->addSubProperty(property);
        customPropertyManager_->setValue(property, dialog->value());
    }
    delete dialog;
}

void PropertyBrowser::onIntValueChanged(QtProperty *property, int val)
{
    if (property->propertyName() == "x")
        levelObject_->setX(val);
    else if (property->propertyName() == "y")
        levelObject_->setY(val);
}

void PropertyBrowser::onBoolValueChanged(QtProperty *property, bool val)
{
    // TODO: evaluate possibility of using Q_PROPERTIES for automating this
    if (property->propertyName() == "flipX")
        levelObject_->setFlipX(val);
    else if (property->propertyName() == "flipY")
        levelObject_->setFlipY(val);
}

void PropertyBrowser::onStringValueChanged(QtProperty *property, const QString &val)
{
    Q_UNUSED(property);
    Q_UNUSED(val);
}

void PropertyBrowser::onCustomPropertyValueChanged(QtProperty *property, const QString &val)
{
    levelObject_->setCustomProperty(property->propertyName(),
                                    val);
}

void PropertyBrowser::updatePosition(const QPointF &pos)
{
    intManager_->setValue(x_, pos.x());
    intManager_->setValue(y_, pos.y());
}

void PropertyBrowser::updateFlipX(bool flipX)
{
    boolManager_->setValue(flipX_, flipX);
}

void PropertyBrowser::updateFlipY(bool flipY)
{
    boolManager_->setValue(flipY_, flipY);
}

void PropertyBrowser::onCustomPropertyChanged(const QString &key, const QString &value)
{
    foreach (QtProperty *property, customGroup_->subProperties()) {
        if (property->propertyName() != key)
            continue;
        customPropertyManager_->setValue(property, value);
    }
}

void PropertyBrowser::connectToPropertiesOf(LevelObject *object)
{
    addProperty(standardGroup_);
    addProperty(customGroup_);

    LevelObject::Properties properties = object->customProperties();
    for (LevelObject::Properties::iterator i = properties.begin();
         i != properties.end(); ++i) {
        QtProperty *property = customPropertyManager_->addProperty(i.key());
        customGroup_->addSubProperty(property);
        customPropertyManager_->setValue(property, i.value());
    }

    connect(object, SIGNAL(positionChanged(QPointF)),
            this, SLOT(updatePosition(QPointF)));
    updatePosition(levelObject_->position());

    connect(object, SIGNAL(flipXChanged(bool)),
            this, SLOT(updateFlipX(bool)));
    updateFlipX(levelObject_->flipX());

    connect(object, SIGNAL(flipYChanged(bool)),
            this, SLOT(updateFlipY(bool)));
    updateFlipY(levelObject_->flipY());

    connect(object, SIGNAL(customPropertyChanged(QString,QString)),
            this, SLOT(onCustomPropertyChanged(QString,QString)));

    connect(object, SIGNAL(destroyed(QObject*)),
            this, SLOT(resetLevelObject()));
}

void PropertyBrowser::disconnectFromPropertiesOf(LevelObject *object)
{
    disconnect(object, nullptr,
               this, nullptr);
}
