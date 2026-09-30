#ifndef SHARED_H
#define SHARED_H

enum VisualState
{
    ServerDefault = 0,
    ServerStarted,
    ServerStopped,
    ServerDownloading,
    ServerInstalling,
    ServerFinishedInstalling,
};

enum PropTab
{
    Main = 0,
    Config,
    SM,
};

#include <QDesktopServices>
#include <QDir>

#endif // SHARED_H
