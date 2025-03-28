#ifndef SERVERSTABBAR_H
#define SERVERSTABBAR_H

#include <QTabBar>
#include <QStylePainter>
#include <QStyleOptionTab>

class ServersTabBar : public QTabBar
{
public:
    ServersTabBar();
    ServersTabBar(int tabWidth, int tabHeight);
    ServersTabBar(QSize tabSize);
    ServersTabBar(QWidget *parent);
    ServersTabBar(QWidget *parent, int tabWidth, int tabHeight);
    ServersTabBar(QWidget *parent, QSize tabSize);
    QSize tabSizeHint(int index) const override;
    ~ServersTabBar();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int width, height;
};

#endif // SERVERSTABBAR_H
