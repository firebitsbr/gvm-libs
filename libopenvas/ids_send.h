#ifndef __NESSUSLIB_IDS_SEND_H__
#define __NESSUSLIB_IDS_SEND_H__

/*
 * Transport layer options
 * XXX: These defines were moved here from libopenvas.h sind they
 * are used most often in ids_send module. More reengineering
 * probably needed.
 */
#define NESSUS_CNX_IDS_EVASION_SPLIT	1L  /* Try to evade NIDS by spliting sends */
#define NESSUS_CNX_IDS_EVASION_INJECT	2L /* Split + insert garbage */
#define NESSUS_CNX_IDS_EVASION_SHORT_TTL 4L /* Split + too short ttl for garbage */
#define NESSUS_CNX_IDS_EVASION_FAKE_RST  8L /* Send a fake RST from our end after each established connection */

#define NESSUS_CNX_IDS_EVASION_SEND_MASK (NESSUS_CNX_IDS_EVASION_SPLIT|NESSUS_CNX_IDS_EVASION_INJECT|NESSUS_CNX_IDS_EVASION_SHORT_TTL)

int ids_send(int, void*, int, int);
int ids_open_sock_tcp(struct arglist*, int, int, int);

#endif
