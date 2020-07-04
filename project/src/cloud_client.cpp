#include"cloud_client.h"

#define STORE_FILE "./list.backup"
#define LISTEN_DIR "./backup/"
#define SERVER_IP "129.211.69.11"
#define SERVER_PORT 9000

int main()
{
	CloudClent client(LISTEN_DIR, STORE_FILE, SERVER_IP, SERVER_PORT);
	client.Start();
	return 0;
}