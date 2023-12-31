#pragma once

#ifndef PASSWORD_HPP
#define PASSWORD_HPP

// ATTENTION! Below in line 17 you need to insert
// the numbers for the correct password. Find the
// correct numbers via google or have a look at the
// links:
// https://de.ifixit.com/Antworten/Ansehen/513422/nfc+Chips+f%C3%BCr+tonies+kaufen?permalink=answer-617274#answer617274
// https://github.com/toniebox-reverse-engineering/teddy/issues/19
// If the password does not work try to change the
// byte order.

uint8_t passwords[3][4] = {{0x0F, 0x0F, 0x0F, 0x0F},
                           {0x00, 0x00, 0x00, 0x00},
                           {0x??, 0x??, 0x??, 0x??}}; // insert password here

#endif // PASSWORD_HPP
