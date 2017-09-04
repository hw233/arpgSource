#include <net/netstd.h>
#include "robotd_app.h"
/*#include "action/FindGridPath.h"*/
#ifdef GOOGLE_PERFTOOLS
#  include "setup_profiler.h"
#endif

#define WELCOME_MSG "Welcome to ROBOT,^C to stop!\n--------------------------------------------------------------\n\n"
int main(int argc, const char **argv)
{
#if PLATFORM == PLATFORM_WINDOWS
	wsa_initializer(TRUE);	
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF  );	
	//_CrtSetBreakAlloc(393678);//分配断点
#endif



	//打印欢迎信息
	printf("%s\n",WELCOME_MSG);

	//载入配置参数
	SvrCoreParams params;
	if(!params.Load(argc,(const char **)argv,NULL))
		return EXIT_FAILURE;

	//载入配置文件
	RobotConfig config;
	if(!config.Load(params.config_file, params.centd_port))
		return EXIT_FAILURE;

	//打开应用程序
	RobotdApp::g_app = new RobotdApp(params,config);
	//RobotdApp::HookSignals();
	if(!RobotdApp::g_app->Open())
	{
		//RobotdApp::UnHookSinals();
		delete RobotdApp::g_app;
		return EXIT_FAILURE;
	}

	//等待网络线程并直到退出
	tea_pinfo("appd running!\n----------------------------\n");				
	RobotdApp::g_app->RunLoop();

	//关闭并释放应用程序资源
	RobotdApp::g_app->Close();
	//RobotdApp::UnHookSinals();
	safe_delete(RobotdApp::g_app);

#if PLATFORM == PLATFORM_WINDOWS
	wsa_initializer(FALSE);	
#endif
	return 0;
}

