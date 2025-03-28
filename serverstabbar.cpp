#include "serverstabbar.h"

ServersTabBar::ServersTabBar() : QTabBar(), width(30), height(115)
{}

ServersTabBar::ServersTabBar(int tabWidth, int tabHeight) : QTabBar(), width(tabSize.width()), height(tabSize.height())
{}
