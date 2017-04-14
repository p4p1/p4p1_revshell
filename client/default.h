#ifndef _DEFAULT_H_
#define _DEFAULT_H_

#define DEFAULT_PORT 4441 // port to connect to
#define DEFAULT_IP "127.0.0.1"   // ip to connect to
#define DEFAULT_IP_SIZE (strlen(DEFAULT_IP) + 1)	// size of ip to allocate it later
#define TEXT_FILE_OFF
#define _DEBUG

#if defined(TEXT_FILE_ON)
	#define TEXT_CONTENT "STUFF IN FILE"
	#define FILE_NAME "NAME_O_FILE.TXT"
#endif
#endif
