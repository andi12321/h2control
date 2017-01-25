/** \file libad_scan.h
 *
 * Scan related declarations.
 */

/* what to store
 */

#define AD_STORE_DISCRETE         0x0001
#define AD_STORE_AVERAGE          0x0002
#define AD_STORE_MIN              0x0004
#define AD_STORE_MAX              0x0008
#define AD_STORE_RMS              0x0010


/* trigger mode
 */

#define AD_TRG_NONE               0x00
#define AD_TRG_POSITIVE           0x01
#define AD_TRG_NEGATIVE           0x02
#define AD_TRG_INSIDE             0x03
#define AD_TRG_OUTSIDE            0x04
#define AD_TRG_DIGITAL            0x80
#define AD_TRG_NEVER              0xff

/* alarm mode
 */

#define AD_ALARM_NONE             0x00
#define AD_ALARM_POSITIVE         0x01
#define AD_ALARM_NEGATIVE         0x02
#define AD_ALARM_INSIDE           0x03
#define AD_ALARM_OUTSIDE          0x04


/** Scan channel descriptor.
 *
 * Defines scan settings of a single channel.
 */

struct ad_scan_cha_desc
{
  int32_t cha;              /**< IN    channel type and id */
  int32_t range;            /**< IN    range number (driver specific) */
  int32_t store;            /* IN    what to sample */
  int32_t ratio;            /* IN    scan ratio */
  uint32_t zero;            /* IN    physical 0.0 */
  uint8_t trg_mode;         /* IN    trigger mode */
  uint8_t alarm_mode;       /* IN    alarm mode */
  uint8_t sc_res1[2];
  uint32_t trg_par[2];      /* IN    trigger parameters */
  int32_t samples_per_run;  /* OUT   number of samples per run */
  uint32_t alarm_par[2];    /* IN    alarm parameters */
  uint32_t sc_res2[5];
};

/** Scan descriptor.
 *
 * Defines global scan settings.
 */

struct ad_scan_desc
{
  double sample_rate;       /* INOUT sampling rate (sec) */
  double clock_rate;        /* IN    ref clock rate (sec) */
  uint64_t prehist;         /* IN    number of samples prehistory */
  uint64_t posthist;        /* IN    number of samples posthistory */
  uint32_t ticks_per_run;   /* INOUT number of ticks per run */
  uint32_t bytes_per_run;   /* OUT   bytes per run */
  uint32_t samples_per_run; /* OUT   samples per run */
  uint32_t flags;           /* INOUT scan flags */
  uint32_t sd_res[12];
};


#define AD_SF_SCANNING        0x00000001       /**< Is set while device is scanning. */
#define AD_SF_TRIGGER         0x00000002       /**< Scan has triggered */
#define AD_SF_SAMPLES         0x00010000       /**< Scan buffers handled internally. */

/** Current scan state.
 */

struct ad_scan_state
{
  /** Scan flags. \see scan_flags
   */
  int32_t flags;

  /** Number of runs ready to read.
   * As long as \p runs_pending is not zero, there is at least a
   * single run pending and calling ad_get_next_run() et al. will not
   * block.
   *
   * \note  Please note that not all data aquisition devices will
   * return the exact number of runs pending and instead return 1
   * even if more than a single run is pending.
   */
  int32_t runs_pending;

  /** Posthistory so far.
   * Returns number of posthistory samples recorded so far.
   * \note Please note that not all data aquisition devices support
   * this parameter and might return zero instead.
   */
  int64_t posthist;
};

/* structure used to carry scan position
 */

typedef uint32_t ad_run_t;

struct ad_scan_pos
{
  ad_run_t run;             /* OUT   id of run */
  uint32_t offset;          /* OUT   run offset */
};


#define ad_scan_pos_lt(a,b) \
  ((a)->run < (b)->run      \
   || ((a)->run == (b)->run && (a)->offset < (b)->offset))

#define ad_scan_pos_ne(a,b) \
  ((a)->run != (b)->run     \
   || (a)->offset != (b)->offset)

void
ad_scan_pos_add (uint32_t run_size, struct ad_scan_pos *res, uint64_t offset);

void
ad_scan_pos_sub (uint32_t run_size, struct ad_scan_pos *res, uint64_t offset);



