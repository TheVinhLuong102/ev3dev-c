
/*  ev3_port.c was generated by yup.py (yupp) 0.8b3
    out of ev3_port.yu-c at 2015-06-26 16:32
 *//**
 *  \file  ev3_port.c (ev3_port.yu-c)
 *  \brief  EV3 Ports.
 *  \author  Vitaly Kravtsov (in4lio@gmail.com)
 *  \copyright  See the LICENSE file.
 */

#define EV3_PORT_IMPLEMENT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modp_numtoa.h"
#include "ev3.h"
#include "ev3_port.h"

#define PATH_PREF_LEN  25
#define _ID_SPOT  "///"

#define PATH_DRIVER_NAME  "/sys/class/lego-port/port" _ID_SPOT "driver_name"
#define PATH_MODE  "/sys/class/lego-port/port" _ID_SPOT "mode"
#define PATH_MODES  "/sys/class/lego-port/port" _ID_SPOT "modes"
#define PATH_PORT_NAME  "/sys/class/lego-port/port" _ID_SPOT "port_name"
#define PATH_SET_DEVICE  "/sys/class/lego-port/port" _ID_SPOT "set_device"
#define PATH_STATUS  "/sys/class/lego-port/port" _ID_SPOT "status"

size_t get_port_driver_name( uint8_t sn, char *buf, size_t sz )
{
	char s[] = PATH_DRIVER_NAME;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_read_char_array( s, buf, sz );
}

size_t get_port_mode( uint8_t sn, char *buf, size_t sz )
{
	char s[] = PATH_MODE;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_read_char_array( s, buf, sz );
}

size_t set_port_mode( uint8_t sn, char *value )
{
	char s[] = PATH_MODE;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_write_char_array( s, value );
}

size_t get_port_modes( uint8_t sn, char *buf, size_t sz )
{
	char s[] = PATH_MODES;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_read_char_array( s, buf, sz );
}

size_t get_port_port_name( uint8_t sn, char *buf, size_t sz )
{
	char s[] = PATH_PORT_NAME;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_read_char_array( s, buf, sz );
}

size_t set_port_set_device( uint8_t sn, char *value )
{
	char s[] = PATH_SET_DEVICE;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_write_char_array( s, value );
}

size_t get_port_status( uint8_t sn, char *buf, size_t sz )
{
	char s[] = PATH_STATUS;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_read_char_array( s, buf, sz );
}

const char *ev3_port_type( uint8_t type_inx )
{
	switch ( type_inx ) {
	case HT_NXT_SMUX_PORT:
		return "ht-nxt-smux-port";
	case LEGOEV3_INPUT_PORT:
		return "legoev3-input-port";
	case LEGOEV3_OUTPUT_PORT:
		return "legoev3-output-port";
	case MS_EV3_SMUX_PORT:
		return "ms-ev3-smux-port";
	case WEDO_PORT:
		return "wedo-port";

	}
	return ( STR_unknown_ );
}

uint8_t get_port_type_inx( uint8_t sn )
{
	char buf[ 64 ];

	if ( !get_port_driver_name( sn, buf, sizeof( buf ))) return ( PORT_TYPE__NONE_ );

	if ( strcmp( buf, "ht-nxt-smux-port" ) == 0 ) return HT_NXT_SMUX_PORT;
	if ( strcmp( buf, "legoev3-input-port" ) == 0 ) return LEGOEV3_INPUT_PORT;
	if ( strcmp( buf, "legoev3-output-port" ) == 0 ) return LEGOEV3_OUTPUT_PORT;
	if ( strcmp( buf, "ms-ev3-smux-port" ) == 0 ) return MS_EV3_SMUX_PORT;
	if ( strcmp( buf, "wedo-port" ) == 0 ) return WEDO_PORT;

	return ( PORT_TYPE__UNKNOWN_ );
}

uint8_t get_port_port_inx( uint8_t sn, uint8_t port_type_inx, uint8_t *extport )
{
	char buf[ 32 ];

	if ( !get_port_port_name( sn, buf, sizeof( buf ))) return ( EV3_PORT__NONE_ );

	return ( ev3_port_inx( port_type_inx, buf, extport ));
}

