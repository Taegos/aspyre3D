#include "Log.h"
#include <time.h> 
#include <cstdio>
#include <Windows.h>
#include <sstream>


std::ostream* Log::fatal_os = nullptr;
std::ostream* Log::error_os = nullptr;
std::ostream* Log::warning_os = nullptr;
std::ostream* Log::info_os = nullptr;
std::ostream* Log::trace_os = nullptr;

Log::Log(const std::string name) :
	name{name}
{
}

void Log::setup_streams(std::ostream & _trace_os, std::ostream & _info_os, std::ostream & _warning_os, std::ostream & _error_os, std::ostream & _fatal_os) {
	if (trace_os != nullptr && 
		info_os != nullptr &&
		warning_os != nullptr &&
		error_os != nullptr &&
		fatal_os != nullptr) {
		throw new std::exception("The streams has already been setup!");
	}
	trace_os = &_trace_os;
	info_os = &_info_os;
	warning_os = &_warning_os;
	error_os = &_error_os;
	fatal_os = &_fatal_os;
}

std::ostream & Log::fatal()
{
	return put_meta_info(*fatal_os, "FATAL");
}

std::ostream & Log::error()
{
	return put_meta_info(*error_os, "ERROR");
}

std::ostream & Log::warning()
{
	return put_meta_info(*warning_os, "WARNING");
}

std::ostream & Log::info()
{
	return put_meta_info(*info_os, "INFO");
}

std::ostream & Log::trace()
{
	return put_meta_info(*trace_os, "INFO");
}


std::ostream & Log::put_meta_info(std::ostream & out, const std::string & log_type)
{
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	std::ostringstream sstream;
	sstream << "[" << log_type.c_str() << "::" << timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec << "] (" << name << "): ";
	out << sstream.str().c_str();
	return out;
}