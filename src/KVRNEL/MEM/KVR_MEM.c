/*   ---     ---     ---     ---     ---    *
 *  KVR_MEM                                 *
 *                                          *
 *  main memory workhorse for KVR           *
 *  handles allocation and deallocation     *
 *  defines behaviour of base MEM struct    *
 *                                          *
 *   ---     ---     ---     ---     ---    */

#include "KVR_MEM.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//   ---     ---     ---     ---     ---

#define MEMSET(m, op)                       /* a counter. goes up on MKMEM, down on DLMEM  */\
    USED_MEMORY op##= (m->size * m->count)                                                   \
                    + sizeof(MEM);

static uint64_t USED_MEMORY = 0;            // total bytes used by MEM instances

//   ---     ---     ---     ---     ---

void* __kvrmalloc(int count, int size)      {

    void* buff = malloc                     ( count * size          );
    if(!buff)                               { return NULL;          };

    memset                                  ( buff, 0, count * size );

    return buff;                                                                            };

//   ---     ---     ---     ---     ---

int MKMEM(MEM* m)                           { m->buff = __kvrmalloc(m->count, m->size);
    if(m->buff != NULL)                     { MEMSET (m, +   ) return 0;                    };
    return FATAL;                                                                           };

void DLMEM(MEM* m)                          {
    if(m->buff != NULL)                     { m->free(m->buff); m->buff = NULL;
                                              MEMSET (m, -   );                             };
                                                                                            };

void FCMEM(MEM* m)                          {

    if(!m->size ) { m->size  = 1;     }
    if(!m->count) { m->count = 1;     }
    if(!m->free ) { m->free  = &free; }                                                     };

//   ---     ---     ---     ---     ---

void* NVMEM(MEM* m, int p)                  {

    if(p < 0)                               // negative indexing (last element first)
    {
        if  (-p <= m->count)                { m->ptr = m->count + p;                        }
        else                                { m->ptr = 0;                                   };
    }

    else                                    // positive indexing (first element first)
    {
        if  ( p <  m->count)                { m->ptr = p;                                   }
        else                                { m->ptr = m->count - 1;                        };
    };

                                            // cast to char and return buff @ptr
    return                                  ((char*) m->buff) + (m->ptr * m->size);         };

//   ---     ---     ---     ---     ---

