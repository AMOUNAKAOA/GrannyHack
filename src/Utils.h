#ifndef UTILS_H
#define UTILS_H
#include "Dobby/dobby.h"
typedef unsigned long DWORD;
static DWORD libBase;

uintptr_t getLibraryBase(const char *libName);
uintptr_t getAbsoluteAddress(const char *libName, uintptr_t relativeAddr);
bool isLibraryLoaded(const char *libraryName);
void hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr);


uintptr_t getLibraryBase(const char *libName) {
    uintptr_t retAddr = 0;

    char fileName[255];
    memset(fileName, 0, sizeof(fileName));

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    snprintf(fileName, sizeof(fileName), "/proc/%d/maps", getpid());
    FILE *fp = fopen(fileName, "rt");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, libName) != NULL) {
                retAddr = (uintptr_t) strtoul(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return retAddr;
}

uintptr_t getAbsoluteAddress(const char *libName, uintptr_t relativeAddr) {
    uintptr_t base = getLibraryBase(libName);
    if (base == 0)
        return 0;
    return (base + relativeAddr);
}



bool isLibraryLoaded(const char *libraryName)
{
    char line[512] = {0};
    FILE *fp = fopen("/proc/self/maps", "rt");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, libraryName))
            {
                fclose(fp);
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}
void hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
{
    DobbyHook(orig_fcn, new_fcn, orig_fcn_ptr);
}

#endif
