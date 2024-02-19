// profiledropdown.cpp

#include "profiledropdown.h"

ProfileDropdown::ProfileDropdown(QWidget *parent) : QWidget(parent)
{
    // Initialiser le QComboBox
    dropdown = new QComboBox(this);
    dropdown->setVisible(true);
    connect(dropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfileDropdown::handleProfileChange);
}

void ProfileDropdown::setProfiles(const QStringList &profiles)
{
    dropdown->clear();
    dropdown->addItems(profiles);

    // Définir le profil actuel sur le premier élément de la liste s'il existe
    if (!profiles.isEmpty()) {
        setCurrentProfile(profiles.first());
    }
}

QString ProfileDropdown::getCurrentProfile() const
{
    return dropdown->currentText();
}

void ProfileDropdown::setCurrentProfile(const QString &profile)
{
    int index = dropdown->findText(profile);
    if (index != -1) {
        dropdown->setCurrentIndex(index);
    }
}

void ProfileDropdown::handleProfileChange(int index)
{
    QString selectedProfile = dropdown->itemText(index);
    emit profileChanged(selectedProfile);
}
