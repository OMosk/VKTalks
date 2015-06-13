#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H
#include <QSettings>
class SettingsManager
{
    const QString authorizationSettingsFileName = QStringLiteral("auth.conf");
    const QString applicationSettingsFileName = QStringLiteral("app.conf");
    QSettings *authorizationSettings = NULL;
    QSettings *applicationSettings = NULL;
public:
    SettingsManager();
    ~SettingsManager();

    QVariant getAuthorizationParameter(QString parameter);
    QVariant getApplicationParameter(QString parameter);

    void setAuthorizationParameter(QString parameter, QVariant value);
    void setApplicationParameter(QString parameter, QVariant value);

};

#endif // SETTINGSMANAGER_H
