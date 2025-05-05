#include "SceneManager.h"
#include "NetworkManager.h"


void main()
{
	srand(time(NULL));
	if(NETWORK.ConnectServer())
	{
		SCENE.InitScenes(new RegisterScene());
		SCENE.Update();
	}
	NETWORK.DisconnectServer();
}