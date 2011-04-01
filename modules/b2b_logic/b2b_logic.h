/*
 * $Id: b2b_logic.h $
 *
 * back-to-back logic module
 *
 * Copyright (C) 2009 Free Software Fundation
 *
 * This file is part of opensips, a free SIP server.
 *
 * opensips is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * opensips is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 * --------
 *  2009-08-03  initial version (Anca Vamanu)
 */

#ifndef _B2B_LOGIC_H_
#define _B2B_LOGIC_H_

#include <libxml/parser.h>
#include "../../str.h"
#include "../b2b_entities/b2b_entities.h"

#define B2B_INIT_MAX_PARAMNO     5
#define B2B_BRIDGING_STATE      -1
#define B2B_CANCEL_STATE        -2
#define B2B_NOTDEF_STATE        -3

#define B2B_TOP_HIDING_SCENARY "top hiding"
#define B2B_TOP_HIDING_SCENARY_LEN  strlen("top hiding")


#define B2BL_ENT_NEW		0
#define B2BL_ENT_CONFIRMED	1


#define b2b_peer(type) ((type+1)%2)

#define HDR_LST_LEN       32
#define HDR_DEFAULT_LEN   9

extern b2b_api_t b2b_api;

enum b2bl_caller_type {
	CALLER_MODULE,
	CALLER_SCRIPT,
	CALLER_MI
};
extern enum b2bl_caller_type b2bl_caller;

typedef struct b2b_rule
{
	unsigned int id;
	int cond_state;
	xmlNodePtr cond_node;
	xmlNodePtr action_node;
	struct b2b_rule* next;
}b2b_rule_t;

enum {
	B2B_INVITE,
	B2B_ACK,
	B2B_BYE,
	B2B_MESSAGE,
	B2B_SUBSCRIBE,
	B2B_NOTIFY,
	B2B_REFER,
	B2B_CANCEL,
	B2B_METHODS_NO
};


typedef struct b2b_scenario
{
	str id;
	unsigned int param_no;
	char use_init_sdp;
	xmlDocPtr doc;
	xmlNodePtr init_node;
	b2b_rule_t* request_rules[B2B_METHODS_NO];
	b2b_rule_t* reply_rules;

	struct b2b_scenario* next;
}b2b_scenario_t;

extern b2b_scenario_t* script_scenaries;
extern b2b_scenario_t* extern_scenaries;

extern str custom_headers_lst[HDR_LST_LEN];
int custom_headers_lst_len;
extern int use_init_sdp;
extern str server_address;
extern unsigned int max_duration;

static inline int b2b_get_request_id(str* request)
{
	if(request->len ==INVITE_LEN&&strncasecmp(request->s,INVITE,INVITE_LEN)==0)
		return B2B_INVITE;

	if(request->len ==ACK_LEN && strncasecmp(request->s,ACK,ACK_LEN)==0)
		return B2B_ACK;

	if(request->len ==BYE_LEN && strncasecmp(request->s,BYE,BYE_LEN)==0)
		return B2B_BYE;

	if(request->len==REFER_LEN &&strncasecmp(request->s, REFER, REFER_LEN)==0)
		return B2B_REFER;

	if(request->len==CANCEL_LEN &&strncasecmp(request->s, CANCEL, CANCEL_LEN)==0)
		return B2B_CANCEL;

	if(request->len==SUBSCRIBE_LEN &&strncasecmp(request->s, SUBSCRIBE, SUBSCRIBE_LEN)==0)
		return B2B_SUBSCRIBE;

	if(request->len==NOTIFY_LEN &&strncasecmp(request->s, NOTIFY, NOTIFY_LEN)==0)
		return B2B_NOTIFY;

	if(request->len==MESSAGE_LEN &&strncasecmp(request->s, MESSAGE, MESSAGE_LEN)==0)
		return B2B_MESSAGE;

	return -1;
}

b2b_scenario_t* b2b_find_scenario(b2b_scenario_t* scenario,
		unsigned int scenario_id);
int b2b_add_dlginfo(str* key, str* entity_key,int src, b2b_dlginfo_t* info);
int b2b_server_notify(struct sip_msg* msg, str* key, int type, void* param);
int b2b_client_notify(struct sip_msg* msg, str* key, int type, void* param);
b2b_scenario_t* get_scenario_id_list(str* sid, b2b_scenario_t* list);

#endif