EV3_PORT *ev3_port_desc( uint8_t sn )
{
	if ( sn >= PORT_DESC__LIMIT_) sn = PORT_DESC__LIMIT_ - 1;
	return ( ev3_port + sn );
}

uint8_t ev3_port_desc_type_inx( uint8_t sn )
{
	return ( ev3_port_desc( sn )->type_inx );
}

uint8_t ev3_port_desc_port( uint8_t sn )
{
	return ( ev3_port_desc( sn )->port );
}

uint8_t ev3_port_desc_extport( uint8_t sn )
{
	return ( ev3_port_desc( sn )->extport );
}

bool ev3_search_port_type( uint8_t type_inx, uint8_t *sn, uint8_t from )
{
	uint8_t _sn = from;

	while ( _sn < PORT_DESC__LIMIT_) {
		if ( ev3_port[ _sn ].type_inx == type_inx ) {
			*sn = _sn;
			return ( true );
		}
		++_sn;
	}
	*sn = PORT__NONE_;
	return ( false );
}

bool ev3_search_port_plugged_in( uint8_t port, uint8_t extport, uint8_t *sn, uint8_t from )
{
	uint8_t _sn = from;

	while ( _sn < PORT_DESC__LIMIT_) {
		if ( ev3_port[ _sn ].port == port ) {
			
			if ( extport ) {
				if ( ev3_port[ _sn ].extport == extport ) {
						*sn = _sn;
						return ( true );
					}
			}
			else {
				*sn = _sn;
				return ( true );
			}
		}
		++_sn;
	}
	*sn = PORT__NONE_;
	return ( false );
}

const char *ev3_port_mode( uint8_t mode_inx )
{
	switch ( mode_inx ) {
	case HT_NXT_SMUX_PORT_ANALOG:
		return "analog";
	case HT_NXT_SMUX_PORT_I2C:
		return "i2c";

	case LEGOEV3_INPUT_PORT_AUTO:
		return "auto";
	case LEGOEV3_INPUT_PORT_NXT_ANALOG:
		return "nxt-analog";
	case LEGOEV3_INPUT_PORT_NXT_COLOR:
		return "nxt-color";
	case LEGOEV3_INPUT_PORT_NXT_I2C:
		return "nxt-i2c";
	case LEGOEV3_INPUT_PORT_EV3_ANALOG:
		return "ev3-analog";
	case LEGOEV3_INPUT_PORT_EV3_UART:
		return "ev3-uart";
	case LEGOEV3_INPUT_PORT_OTHER_UART:
		return "other-uart";
	case LEGOEV3_INPUT_PORT_RAW:
		return "raw";

	case LEGOEV3_OUTPUT_PORT_AUTO:
		return "auto";
	case LEGOEV3_OUTPUT_PORT_EV3_TACHO_MOTOR:
		return "ev3-tacho-motor";
	case LEGOEV3_OUTPUT_PORT_RCX_MOTOR:
		return "rcx-motor";
	case LEGOEV3_OUTPUT_PORT_RCX_LED:
		return "rcx-led";
	case LEGOEV3_OUTPUT_PORT_RAW:
		return "raw";

	case MS_EV3_SMUX_PORT_UART:
		return "uart";
	case MS_EV3_SMUX_PORT_ANALOG:
		return "analog";

	case WEDO_PORT_AUTO:
		return "auto";

	}
	return ( STR_unknown_ );
}

