#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

#include <time.h>
#include <unistd.h>

namespace {

	void usage()
	{
		std::puts("usage: duntil @POSIXTIME COMMAND [ARGS..]");
		std::puts("       duntil +SECONDS COMMAND [ARGS..]");
	}

	// 'double' changes value when time_t over 53-bit
	const double k_time_t_limit =
		double(std::numeric_limits<time_t>::max())
		* (1.0 - std::numeric_limits<double>::epsilon());

	int parseDate(const char* dateStr, timespec& t, bool& isAbsolute)
	{
		switch ( dateStr[0] ) {
		case '+':
			isAbsolute = false;
			break;

		case '@':
			isAbsolute = true;
			break;

		default:
			return EINVAL;
		}
		dateStr += 1;

		char* next = nullptr;
		double secs = std::strtod(dateStr, &next);
		if ( errno == ERANGE ) {
			return ERANGE;
		}
		if ( next[0] != '\0' ) {
			return EINVAL;
		}

		double isecs = 0;
		double mod1 = std::modf(secs, &isecs);
		if ( isecs < 0 || k_time_t_limit < isecs ) {
			return ERANGE;
		}
		t.tv_sec = time_t(isecs);
		t.tv_nsec = long(mod1 * 1000000000.0);

		return 0;
	}

} // namespace

int main(int argc, char* argv[])
{
	if ( argc < 3 ) {
		usage();
		return 1;
	}

	const char* dateStr = argv[1];
	argv += 2;

	timespec date{};
	bool isAbsolute = false;

	int err = parseDate(dateStr, date, isAbsolute);
	if ( err ) {
		std::fprintf(stderr, "%s\n", std::strerror(err));
		usage();
		return 1;
	}
	int flags = isAbsolute ? TIMER_ABSTIME : 0;

	if ( clock_nanosleep(CLOCK_REALTIME, flags, &date, NULL) ) {
		std::perror("clock_nanosleep");
		return 1;
	}
	return execvp(argv[0], argv);
}
