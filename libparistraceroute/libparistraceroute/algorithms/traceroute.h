#ifndef ALGORITHMS_TRACEROUTE_H
#define ALGORITHMS_TRACEROUTE_H

/*
 * Principle: (from man page)
 * 
 * traceroute - print the route packets trace to network host
 * 
 * traceroute  tracks the route packets taken from an IP network on
 * their way to a given host. It utilizes the IP protocol's time to
 * live (TTL) field and * attempts to elicit an ICMP TIME_EXCEEDED
 * response from each gateway along the path to the host.
 * 
 * --------------------------------------------------------------------
 * Uses
 * --------------------------------------------------------------------
 *
 * - probe/reply: IP  / ICMP TTL expired
 *
 *      This means that we send IP packet and we're expecting ICMP TTL
 *      expired responses.
 *
 * - tag: TCP / UDP ports
 *
 *      Where we store the identifier of each sent probes.
 */

/* --------------------------------------------------------------------
 * Options
 * --------------------------------------------------------------------
 */

typedef struct {
    unsigned     min_ttl;    /**< Minimum ttl at which to send probes */
    unsigned     max_ttl;    /**< Maximum ttl at which to send probes */
    unsigned     num_probes; /**< Number of probes per hop            */
    const char * dst_ip;     /**< The target IP */
} traceroute_options_t;

// Default options

//static traceroute_options_t traceroute_default = {
//    .min_ttl    = 1,
//    .max_ttl    = 30,
//    .num_probes = 3,
//};

/* --------------------------------------------------------------------
 * Algorithm
 * --------------------------------------------------------------------
 *
 *     INIT:
 *         cur_ttl = min_ttl
 *         SEND
 *
 *     SEND:
 *         send num_probes probes with TTL = cur_ttl
 * 
 *     PROBE_REPLY:
 *         if < num_probes
 *             continue waiting
 *             if all_stars or destination_reached or stopping ICMP error
 *                 EXIT
 *             cur_ttl += 1
 *             SEND
 */

/* --------------------------------------------------------------------
 * Probe output
 * --------------------------------------------------------------------
 *
 *     A set of probes and their replies
 *
 */

typedef enum {
    TRACEROUTE_DESTINATION_REACHED,
    TRACEROUTE_ALL_STARS,
    TRACEROUTE_PROBE_REPLY,
    TRACEROUTE_ICMP_ERROR
} traceroute_event_type_t;

// This structure gathers the information passed to the
// user-defined handler

typedef struct {
    // Mandatory field
    const traceroute_options_t * options;  /**< Options passed to this instance */
    // Event-specific fields
    const char * discovered_ip;    /**< Discovered IP */
    unsigned     current_ttl;      /**< Current TTL */
    unsigned     num_sent_probes;  /**< This is i-th probe sent for this TTL */
} traceroute_probe_reply_t;

typedef union {
    traceroute_probe_reply_t probe_reply;
} traceroute_event_value_t;

typedef struct {
    traceroute_event_type_t  type;
    traceroute_event_value_t value;
} traceroute_caller_data_t;



/* --------------------------------------------------------------------
 * Interpretation output
 * --------------------------------------------------------------------
 *
 *     The set of interfaces met at each ttl, in order, annotated with
 *     DNS and RTT or ICMP errors * or stars
 */

#endif