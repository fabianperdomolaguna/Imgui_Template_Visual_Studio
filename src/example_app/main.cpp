#include <compare>

import Application;
import MainMenuBar;
import RenderScene;

int main()
{
    Application* app = new Application("ImGui - OpenGL Context");

    app->PushLayerApp<MainMenuBar>();
    app->PushLayerApp<SimpleRender>();

    app->Run();

    delete app;
}