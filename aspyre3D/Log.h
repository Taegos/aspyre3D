#pragma once
#include <ostream>
#include <iostream>
#include <vector>
#include <fstream> 

class NoStream : public std::streambuf
{
public:
	int overflow(int c) { return c; }
};

class Log {
public:

	Log(const std::string);
	Log(const Log&) = delete;
	Log(const Log&&) = delete;
	Log & operator=(const Log&) = delete;

	static void setup_streams(std::ostream & trace_ostream, std::ostream & info_ostream, std::ostream & warning_ostream, std::ostream & error_ostream, std::ostream & fatal_ostream);

	//https://stackoverflow.com/questions/2031163/when-to-use-the-different-log-levels

	std::ostream & fatal();
	std::ostream & error();
	std::ostream & warning();
	std::ostream & info();
	std::ostream & trace();
	//std::ostream & debug();

private:
	std::ostream & put_meta_info(std::ostream &, const std::string &);
	const std::string name;

	static std::string focused;
	static std::ostream * fatal_os;
	static std::ostream * error_os;
	static std::ostream * warning_os;
	static std::ostream * info_os;
	static std::ostream * trace_os;
};