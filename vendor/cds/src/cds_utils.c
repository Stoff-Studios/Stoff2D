#include <cds/cds_utils.h>

//////////////////////////////// cmp functions ////////////////////////////////

int cds_cmpi(const void* first, const void* second) {
    int firsti = *((int*) first);
    int secondi = *((int*) second);
    if (firsti > secondi) {
        return 1;
    } 
    if (firsti < secondi) {
        return -1;
    }
    return 0;
}

int cds_cmpu(const void* first, const void* second) {
    unsigned int firstu = *((unsigned int*) first);
    unsigned int secondu = *((unsigned int*) second);
    if (firstu > secondu) {
        return 1;
    } 
    if (firstu < secondu) {
        return -1;
    }
    return 0;
}

int cds_cmpf(const void* first, const void* second) {
    float firstf = *((float*) first);
    float secondf = *((float*) second);
    if (firstf > secondf) {
        return 1;
    } 
    if (firstf < secondf) {
        return -1;
    }
    return 0;
}

int cds_cmpd(const void* first, const void* second) {
    double firstd = *((double*) first);
    double secondd = *((double*) second);
    if (firstd > secondd) {
        return 1;
    } 
    if (firstd < secondd) {
        return -1;
    }
    return 0;
}

//////////////////////////////// vtv functions ////////////////////////////////

char cds_vtvc(void* data) {
    return *((char*) data);
}

int cds_vtvi(void* data) {
    return *((int*) data);
}

unsigned int cds_vtvu(void* data) {
    return *((unsigned int*) data);
}

float cds_vtvf(void* data) {
    return *((float*) data);
}

double cds_vtvd(void* data) {
    return *((double*) data);
}
