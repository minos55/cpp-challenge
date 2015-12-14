#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <zmq.h>
#include "zmq.hpp"
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)    Sleep(n)
#endif

using namespace cv;
using namespace std;

//Reads the requests of the client
int recvAsInt(zmq::socket_t & socket)
{
    zmq::message_t message;
    if(!socket.recv(&message))
        throw error_t();

    return *(static_cast<int*>(message.data()));
}

//Sends reply in form of a number
bool sendReply(zmq::socket_t & socket,int value, int flags =0)
{
    zmq::message_t msg(sizeof(int));
    memcpy(msg.data(), &value, sizeof(int));
    return socket.send(msg, flags);
}

//Sends reply in form of a messege
static bool
s_send (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message);
    return (rc);
}

//Gets Ip's of server
string getIps()
{
    string Adresses;
    char          buf[1024];
    struct ifconf ifc;
	struct ifreq *ifr;
	int           sck,nInterfaces;

    // Get a socket handle.
	sck = socket(AF_INET, SOCK_DGRAM, 0);
	if(sck < 0)
	{
		perror("socket");
	}

    //Query available interfaces.
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
	{
		perror("ioctl(SIOCGIFCONF)");
	}

    //Iterate through the list of interfaces.
	ifr         = ifc.ifc_req;
	nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
	for(int i = 0; i < nInterfaces; i++)
	{
		struct ifreq *item = &ifr[i];

	//Show the IP address
		Adresses+=(inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
        if(i+1<nInterfaces)
        Adresses+=";";
	}

    return Adresses;
}

int main()
{
    int Request;
    string ipAdresses=getIps();
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5000");
    vector<char> inMsg;
    Mat img;
    vector<Mat> channels;
    Scalar m;
    int more;
    try
    {
        while (true)
        {
            zmq::message_t request;
            Request=recvAsInt(socket);

            //Request detection and reaction
            switch(Request)
            {
                case 700:
                    sendReply(socket,200, ZMQ_SNDMORE);
                    s_send(socket,ipAdresses);
                    break;
                case 701:
                    sendReply(socket,200, ZMQ_SNDMORE);
                    s_send(socket,"pong");
                    break;
                case 702:
                    socket.recv(&request);
                    if(request.size()%8==0)
                    {
                        inMsg.resize(request.size());
                        memcpy(inMsg.data(), request.data(), request.size());
                        img=imdecode(inMsg,CV_LOAD_IMAGE_GRAYSCALE);
                        inMsg.clear();
                    }

                    if(!img.empty())
                    {
                        m= mean(img);
                        img.release();
                        sendReply(socket,200, ZMQ_SNDMORE);
                        sendReply(socket,m.val[0]);
                        break;
                    }
                default:
                    while(1)
                    {
                        more=0;
                        size_t more_size = sizeof (more);
                        socket.getsockopt (ZMQ_RCVMORE, &more, &more_size);

                        if (more)
                            socket.recv(&request);
                        else
                            break;
                    }
                    sendReply(socket,400, ZMQ_SNDMORE);
                    s_send(socket,"String containing an error message");
                    break;
            }
            //  Do some 'work'
            sleep(1);
        }
    }
catch (int e)
{
    cout << "An exception occurred. Exception Nr. " << e << '\n';
}
    return 0;
}
