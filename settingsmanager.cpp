#include "settingsmanager.h"

SettingsManager::SettingsManager()
{
    authorizationSettings = new QSettings(authorizationSettingsFileName, QSettings::Format::IniFormat);
    applicationSettings = new QSettings(applicationSettingsFileName, QSettings::Format::IniFormat);
}
SettingsManager::~SettingsManager(){
    delete authorizationSettings;
    delete applicationSettings;
}

QVariant SettingsManager::getAuthorizationParameter(QString parameter)
{
    return authorizationSettings->value(parameter);
}

QVariant SettingsManager::getApplicationParameter(QString parameter)
{
    return applicationSettings->value(parameter);
}

void SettingsManager::setAuthorizationParameter(QString parameter, QVariant value)
{
    authorizationSettings->setValue(parameter, value);
}

void SettingsManager::setApplicationParameter(QString parameter, QVariant value)
{
    applicationSettings->setValue(parameter, value);
}
