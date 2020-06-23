
#include "lelyco-val.h"

#ifdef HAVE_LELY_OVERRIDE

#ifdef LELY_ENABLE_SHARED
#define _GNU_SOURCE
#include <dlfcn.h>
#endif

int lely_override_co_val_read_vc = -1;
int lely_override_co_val_write_vc = -1;

#ifdef LELY_ENABLE_SHARED
typedef size_t co_val_read_t(co_unsigned16_t, void *, const uint_least8_t *,
		const uint_least8_t *);
typedef size_t co_val_write_t(co_unsigned16_t, const void *, uint_least8_t *,
		uint_least8_t *);
#else
extern size_t __real_co_val_read(co_unsigned16_t type, void *val,
		const uint_least8_t *begin, const uint_least8_t *end);
extern size_t __real_co_val_write(co_unsigned16_t type, const void *val,
		uint_least8_t *begin, uint_least8_t *end);
#endif

#ifdef LELY_ENABLE_SHARED
size_t
co_val_read(co_unsigned16_t type, void *val, const uint_least8_t *begin,
		const uint_least8_t *end)
#else
size_t
__wrap_co_val_read(co_unsigned16_t type, void *val, const uint_least8_t *begin,
		const uint_least8_t *end)
#endif
{
	if (lely_override_co_val_read_vc == 0)
		return 0;

	if (lely_override_co_val_read_vc > 0)
		--lely_override_co_val_read_vc;

#ifdef LELY_ENABLE_SHARED
	co_val_read_t *orig_co_val_read;
	*(void **)(&orig_co_val_read) = dlsym(RTLD_NEXT, "co_val_read");
	return orig_co_val_read(type, val, begin, end);
#else
	return __real_co_val_read(type, val, begin, end);
#endif
}

#ifdef LELY_ENABLE_SHARED
size_t
co_val_write(co_unsigned16_t type, const void *val, uint_least8_t *begin,
		uint_least8_t *end)
#else
size_t
__wrap_co_val_write(co_unsigned16_t type, const void *val, uint_least8_t *begin,
		uint_least8_t *end)
#endif
{
	if (lely_override_co_val_write_vc == 0)
		return 0;

	if (lely_override_co_val_write_vc > 0)
		--lely_override_co_val_write_vc;

#ifdef LELY_ENABLE_SHARED
	co_val_write_t *orig_co_val_write;
	*(void **)(&orig_co_val_write) = dlsym(RTLD_NEXT, "co_val_write");
	return orig_co_val_write(type, val, begin, end);
#else
	return __real_co_val_write(type, val, begin, end);
#endif
}

#endif // HAVE_LELY_OVERRIDE
