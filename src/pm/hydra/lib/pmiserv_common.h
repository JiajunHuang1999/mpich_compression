/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "hydra.h"
/* from libpmi */
#include "pmi_wire.h"
#include "pmi_msg.h"

/* Generic definitions */
#define PMI_MAXKEYLEN    (64)   /* max length of key in keyval space */
#define PMI_MAXVALLEN    (1024) /* max length of value in keyval space */
#define PMI_MAXKVSLEN    (256)  /* max length of various names */

struct HYD_pmcd_pmi_kvs_pair {
    char key[PMI_MAXKEYLEN];
    char val[PMI_MAXVALLEN];
    struct HYD_pmcd_pmi_kvs_pair *next;
};

struct HYD_pmcd_pmi_kvs {
    char kvsname[PMI_MAXKVSLEN];        /* Name of this kvs */
    struct HYD_pmcd_pmi_kvs_pair *key_pair;
    struct HYD_pmcd_pmi_kvs_pair *tail;
};

/* init header proxy send to server upon connection */
struct HYD_pmcd_init_hdr {
    char signature[4]; /* HYD\0 */ ;
    int proxy_id;
};

HYD_status HYD_pmcd_pmi_allocate_kvs(struct HYD_pmcd_pmi_kvs **kvs, int pgid);
void HYD_pmcd_free_pmi_kvs_list(struct HYD_pmcd_pmi_kvs *kvs_list);
HYD_status HYD_pmcd_pmi_add_kvs(const char *key, const char *val, struct HYD_pmcd_pmi_kvs *kvs,
                                int *ret);

/* ---- struct HYD_pmcd_hdr ---- */

/* The set of commands supported */
enum HYD_pmcd_cmd {
    CMD_INVALID = 0,            /* for sanity testing */

    /* UI to proxy commands */
    CMD_PROC_INFO,
    CMD_PMI_RESPONSE,
    CMD_SIGNAL,
    CMD_STDIN,

    /* Proxy to UI commands */
    CMD_PID_LIST,
    CMD_EXIT_STATUS,
    CMD_PMI,
    CMD_STDOUT,
    CMD_STDERR,
    CMD_PROCESS_TERMINATED
};

/* PMI_CMD */
struct HYD_hdr_pmi {
    int pid;                    /* ID of the requesting process */
    int pmi_version;            /* PMI version */
};

/* STDOUT/STDERR */
struct HYD_hdr_io {
    int pgid;
    int proxy_id;
    int rank;
};

struct HYD_pmcd_hdr {
    enum HYD_pmcd_cmd cmd;
    int buflen;
    union {
        int data;               /* for commands with a single integer data */
        struct HYD_hdr_pmi pmi;
        struct HYD_hdr_io io;
    } u;
};

void HYD_pmcd_init_header(struct HYD_pmcd_hdr *hdr);
void HYD_pmcd_pmi_dump(struct PMIU_cmd *pmi);
HYD_status HYD_pmcd_pmi_send(int fd, struct PMIU_cmd *pmi, struct HYD_pmcd_hdr *hdr, int debug);

#endif /* COMMON_H_INCLUDED */
