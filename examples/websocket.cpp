#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/JsonController.h>

using namespace std;
using namespace Mongoose;

class MyController : public WebController
{
    public: 
        void webSocketReady(WebSocket *websocket)
        {
            cout << "Opening new websocket on " << websocket->getRequest().getUrl() << endl;
            websocket->send("server ready");
        }

        void webSocketData(WebSocket *websocket, string data)
        {
            cout << "[recv] " << data << endl;
            websocket->send(data);

            if (data == "exit") {
                cout << "Client exiting" << endl;
               websocket->close();
            }
        }
};

volatile static bool running = true;

void handle_signal(int sig)
{
    if (running) {
        cout << "Exiting..." << endl;
        running = false;
    }
}

int main()
{
    srand(time(NULL));

    signal(SIGINT, handle_signal);

    MyController myController;
    Server server(8080);
    server.registerController(&myController);
    server.setOption("enable_directory_listing", "false");
    server.setOption("document_root", "websocket_html_root");
    server.start();

    cout << "Server started, routes:" << endl;
    myController.dumpRoutes();

    while (running) {
        sleep(1);
    }

    server.stop();

    return EXIT_SUCCESS;
}