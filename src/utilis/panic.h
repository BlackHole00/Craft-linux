#pragma once

#define VX_PANIC(_MESSAGE) vx_panic(__FILE__, __LINE__, __PRETTY_FUNCTION__, _MESSAGE)
#define VX_ASSERT(_MESSAGE, _EQ) { if(!(_EQ)) { VX_PANIC(_MESSAGE); } }
#define VX_CHECK(_EQ, _RET) { if (!(_EQ)) { return _RET; } }
#define VX_PANIC_EXIT_OP(_MESSAGE, _EXIT_OP) _EXIT_OP; VX_PANIC(_MESSAGE);
#define VX_ASSERT_EXIT_OP(_MESSAGE, _EQ, _EXIT_OP) { if(!(_EQ)) { VX_PANIC_EXIT_OP(_MESSAGE, _EXIT_OP); } }
#define VX_CHECK_EXIT_OP(_EQ, _RET, _EXIT_OP) { if (!(_EQ)) { _EXIT_OP; return _RET; } }

void vx_panic(char*, int, const char*, char*);

/*  EXAMPLE:
*       int main() {
*           int a = 0;
*           a += 1;
*           VX_ASSERT("a is not 1!!!", a == 1);
*
*           int b = 3;
*           if (b == 4) {
*             VX_PANIC("b is 4!!!");
*           }
*
*           int c = 100;
*           //  if c != 100 then return -1
*           VX_CHECK(c == 100, -1);
*       }
*/
