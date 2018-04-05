
#include "loger.hpp"

using namespace zsummer::log4z;
using namespace std;

LoggerId LOGID_TEST;

int setup_loger()
{
	//start log4z
	cout<<"setup loger!!"<<endl;
	LOGID_TEST = ILog4zManager::getRef().createLogger("test" );
   // ILog4zManager::getRef().config("config.cfg");
	ILog4zManager::getRef().setLoggerDisplay(LOG4Z_MAIN_LOGGER_ID, false);
	ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID, false);

	ILog4zManager::getRef().setLoggerDisplay(LOGID_TEST, true);
	ILog4zManager::getRef().setLoggerOutFile(LOGID_TEST, true);
    ILog4zManager::getRef().start();
    cout<<"LOGID_TEST:"<<LOGID_TEST<<("!setup finish!!")<<endl;
    return 0;
}

LoggerId get_logger()
{
    return LOGID_TEST;
}