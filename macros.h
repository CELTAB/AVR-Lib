#ifndef MACROS_H
#define MACROS_H

#define BV(bit)               (1 << bit)
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit))
#define toggle_bit(sfr, bit)  (_SFR_BYTE(sfr) ^= BV(bit))

#endif