uint8_t get_port_mode_inx( uint8_t sn, uint8_t type_inx )
{
	char buf[ 64 ];

	if ( !get_port_mode( sn, buf, sizeof( buf ))) return ( PORT_MODE__NONE_ );

	switch ( type_inx ) {
			
	case HT_NXT_SMUX_PORT:
		if ( strcmp( buf, "analog" ) == 0 ) return HT_NXT_SMUX_PORT_ANALOG;
		if ( strcmp( buf, "i2c" ) == 0 ) return HT_NXT_SMUX_PORT_I2C;

		break;

	case LEGOEV3_INPUT_PORT:
		if ( strcmp( buf, "auto" ) == 0 ) return LEGOEV3_INPUT_PORT_AUTO;
		if ( strcmp( buf, "nxt-analog" ) == 0 ) return LEGOEV3_INPUT_PORT_NXT_ANALOG;
		if ( strcmp( buf, "nxt-color" ) == 0 ) return LEGOEV3_INPUT_PORT_NXT_COLOR;
		if ( strcmp( buf, "nxt-i2c" ) == 0 ) return LEGOEV3_INPUT_PORT_NXT_I2C;
		if ( strcmp( buf, "ev3-analog" ) == 0 ) return LEGOEV3_INPUT_PORT_EV3_ANALOG;
		if ( strcmp( buf, "ev3-uart" ) == 0 ) return LEGOEV3_INPUT_PORT_EV3_UART;
		if ( strcmp( buf, "other-uart" ) == 0 ) return LEGOEV3_INPUT_PORT_OTHER_UART;
		if ( strcmp( buf, "raw" ) == 0 ) return LEGOEV3_INPUT_PORT_RAW;

		break;

	case LEGOEV3_OUTPUT_PORT:
		if ( strcmp( buf, "auto" ) == 0 ) return LEGOEV3_OUTPUT_PORT_AUTO;
		if ( strcmp( buf, "ev3-tacho-motor" ) == 0 ) return LEGOEV3_OUTPUT_PORT_EV3_TACHO_MOTOR;
		if ( strcmp( buf, "rcx-motor" ) == 0 ) return LEGOEV3_OUTPUT_PORT_RCX_MOTOR;
		if ( strcmp( buf, "rcx-led" ) == 0 ) return LEGOEV3_OUTPUT_PORT_RCX_LED;
		if ( strcmp( buf, "raw" ) == 0 ) return LEGOEV3_OUTPUT_PORT_RAW;

		break;

	case MS_EV3_SMUX_PORT:
		if ( strcmp( buf, "uart" ) == 0 ) return MS_EV3_SMUX_PORT_UART;
		if ( strcmp( buf, "analog" ) == 0 ) return MS_EV3_SMUX_PORT_ANALOG;

		break;

	case WEDO_PORT:
		if ( strcmp( buf, "auto" ) == 0 ) return WEDO_PORT_AUTO;

		break;

	}
	return ( PORT_MODE__UNKNOWN_ );
}

size_t set_port_mode_inx( uint8_t sn, uint8_t mode_inx )
{
	return set_port_mode( sn, ( char* ) ev3_port_mode( mode_inx ));
}

int ev3_port_init( void )
{
	char list[ 256 ];
	char *p;
	uint32_t sn;
	uint8_t type_inx;
	int cnt = 0;

	memset( ev3_port, 0, sizeof( ev3_port ));

	if ( !ev3_listdir( "/sys/class/lego-port", list, sizeof( list ))) return ( -1 );

	p = strtok( list, " " );
	while ( p ) {
		if (( ev3_string_suffix( "port", &p, &sn ) == 1 ) && ( sn < PORT_DESC__LIMIT_)) {
			
			type_inx = get_port_type_inx( sn );
			ev3_port[ sn ].type_inx = type_inx;
			ev3_port[ sn ].port = get_port_port_inx( sn, type_inx, &ev3_port[ sn ].extport );
			++cnt;
		}
		p = strtok( NULL, " " );
	}
	return ( cnt );
}

static const char * const ht_nxt_smux_port_name[] = {
	"in1:mux1",
	"in1:mux2",
	"in1:mux3",
	"in1:mux4",

	"in2:mux1",
	"in2:mux2",
	"in2:mux3",
	"in2:mux4",

	"in3:mux1",
	"in3:mux2",
	"in3:mux3",
	"in3:mux4",

	"in4:mux1",
	"in4:mux2",
	"in4:mux3",
	"in4:mux4",

};

#define HT_NXT_SMUX_PORT_PARENT_LEN  3
#define HT_NXT_SMUX_PORT_LEN  4
#define HT_NXT_SMUX_PORT_FULL_LEN  ( HT_NXT_SMUX_PORT_PARENT_LEN + HT_NXT_SMUX_PORT_LEN + 1 )

