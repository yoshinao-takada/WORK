Basic libraries
1. BL_types.h : basic types of base_l.
2. BL_containers.h : generic container library
3. BL_futils.h : file utilities
4. BL_diag.h : process/thread/time diagnostics
5. BL_debug.h : debugging tools
6. BL_locker.h : pthread mutex wrapper for exclusive control
7. BL_loggerbase.h : logging functions; don't use directly. They are called only by macros in BL_logger.h
8. BL_putils.h : process utilities; creating, waiting for termination, and execute synchronously.