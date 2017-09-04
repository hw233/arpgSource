#include "scened_app.h"

#ifdef GOOGLE_PERFTOOLS
#  include "setup_profiler.h"
#endif

#define WELCOME_MSG "Welcome to SCENED,^C to stop!\n--------------------------------------------------------------\n\n"

int main(int argc, char **argv)
{
#if PLATFORM == PLATFORM_WINDOWS
    wsa_initializer(TRUE);
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF  );	
	//_CrtSetBreakAlloc(969991);
#endif

	//打印欢迎信息
	printf("%s\n",WELCOME_MSG);

    int ret = 0;

    //载入配置参数
    SvrCoreParams params;

    if(!params.Load(argc, (const char **)argv, NULL))
        return EXIT_FAILURE;

    //载入配置文件
    ScenedConfig config;

    if(!config.Load(params.config_file, params.centd_port))
        return EXIT_FAILURE;

    //打开应用程序
    ScenedApp::g_app = new ScenedApp(params, config);
    ScenedApp::HookSignals();

    //等待网络线程并直到退出
    if(ScenedApp::g_app->Open())
    {
        ret = EXIT_SUCCESS;

		//等待网络线程并直到退出
		tea_pinfo("scened running!\n----------------------------\n");
        ScenedApp::g_app->Wait();
    }
    else ret = EXIT_SUCCESS;

    //关闭并释放应用程序资源
    ScenedApp::g_app->Close();
    ScenedApp::UnHookSinals();	
    delete ScenedApp::g_app;
	ScenedApp::g_app = NULL;
	
#if PLATFORM == PLATFORM_WINDOWS
    wsa_initializer(FALSE);
#endif
    return ret;
}

