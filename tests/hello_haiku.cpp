#include <Application.h>
#include <Window.h>
#include <View.h>
#include <StringView.h>

class MyView : public BView {
public:
    MyView(BRect frame)
        : BView(frame, "MyView", B_FOLLOW_ALL, B_WILL_DRAW)
    {
        SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
        BStringView* stringView = new BStringView(BRect(10, 10, 100, 30), "greeting", "Hello, Haiku!");
        AddChild(stringView);
    }
};

class MyWindow : public BWindow {
public:
    MyWindow()
        : BWindow(BRect(100, 100, 300, 200), "Hello Haiku", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
    {
        MyView* myView = new MyView(Bounds());
        AddChild(myView);
    }

    virtual bool QuitRequested()
    {
        be_app->PostMessage(B_QUIT_REQUESTED);
        return true;
    }
};

class MyApplication : public BApplication {
public:
    MyApplication()
        : BApplication("application/x-vnd.Hello-Haiku")
    {
        MyWindow* myWindow = new MyWindow();
        myWindow->Show();
    }
};

int main()
{
    MyApplication app;
    app.Run();
    return 0;
}
