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
//        File Name: Queue.c
///////////////////////////////////////////////////////////////

#include "typedef.h"
#include "queue.h"

#if 0
static int QUEUE_GetWritePointer(Queue* queue, unsigned int count)
{
	int nPos = queue->m_WritePointer;
	while(count--)
	{
		nPos += queue->m_nElementSize;
		if(nPos >= queue->m_nBufferSize)
		{
			nPos = 0;
		}
		if(nPos == queue->m_ReadPointer)
		{
			nPos = -1;
		}
	}
	return nPos;
}

Bool QUEUE_add(Queue* queue, void* element)
{
	int pointer = queue->m_WritePointer;

	if(queue->m_isFull) return False;

	//For multi thread
	if(queue->m_isWriteLock)
	{
		pointer = QUEUE_GetWritePointer(queue, queue->m_count+1);
		if(pointer >= 0)
		{
			queue->m_count++;
			memcpy(&queue->m_pBuffer[pointer], element, queue->m_nElementSize);
		}
		else
		{
			return False;
		}
		return True;
	}

	//LOCK
	queue->m_isWriteLock = True;

	memcpy(&queue->m_pBuffer[pointer], element, queue->m_nElementSize);
	queue->m_isEmpty = False;

	pointer = QUEUE_GetWritePointer(queue, queue->m_count + 1);
	if(pointer >= 0)
	{
		queue->m_WritePointer = pointer;
	}
	else
	{
		queue->m_WritePointer = queue->m_ReadPointer;
		queue->m_isFull = True;
	}
	queue->m_count = 0;
	
	//UNLOCK
	queue->m_isWriteLock = False;

	return True;
}
#else

Bool QUEUE_addByte(Queue* queue, uint8 byte)
{
	if(queue->m_isFull) return False;

	queue->m_pBuffer[queue->m_WritePointer] = byte;

	queue->m_WritePointer += queue->m_nElementSize;
	if(queue->m_WritePointer >= queue->m_nBufferSize)
	{
		queue->m_WritePointer = 0;
	}
	
	queue->m_isEmpty = False;
	if(queue->m_WritePointer == queue->m_ReadPointer)
	{
		queue->m_isFull = True;
	}
	
	return True;
}


Bool QUEUE_add(Queue* queue, void* element)
{
	if(queue->m_isFull) return False;

	memcpy(&queue->m_pBuffer[queue->m_WritePointer], element, queue->m_nElementSize);

	queue->m_WritePointer += queue->m_nElementSize;
	if(queue->m_WritePointer >= queue->m_nBufferSize)
	{
		queue->m_WritePointer = 0;
	}
	
	queue->m_isEmpty = False;
	if(queue->m_WritePointer == queue->m_ReadPointer)
	{
		queue->m_isFull = True;
	}
	
	return True;
}

#endif

void* QUEUE_getHead(Queue* queue)
{
	if(queue->m_isEmpty)
	{
		return Null;
	}
	
	return &queue->m_pBuffer[queue->m_ReadPointer];
}

//Pop the head element;
void QUEUE_removeHead(Queue* queue)
{
	if(queue->m_isEmpty)
	{
		return ;
	}
	
	queue->m_ReadPointer += queue->m_nElementSize;
	if(queue->m_ReadPointer >= queue->m_nBufferSize)
	{
		queue->m_ReadPointer = 0;
	}
	
	queue->m_isFull = False;
	if(queue->m_ReadPointer == queue->m_WritePointer)
	{
		queue->m_isEmpty = True;
	}
}

void QUEUE_removeAll(Queue* queue)
{
	queue->m_ReadPointer = 0;
	queue->m_WritePointer = 0;
	queue->m_isEmpty = True;
	queue->m_isFull = False;
}

//Get the head element and pop it
void* QUEUE_popGetHead(Queue* queue)
{
	void* pvalue = QUEUE_getHead(queue);
	QUEUE_removeHead(queue);
	return pvalue;
}


//Is queue empty 
//return: 1=Empty 0=Not Empty
Bool QUEUE_isEmpty(Queue* queue)
{
	return queue->m_isEmpty;
}

Bool QUEUE_isFull(Queue* queue)
{
	return queue->m_isFull;
}

int QUEUE_GetCount(Queue* queue)
{
	int count = 0;
	if(queue->m_WritePointer > queue->m_ReadPointer)
	{
		return (queue->m_WritePointer - queue->m_ReadPointer) / queue->m_nElementSize;
	}
	else if(queue->m_ReadPointer > queue->m_WritePointer)
	{
		count = queue->m_nBufferSize - queue->m_ReadPointer + queue->m_WritePointer;
		return count / queue->m_nElementSize;
	}
	else if(queue->m_isEmpty)
	{
		return 0;
	}
	else	//Full
	{
		return queue->m_nBufferSize / queue->m_nElementSize;
	}
}

Bool QUEUE_init(Queue* queue, void* pBuffer, unsigned short itemSize, unsigned short itemCount)
{
	memset(queue, 0, sizeof(Queue));
	queue->m_isUsed = 1;

	queue->m_pBuffer = pBuffer;	
	queue->m_isEmpty = True;
	queue->m_nElementSize = itemSize;
	queue->m_nBufferSize = itemSize * itemCount;

	return True;
}

void QUEUE_reset(Queue* queue)
{
	queue->m_ReadPointer = 0;
	queue->m_WritePointer = 0;
	queue->m_isEmpty = True;
	queue->m_isFull = False;
	memset(queue->m_pBuffer, 0, queue->m_nBufferSize);
}