static uint8_t ht_nxt_smux_port_inx( uint8_t type_inx, const char *name )
{
	if (( type_inx != HT_NXT_SMUX_PORT ) && ( type_inx != PORT_TYPE__NONE_ )) return ( EV3_PORT__NONE_ );

	if ( strncmp( name, "mux", HT_NXT_SMUX_PORT_LEN - 1 ) == 0 ) {
		name += HT_NXT_SMUX_PORT_LEN - 1;
		switch ( *name ) {
		case '1':
			return HT_NXT_SMUX_PORT_1;
		case '2':
			return HT_NXT_SMUX_PORT_2;
		case '3':
			return HT_NXT_SMUX_PORT_3;
		case '4':
			return HT_NXT_SMUX_PORT_4;

		}
	}
	return ( EV3_PORT__NONE_ );
}

static const char * const legoev3_input_port_name[] = {
	"in1",
	"in2",
	"in3",
	"in4",

};

#define LEGOEV3_INPUT_PORT_PARENT_LEN  3
#define LEGOEV3_INPUT_PORT_LEN  3
#define LEGOEV3_INPUT_PORT_FULL_LEN  LEGOEV3_INPUT_PORT_LEN

static uint8_t legoev3_input_port_inx( uint8_t type_inx, const char *name )
{
	if (( type_inx != LEGOEV3_INPUT_PORT ) && ( type_inx != PORT_TYPE__NONE_ )) return ( EV3_PORT__NONE_ );

	if ( strncmp( name, "in", LEGOEV3_INPUT_PORT_LEN - 1 ) == 0 ) {
		name += LEGOEV3_INPUT_PORT_LEN - 1;
		switch ( *name ) {
		case '1':
			return LEGOEV3_INPUT_PORT_1;
		case '2':
			return LEGOEV3_INPUT_PORT_2;
		case '3':
			return LEGOEV3_INPUT_PORT_3;
		case '4':
			return LEGOEV3_INPUT_PORT_4;

		}
	}
	return ( EV3_PORT__NONE_ );
}

static const char * const legoev3_output_port_name[] = {
	"outA",
	"outB",
	"outC",
	"outD",

};

#define LEGOEV3_OUTPUT_PORT_PARENT_LEN  4
#define LEGOEV3_OUTPUT_PORT_LEN  4
#define LEGOEV3_OUTPUT_PORT_FULL_LEN  LEGOEV3_OUTPUT_PORT_LEN

static uint8_t legoev3_output_port_inx( uint8_t type_inx, const char *name )
{
	if (( type_inx != LEGOEV3_OUTPUT_PORT ) && ( type_inx != PORT_TYPE__NONE_ )) return ( EV3_PORT__NONE_ );

	if ( strncmp( name, "out", LEGOEV3_OUTPUT_PORT_LEN - 1 ) == 0 ) {
		name += LEGOEV3_OUTPUT_PORT_LEN - 1;
		switch ( *name ) {
		case 'A':
			return LEGOEV3_OUTPUT_PORT_A;
		case 'B':
			return LEGOEV3_OUTPUT_PORT_B;
		case 'C':
			return LEGOEV3_OUTPUT_PORT_C;
		case 'D':
			return LEGOEV3_OUTPUT_PORT_D;

		}
	}
	return ( EV3_PORT__NONE_ );
}

static const char * const ms_ev3_smux_port_name[] = {
	"in1:mux1",
	"in1:mux2",
	"in1:mux3",
	"in1:mux4",

	"in2:mux1",
	"in2:mux2",
	"in2:mux3",
	"in2:mux4",

	"in3:mux1",
	"in3:mux2",
	"in3:mux3",
	"in3:mux4",

	"in4:mux1",
	"in4:mux2",
	"in4:mux3",
	"in4:mux4",

};

#define MS_EV3_SMUX_PORT_PARENT_LEN  3
#define MS_EV3_SMUX_PORT_LEN  4
#define MS_EV3_SMUX_PORT_FULL_LEN  ( MS_EV3_SMUX_PORT_PARENT_LEN + MS_EV3_SMUX_PORT_LEN + 1 )

