#pragma once

#define SERIAL_BITS(s) (sizeof(s) * 8);
#define SERIAL_BITS_2(s) (2 << (SERIAL_BITS(s) - 1))

#define sn_less(s1, s2) ((s1 < s2 && s2 - s1 < SERIAL_BITS_2(s1)) || (s1 > s2 && s1 - s2 > SERIAL_BITS_2(s1)))

#define sn_greater(s1, s2) ((s1 < s2 && s2 - s1 > SERIAL_BITS_2(s1)) || (s1 > s2 && s1 - s2 < SERIAL_BITS_2(s1)))

#define sn_equal(s1, s2) (s1 == s2)
