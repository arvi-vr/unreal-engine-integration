#pragma once

typedef size_t REQUEST_ID;
typedef void* RESPONSE_ID;
typedef void* MESSAGE_ID;

#define INVALID_ID ((size_t)-1)

typedef enum {
	REQUEST_UNKNOWN = 0,
	REQUEST_SIMPLE = 1,
} REQUEST_TYPE;


typedef enum
{
	TEAM_PUBLIC = 0,
	TEAM_1 = 1,
	TEAM_2 = 2,
	TEAM_3 = 3,
	TEAM_4 = 4,
	TEAM_5 = 5,
	TEAM_6 = 6,
	TEAM_7 = 7,
	TEAM_8 = 8,
	TEAM_9 = 9,
	TEAM_10 = 10,
} TEAM_ID;

typedef enum {
	ERROR_NONE = 0,
	ERROR_UNKNOWN = -1,
	ERROR_BAD_RESPONSE_ID = -2,
	ERROR_REQUEST_LIMIT = -3,

	ERROR_LOAD_APP_KEY = 1,
	ERROR_CREATE_AUTH_DATA = 2,
	ERROR_AUTHENTICATION = 3,

	ERROR_REQUEST_SEND = 4,
	
	ERROR_HTTP = 5
} ERROR_CODE;
