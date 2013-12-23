///////////////////////////////////////////////////////////////
//                                Copyright (c) 2008 Symwave, Inc.
//
//        This material is the exclusive and confidential property of
//        Symwave, Inc. and may not be reproduced, stored in a retrieval
//        system, or otherwise transmitted, in any form or by any means,
//        electronic, mechanical, recording, or otherwise, without the
//        prior written permission from Symwave, Inc.
//
//        All rights reserved
//
//        File Name: queue.h
///////////////////////////////////////////////////////////////


#ifndef QUEUE_H
#define QUEUE_H     

#include "typedef.h"

#define QUEUE_MAX_QUEUE              4
#define MAX_STATIC_QUEUE             8
//#define QUEUE_MAX_ELEMENT            32

typedef struct _Queue
{
    int m_ReadPointer;
    int m_WritePointer;
	unsigned short	m_nElementSize;		//Element size

    unsigned char m_isUsed:1;
    unsigned char m_isEmpty:1;
    unsigned char m_isFull:1;
    unsigned char m_isWriteLock:1;
    unsigned char m_count:4;
	
	int	m_nBufferSize;
    uint8* m_pBuffer;
}Queue;

//void* QUEUE_getNew(Queue* queue);
Bool QUEUE_addByte(Queue* queue, uint8 byte);
Bool QUEUE_add(Queue* queue, void* element);
Queue* QUEUE_create(int siz);

//Get the head element of queue
void* QUEUE_getHead(Queue* queue);

//Remove the head element;
void QUEUE_removeHead(Queue* queue);

//Get the head element and remove it
void* QUEUE_popGetHead(Queue* queue);

//Is queue empty 
//return: 1=Empty 0=Not Empty
Bool QUEUE_isEmpty(Queue* queue);
Bool QUEUE_isFull(Queue* queue);
void QUEUE_removeAll(Queue* queue);

Bool QUEUE_init(Queue* queue, void* pBuffer, unsigned short itemSize, unsigned short itemCount);
void QUEUE_reset(Queue* queue);
int QUEUE_GetCount(Queue* queue);

#endif

