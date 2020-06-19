
#define _GNU_SOURCE
#include <dlfcn.h>

#include "lelyco_val.h"

int override_co_val_read_vc = -1;
int override_co_val_write_vc = -1;

typedef size_t co_val_read_t(co_unsigned16_t, void *, const uint_least8_t *,
		const uint_least8_t *);
typedef size_t co_val_write_t(co_unsigned16_t, const void *, uint_least8_t *,
		uint_least8_t *);

size_t
co_val_read(co_unsigned16_t type, void *val, const uint_least8_t *begin,
		const uint_least8_t *end)
{
	if (override_co_val_read_vc == 0)
		return 0;

	if (override_co_val_read_vc > 0)
		--override_co_val_read_vc;

	co_val_read_t *orig_co_val_read;
	*(void **)(&orig_co_val_read) = dlsym(RTLD_NEXT, "co_val_read");
	return orig_co_val_read(type, val, begin, end);
}

size_t
co_val_write(co_unsigned16_t type, const void *val, uint_least8_t *begin,
		uint_least8_t *end)
{
	if (override_co_val_write_vc == 0)
		return 0;

	if (override_co_val_write_vc > 0)
		--override_co_val_write_vc;

	co_val_write_t *orig_co_val_write;
	*(void **)(&orig_co_val_write) = dlsym(RTLD_NEXT, "co_val_write");
	return orig_co_val_write(type, val, begin, end);
}