static uint8_t ms_ev3_smux_port_inx( uint8_t type_inx, const char *name )
{
	if (( type_inx != MS_EV3_SMUX_PORT ) && ( type_inx != PORT_TYPE__NONE_ )) return ( EV3_PORT__NONE_ );

	if ( strncmp( name, "mux", MS_EV3_SMUX_PORT_LEN - 1 ) == 0 ) {
		name += MS_EV3_SMUX_PORT_LEN - 1;
		switch ( *name ) {
		case '1':
			return MS_EV3_SMUX_PORT_1;
		case '2':
			return MS_EV3_SMUX_PORT_2;
		case '3':
			return MS_EV3_SMUX_PORT_3;
		case '4':
			return MS_EV3_SMUX_PORT_4;

		}
	}
	return ( EV3_PORT__NONE_ );
}

static const char * const wedo_port_name[] = {
	"in1:wedo1",
	"in1:wedo2",
	"in1:wedo3",
	"in1:wedo4",

	"in2:wedo1",
	"in2:wedo2",
	"in2:wedo3",
	"in2:wedo4",

	"in3:wedo1",
	"in3:wedo2",
	"in3:wedo3",
	"in3:wedo4",

	"in4:wedo1",
	"in4:wedo2",
	"in4:wedo3",
	"in4:wedo4",

};

#define WEDO_PORT_PARENT_LEN  3
#define WEDO_PORT_LEN  5
#define WEDO_PORT_FULL_LEN  ( WEDO_PORT_PARENT_LEN + WEDO_PORT_LEN + 1 )

static uint8_t wedo_port_inx( uint8_t type_inx, const char *name )
{
	if (( type_inx != WEDO_PORT ) && ( type_inx != PORT_TYPE__NONE_ )) return ( EV3_PORT__NONE_ );

	if ( strncmp( name, "wedo", WEDO_PORT_LEN - 1 ) == 0 ) {
		name += WEDO_PORT_LEN - 1;
		switch ( *name ) {
		case '1':
			return WEDO_PORT_1;
		case '2':
			return WEDO_PORT_2;
		case '3':
			return WEDO_PORT_3;
		case '4':
			return WEDO_PORT_4;

		}
	}
	return ( EV3_PORT__NONE_ );
}

static uint8_t __search_extport( uint8_t port )
{
	uint8_t sn;

	for ( sn = 0; sn < PORT_DESC__LIMIT_; sn++ ) {
		if (( ev3_port[ sn ].port == port ) && ( ev3_port[ sn ].extport != EXT_PORT__NONE_ )) {
			return ( ev3_port[ sn ].type_inx );
		}
		++sn;
	}
	return ( PORT_TYPE__NONE_ );
}

uint8_t ev3_port_inx( uint8_t type_inx, const char *name, uint8_t *extport )
{
	uint8_t port = EV3_PORT__NONE_;

	*extport = EXT_PORT__NONE_;
	
	if ( strlen( name ) == HT_NXT_SMUX_PORT_FULL_LEN ) {
		if ( port == EV3_PORT__NONE_ ) port = legoev3_input_port_inx( type_inx, name );
		if ( port != EV3_PORT__NONE_ ) {
			if ( type_inx == PORT_TYPE__NONE_ ) type_inx = __search_extport( port );
			*extport = ht_nxt_smux_port_inx( type_inx, name + HT_NXT_SMUX_PORT_PARENT_LEN + 1 );
			if ( *extport != EXT_PORT__NONE_ ) {
				return ( port );
			}
		}
	}

	if ( strlen( name ) == LEGOEV3_INPUT_PORT_FULL_LEN ) {
		if ( port == EV3_PORT__NONE_ ) port = legoev3_input_port_inx( type_inx, name );
		if ( port != EV3_PORT__NONE_ ) {
			return ( port );
		}
	}

	if ( strlen( name ) == LEGOEV3_OUTPUT_PORT_FULL_LEN ) {
		if ( port == EV3_PORT__NONE_ ) port = legoev3_output_port_inx( type_inx, name );
		if ( port != EV3_PORT__NONE_ ) {
			return ( port );
		}
	}

	if ( strlen( name ) == MS_EV3_SMUX_PORT_FULL_LEN ) {
		if ( port == EV3_PORT__NONE_ ) port = legoev3_input_port_inx( type_inx, name );
		if ( port != EV3_PORT__NONE_ ) {
			if ( type_inx == PORT_TYPE__NONE_ ) type_inx = __search_extport( port );
			*extport = ms_ev3_smux_port_inx( type_inx, name + MS_EV3_SMUX_PORT_PARENT_LEN + 1 );
			if ( *extport != EXT_PORT__NONE_ ) {
				return ( port );
			}
		}
	}

	if ( strlen( name ) == WEDO_PORT_FULL_LEN ) {
		if ( port == EV3_PORT__NONE_ ) port = legoev3_input_port_inx( type_inx, name );
		if ( port != EV3_PORT__NONE_ ) {
			if ( type_inx == PORT_TYPE__NONE_ ) type_inx = __search_extport( port );
			*extport = wedo_port_inx( type_inx, name + WEDO_PORT_PARENT_LEN + 1 );
			if ( *extport != EXT_PORT__NONE_ ) {
				return ( port );
			}
		}
	}

 	return ( port );
}

