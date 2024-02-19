// profiledropdown.h
#ifndef PROFILEDROPDOWN_H
#define PROFILEDROPDOWN_H

#include <QWidget>
#include <QComboBox>


class ProfileDropdown : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileDropdown(QWidget *parent = nullptr);
    void setProfiles(const QStringList &profiles);
    QString getCurrentProfile() const;
    void setCurrentProfile(const QString &profile);

signals:
    void profileChanged(const QString &newProfile);

private:
    QComboBox *dropdown;

private slots:
    void handleProfileChange(int index);
};

#endif // PROFILEDROPDOWN_H
