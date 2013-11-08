#ifndef LOGGER_H_
#define LOGGER_H_


#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "PowerFailureAlarm.h"

// some help https://github.com/mrRobot62/Arduino-logging-library/blob/master/Logging.h

#define LOG_LEVEL_NOOUTPUT 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_VERBOSE 5

// default loglevel if nothing is set from user
#define LOGLEVEL LOG_LEVEL_DEBUG

#if LOG_LEVEL >= LOG_LEVEL_ERROR
	#define ERROR(_message) LOG(_message, LOG_LEVEL_ERROR);
#else
	#define ERROR(_message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
	#define WARNING(_message) LOG(_message, LOG_LEVEL_WARNING);
#else
	#define WARNING(_message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
	#define INFO(_message) LOG(_message, LOG_LEVEL_INFO);
#else
	#define INFO(_message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
	#define DEBUG(_message) LOG(_message, LOG_LEVEL_DEBUG);
#else
	#define DEBUG(_message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
	#define VERBOSE(_message) LOG(_message, LOG_LEVEL_VERBOSE);
#else
	#define VERBOSE(_message)
#endif

#define LOG(_message, _logLevel) {Logger::getInstance()->log(String(__LINE__) + ":" + _message, MODULE_ID,_logLevel);};

#define CR "\r\n"


class Logger
{
private:
	Stream *m_loggingStream;
	bool m_serial;
	int m_logLevel;
	static Logger *m_instance;
  
public:
	Logger(Stream *_loggingStream, int _logLevel = LOG_LEVEL_WARNING, bool _serial = true);
	void error(String _message, int _moduleId);
	void warning(String _message, int _moduleId);
	void info(String _message, int _moduleId);
	void debug(String _message, int _moduleId);
	void verbose(String _message, int _moduleId);
	void log(String _message, int _moduleId, int _logLevel = LOG_LEVEL_INFO);
	void log(int _value, int _moduleId = 0, int _signalId = 0, int _logLevel = LOG_LEVEL_INFO);
	Stream *getStream();
	static Logger *getInstance();
	bool ready();
};


#endif