const char *ev3_port_name( uint8_t port, uint8_t extport )
{
						  
	if (( extport >= HT_NXT_SMUX_PORT__BASE_ ) && ( extport < HT_NXT_SMUX_PORT__BASE_ + HT_NXT_SMUX_PORT__COUNT_ )
	 && ( port >= LEGOEV3_INPUT_PORT__BASE_ ) && ( port < LEGOEV3_INPUT_PORT__BASE_ + LEGOEV3_INPUT_PORT__COUNT_ )) {
		return ht_nxt_smux_port_name[ ( port - LEGOEV3_INPUT_PORT__BASE_ ) * HT_NXT_SMUX_PORT__COUNT_ + ( extport - HT_NXT_SMUX_PORT__BASE_ ) ];
	}
	if (( extport == EXT_PORT__NONE_ )
	 && ( port >= LEGOEV3_INPUT_PORT__BASE_ ) && ( port < LEGOEV3_INPUT_PORT__BASE_ + LEGOEV3_INPUT_PORT__COUNT_ )) {
		return legoev3_input_port_name[ port - LEGOEV3_INPUT_PORT__BASE_ ];
	}
	if (( extport == EXT_PORT__NONE_ )
	 && ( port >= LEGOEV3_OUTPUT_PORT__BASE_ ) && ( port < LEGOEV3_OUTPUT_PORT__BASE_ + LEGOEV3_OUTPUT_PORT__COUNT_ )) {
		return legoev3_output_port_name[ port - LEGOEV3_OUTPUT_PORT__BASE_ ];
	}
					  
	if (( extport >= MS_EV3_SMUX_PORT__BASE_ ) && ( extport < MS_EV3_SMUX_PORT__BASE_ + MS_EV3_SMUX_PORT__COUNT_ )
	 && ( port >= LEGOEV3_INPUT_PORT__BASE_ ) && ( port < LEGOEV3_INPUT_PORT__BASE_ + LEGOEV3_INPUT_PORT__COUNT_ )) {
		return ms_ev3_smux_port_name[ ( port - LEGOEV3_INPUT_PORT__BASE_ ) * MS_EV3_SMUX_PORT__COUNT_ + ( extport - MS_EV3_SMUX_PORT__BASE_ ) ];
	}
					  
	if (( extport >= WEDO_PORT__BASE_ ) && ( extport < WEDO_PORT__BASE_ + WEDO_PORT__COUNT_ )
	 && ( port >= LEGOEV3_INPUT_PORT__BASE_ ) && ( port < LEGOEV3_INPUT_PORT__BASE_ + LEGOEV3_INPUT_PORT__COUNT_ )) {
		return wedo_port_name[ ( port - LEGOEV3_INPUT_PORT__BASE_ ) * WEDO_PORT__COUNT_ + ( extport - WEDO_PORT__BASE_ ) ];
	}

	return ( STR_unknown_ );
}

uint8_t ev3_search_port( uint8_t port, uint8_t extport )
{
	uint8_t sn;

	ev3_search_port_plugged_in( port, extport, &sn, 0 );
	return ( sn );
}

