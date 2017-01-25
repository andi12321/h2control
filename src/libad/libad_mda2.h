/** \file libad_mda2.h
 *
 * MDAII hardware specific control codes.
 *
 */

#ifndef LIBAD_MDA2__H
#define LIBAD_MDA2__H

#ifdef __cplusplus
extern "C" {
#endif

#define AD_MDA2_SET_GENERATOR       0xb3800082
#define AD_MDA2_START_GENERATOR     0xb3800083
#define AD_MDA2_STOP_GENERATOR      0xb3800084

struct ad_mda2_generator_cha
{
  uint32_t cha;               /* generator output number */
  uint32_t range;             /* range number */
  uint32_t rate;
  uint32_t start_addr;        /* start address */
  uint32_t stop_addr;         /* end address */
};

struct ad_mda2_generator
{
  uint32_t cha;               /* channel id */
  uint32_t chac;              /* number of channels */
  struct ad_mda2_generator_cha chav[16];
  uint32_t ram[2048];         /* ram */
};


#ifdef __cplusplus
}
#endif

#endif /* !LIBAD_MDA2__H */
