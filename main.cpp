
/*
 * 1. int main | A simple terminal where an user can defined a server port
 * 2. int initServer | Create the server listening connection
 * 3. static void ev_handler | Server static html files
 */

// include mongoose
#include "mongoose.h";
#include <string>
#include <iostream>

// Struct containing settings for how to server HTTP with Mongoose
static struct mg_serve_http_opts s_http_server_opts;

//create even handler
static void ev_handler (struct mg_connection *nc, int ev, void *p) {
    //if event is http request
    if (ev == MG_EV_HTTP_REQUEST)
    {
        //serve static html file
        mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
    }
}

int initServer(int port){
    //Mongoose event manager
    struct mg_mgr mgr;
    //Mongoose conection
    struct mg_connection *nc;
    //convert port variable to char
    std::string portToChar = std::to_string(port);
    static char const *sPort = portToChar.c_str();
    //Init mongoose
    mg_mgr_init(&mgr, NULL);
    std::cout << "Starting server on Port " << sPort << std::endl;

    nc = mg_bind(&mgr, sPort, ev_handler);

    //If the connection fail
    if (nc == NULL)
    {
        std::cout << "Failed to create listener" << std::endl;
        return 1;
    }

    //Set up HTTP server options
    mg_set_protocol_http_websocket(nc);

    s_http_server_opts.document_root = ".";

    s_http_server_opts.enable_directory_listing = "yes";

    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }

    //free up all memory allocated
    mg_mgr_free(&mgr);
    return 0;
}

int main(void) {
    int port;
    std::cout << "Select server port" << std::endl;
    std::cin >> port;
    //fail case
    if (std::cin.fail())
    {
        port = 1000;
    }
    initServer(port);
    
    return 0;

}
