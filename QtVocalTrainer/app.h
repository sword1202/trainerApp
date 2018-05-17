#ifndef APP_H
#define APP_H


#import <QGuiApplication>

class App : public QGuiApplication
{
public:
    App(int argc, char *argv[]);
    void executeOnMainThread(const std::function<void()>& callback);

    static App* instance();

protected:
    virtual bool event(QEvent *event) override;
};

#endif // APP_H