/* Information available for each sampled channel
 *
 * Pls note that the trigger position is known only after
 * the trigger event has been reached. So don't expected
 * posthist_samples to be valid *before* the scan has triggered.
 */

struct ad_sample_layout
{
  uint64_t buffer_start;             /**< Position of 1st buffered sample. */
  uint64_t start;                    /**< Position of 1st sample. */

  uint64_t prehist_samples;          /**< Number of samles in prehistory. */
  uint64_t posthist_samples;         /**< Number of samles in posthistory. */
};



/* start scan operation
 *
 * adh    handle returned by ad_open
 *
 * returns 0 on success, otherwise error code
 */

int32_t
ad_start_scan (int32_t adh, struct ad_scan_desc *sd,
               uint32_t chac, struct ad_scan_cha_desc *chav);

int32_t
ad_start_mux_scan (int32_t adh, struct ad_scan_desc *sd,
                   uint32_t chac, struct ad_scan_cha_desc *chav);

int32_t
ad_calc_run_size (int32_t adh, struct ad_scan_desc *sd,
                  uint32_t chac, struct ad_scan_cha_desc *chav);

int32_t
ad_prep_scan (int32_t adh, struct ad_scan_desc *sd,
              uint32_t chac, struct ad_scan_cha_desc *chav);

int32_t
ad_start_prepared_scan (int32_t adh);


/* start scan helpers
 */

int32_t
ad_start_scan_v (int32_t adh, double sample_rate, uint32_t posthist,
                 uint32_t chac, int32_t *chav,
                 uint32_t rangec, int32_t *rangev);

int32_t
ad_start_mem_scan (int32_t adh, struct ad_scan_desc *sd,
                   uint32_t chac, struct ad_scan_cha_desc *chav);

int32_t
ad_prep_mem_scan (int32_t adh, struct ad_scan_desc *sd,
                  uint32_t chac, struct ad_scan_cha_desc *chav);


/* Read samples from scan buffer. These functions require
 * AD_SF_SAMPLES to be set in struct ad_scan_desc, or else
 * they will fail...
 */

int32_t
ad_get_samples (int32_t adh, int32_t index, int32_t type, uint64_t offset, uint32_t *n, void *buf);

int32_t
ad_get_samples_f (int32_t adh, int32_t index, int32_t type, uint64_t offset, uint32_t *n, float *buf);

int32_t
ad_get_samples_f64 (int32_t adh, int32_t index, int32_t type, uint64_t offset, uint32_t *n, double *buf);

int32_t
ad_get_sample_layout (int32_t adh, int32_t index, struct ad_sample_layout *layout);


/* stop scan
 */

int32_t
ad_stop_scan (int32_t adh, int32_t *result);


/* get next available run (blocks until run is ready)
 *
 * adh    handle returned by ad_open
 * state  current state information
 * run    id of returned run
 * p      buffer to receive data of next run
 *
 * returns 0 on success, otherwise error code
 */

int32_t
ad_get_next_run (int32_t adh, struct ad_scan_state *state,
                 uint32_t *run, void *p);

int32_t
ad_get_next_run_f (int32_t adh, struct ad_scan_state *state, uint32_t *run, float *p);

int32_t
ad_get_next_run_f64 (int32_t adh, struct ad_scan_state *state, uint32_t *run, double *p);

/* get buffer ptr
 */

int32_t
ad_next_buffer (int32_t adh, const void *buf, void **next);

int32_t
ad_next_run (int32_t adh, struct ad_scan_state *state,
             uint32_t *run, void **buf);


/* get current scan state
 */

int32_t
ad_poll_scan_state (int32_t adh, struct ad_scan_state *state);


struct ad_timeval
{
  uint64_t tv_sec;
  uint32_t tv_usec;
};

int32_t
ad_get_scan_start (int32_t adh, struct ad_timeval *scan_start);


/* return trigger information
 */

int32_t
ad_get_trigger_pos (int32_t adh, struct ad_scan_pos *pos);

int32_t
ad_set_trigger_pos (int32_t adh, struct ad_scan_pos *pos);


/* information available for each sampled channel
 * - only after the scan is done
 */

struct ad_cha_layout
{
  struct ad_scan_pos start;

  int64_t prehist_samples;
  int64_t posthist_samples;
  double t0;
};

int32_t
ad_get_channel_layout (int32_t adh, int idx, struct ad_cha_layout *layout);

