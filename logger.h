#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

#include <string>

// Logger class, responsiblew for logging errors, warnings anf infos.
// Sample usage:
//
//	Logger *logger = new Logger();
//	logger->setLogVerbosity(L2);
//	
//	logger->print("text", LOG_INFO, L1); // text will be printed -> L1 <= L2
//	logger->print("text", LOG_INFO, L3); // text will NOT be printed -> L3 > L2
class Logger
{
public:
	enum LogType { LOG_ERROR, LOG_NEW_EVENT, LOG_INFO };

	enum LogVerbosity { L1 = 1, L2, L3, L4 };

	// Prints messages into output stream.
	void Print(const std::string text, LogVerbosity logVerb = L1, LogType logType = LOG_INFO);

	void SetLogVerbosity(LogVerbosity lv);

	LogVerbosity GetLogVerbosity();

	static Logger *GetInstance();

protected:
	Logger();

	virtual ~Logger();

	static Logger *instance_;

	LogVerbosity log_verbosity_;			// Log verbosity.       
};

#endif /* LOGGER_LOGGER_H_ */#pragma once
