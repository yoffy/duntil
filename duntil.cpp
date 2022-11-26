#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <time.h>
#include <unistd.h>

namespace {

	void usage()
	{
		std::puts("usage: duntil [+]@POSIXTIME COMMAND [ARGS..]");
	}

	int parseDate(const char* dateStr, timespec& t, bool& isAbsolute)
	{
		std::size_t len = strlen(dateStr);
		const char* end = &dateStr[len];

		if ( len < 2 ) {
			return 1;
		}
		if ( dateStr[0] == '+' ) {
			isAbsolute = false;
			dateStr += 1;
		} else {
			isAbsolute = true;
		}
		if ( dateStr[0] != '@' ) {
			return 1;
		}
		dateStr += 1;

		const char* dot = strchr(dateStr, '.');

		if ( dot ) {
			if ( dateStr == dot ) {
				t.tv_sec = 0;
			} else {
				t.tv_sec = time_t(std::atoll(dateStr));
			}

			const char* p = &dot[1];
			std::ptrdiff_t afterLen = std::min(ptrdiff_t(9), end - p);
			std::uint32_t after = 0;

			for ( ptrdiff_t i = 0; i < afterLen; i += 1 ) {
				char n = p[i];
				if ( n < '0' || '9' < n ) {
					return 1;
				}
				after = after * 10 + (n - '0');
			}
			for ( std::ptrdiff_t i = afterLen; i < 9; i += 1 ) {
				after *= 10;
			}

			t.tv_nsec = after;
		} else {
			t.tv_sec = std::atoll(dateStr);
			t.tv_nsec = 0;
		}

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

	if ( parseDate(dateStr, date, isAbsolute) ) {
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
