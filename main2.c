#include <libakrypt.h>
#include "ed.h"
inline void ak_edpoint_double( ak_wpoint edp, ak_wcurve ec )
{
 ak_mpznmax c, d, e, f, h, j, e_d, xy, tmp;

 if( ak_mpzn_cmp_ui( edp->z, ec->size, 0 ) == ak_true ) return;
 if( ak_mpzn_cmp_ui( edp->y, ec->size, 0 ) == ak_true ) {
   ak_wpoint_set_as_unit( edp, ec );
   return;
 }
 ak_mpzn_mul_montgomery( c, edp->x, edp->x, ec->p, ec->n, ec->size ); //C = x^2
 ak_mpzn_mul_montgomery( d, edp->y, edp->y, ec->p, ec->n, ec->size ); //D = y^2
 ak_mpzn_mul_montgomery( e, c, ec->a, ec->p, ec->n, ec->size );  //E = ex^2 = eC
 ak_mpzn_add_montgomery( f, e, d, ec->p, ec->size ); // F =ax^2 + y^2 = E + D
 ak_mpzn_mul_montgomery( h, edp->z, edp->z, ec->p, ec->n, ec->size ); // H = z^2
 ak_mpzn_lshift_montgomery( h, h, ec->p, ec->size ); // 2z^2
 if (ak_mpzn_cmp( f, h, ec->size ) == -1){
  ak_mpzn_sub(tmp, ec->p, h, ec->size);
  ak_mpzn_add(j, tmp, f, ec->size);
 }
 else {
  ak_mpzn_sub( j, f, h, ec->size ); // J = ax^2+y^2-2z^2 = F - 2H
 }
 ak_mpzn_mul_montgomery(xy, edp->x, edp->y, ec->p, ec->n, ec->size);
 ak_mpzn_lshift_montgomery( xy, xy, ec->p, ec->size ); // 2xy
 if (ak_mpzn_cmp( e, d, ec->size ) == -1){
  ak_mpzn_sub(tmp, ec->p, d, ec->size);
  ak_mpzn_add(e_d, tmp, e, ec->size);
 }
 else {
  ak_mpzn_sub( e_d, e, d, ec->size); // ax^2 - y^2 = E - D
 }
 ak_mpzn_mul_montgomery(edp->x, xy, j, ec->p, ec->n, ec->size); // (B-C-D) * J
 ak_mpzn_mul_montgomery(edp->y, f, e_d, ec->p, ec->n, ec->size); // F * (E-D)
 ak_mpzn_mul_montgomery(edp->z, f, j, ec->p, ec->n, ec->size); // F * J

}
inline void ak_convert_weier_point_to_edward(ak_wpoint ep, ak_wpoint wp, ak_wcurve ec)
{
  ak_mpznmax s, t, inv4, inv6, tmp, tmp2, tmp1, r;
  ak_mpzn_set_ui( inv4, ec->size, 4 );
  ak_mpzn_mul_montgomery( inv4, inv4, ec->r2, ec->p, ec->n, ec->size );
  ak_mpzn_set_ui( r, ec->size, 2 );
  ak_mpzn_sub( r, ec->p, r, ec->size ); // p-2
  if (ak_mpzn_cmp(ec->a, ec->b, ec->size) == -1) {  // ||| e to a and d to b |||
    ak_mpzn_sub(tmp, ec->p, ec->b, ec->size);   // ||| d to b |||
    ak_mpzn_add(tmp, tmp, ec->a, ec->size); //     ||| e to a |||
  }
  else {
    ak_mpzn_sub(tmp, ec->a, ec->b, ec->size);   // ||| e to a and d to b |||
  } // e-d
  ak_mpzn_modpow_montgomery( inv4, inv4, r, ec->p, ec->n, ec->size ); // 1/4
  ak_mpzn_mul_montgomery(s, tmp, inv4, ec->p, ec->n, ec->size); // (e-d) * 1/4
  ak_mpzn_add_montgomery(tmp, ec->a, ec->b, ec->p, ec->size); // e+d   ||| e to a and d to b |||
  ak_mpzn_set_ui( inv6, ec->size, 6 ); //6
  ak_mpzn_mul_montgomery( inv6, inv6, ec->r2, ec->p, ec->n, ec->size );
  ak_mpzn_modpow_montgomery( inv6, inv6, r, ec->p, ec->n, ec->size ); // 1/6
  ak_mpzn_mul_montgomery(t, tmp, inv6, ec->p, ec->n, ec->size); // (e+d) * 1/6
  // s=(e-d)/4, t=(e+d)/6
  ///
  struct wpoint ep1;
  ak_wpoint_set_wpoint(&ep1, wp, ec);
  ak_wpoint_reduce(&ep1, ec); //????????
  ak_mpzn_mul_montgomery( ep1.x, ep1.x, ec->r2, ec->p, ec->n, ec->size ); // x in mont
  if (ak_mpzn_cmp(ep1.x, t, ec->size) == -1 ){
    ak_mpzn_sub(tmp, ec->p, t, ec->size);
    ak_mpzn_add(tmp, tmp, ep1.x, ec->size);
  }
  else {
    ak_mpzn_sub(tmp, ep1.x, t, ec->size);
  } // x-t

  ak_mpzn_mul_montgomery( ep1.y, ep1.y, ec->r2, ec->p, ec->n, ec->size ); // y in mont
  ak_mpzn_modpow_montgomery( tmp1, ep1.y, r, ec->p, ec->n, ec->size ); // 1/y
  ak_mpzn_mul_montgomery(ep->x, tmp, tmp1, ec->p, ec->n, ec->size); //(x-t)/y == new_x

  if (ak_mpzn_cmp(tmp, s, ec->size) == -1 ){
    ak_mpzn_sub(tmp1, ec->p, s, ec->size);
    ak_mpzn_add(tmp1, tmp1, tmp, ec->size);
  }
  else {
    ak_mpzn_sub(tmp1, tmp, s, ec->size);
  } // x-t-s

  ak_mpzn_add_montgomery(tmp2, tmp, s, ec->p, ec->size); // x-t+s
  ak_mpzn_modpow_montgomery( tmp2, tmp2, r, ec->p, ec->n, ec->size ); // 1/(x-t+s)

  ak_mpzn_mul_montgomery(ep->y, tmp1, tmp2, ec->p, ec->n, ec->size);
  ak_mpzn_mul_montgomery( ep->z, ep1.z, ec->r2, ec->p, ec->n, ec->size );

  ak_wpoint_reduce(ep, ec); 
}


inline void ak_convert_edward_point_to_weier(ak_wpoint wp, ak_wpoint ep, ak_wcurve ec)
{
    ak_mpznmax s, t, inv4, inv6, tmp, tmp2, tmp1, r, one;
    ak_mpzn_set_ui( inv4, ec->size, 4 );

  ak_mpzn_mul_montgomery( inv4, inv4, ec->r2, ec->p, ec->n, ec->size );
  ak_mpzn_set_ui( r, ec->size, 2 );
  ak_mpzn_sub( r, ec->p, r, ec->size ); // p-2
  if (ak_mpzn_cmp(ec->a, ec->b, ec->size) == -1) {
    ak_mpzn_sub(tmp, ec->p, ec->b, ec->size);
    ak_mpzn_add(tmp, tmp, ec->a, ec->size);
  }
  else {
    ak_mpzn_sub(tmp, ec->a, ec->b, ec->size);
  } // e-d
  ak_mpzn_modpow_montgomery( inv4, inv4, r, ec->p, ec->n, ec->size ); // 1/4
  ak_mpzn_mul_montgomery(s, tmp, inv4, ec->p, ec->n, ec->size); // (e-d) * 1/4
  ak_mpzn_add_montgomery(tmp, ec->a, ec->b, ec->p, ec->size); // e+d
  ak_mpzn_set_ui( inv6, ec->size, 6 ); //6

  ak_mpzn_mul_montgomery( inv6, inv6, ec->r2, ec->p, ec->n, ec->size );
  ak_mpzn_modpow_montgomery( inv6, inv6, r, ec->p, ec->n, ec->size ); // 1/6
  ak_mpzn_mul_montgomery(t, tmp, inv6, ec->p, ec->n, ec->size); // (e+d) * 1/6
  // s=(e-d)/4, t=(e+d)/6

  struct wpoint ep1;
  ak_wpoint_set_wpoint(&ep1, ep, ec);

  ak_mpzn_set_ui( one, ec->size, 1 );
  ak_mpzn_mul_montgomery( ep1.x, ep1.x, ec->r2, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( ep1.y, ep1.y, ec->r2, ec->p, ec->n, ec->size );

  ak_mpzn_mul_montgomery(one, one, ec->r2, ec->p, ec->n, ec->size ); // one in mont
  ak_mpzn_sub(tmp1, ec->p, ep1.y, ec->size); // p-v
  ak_mpzn_add(tmp1, tmp1, one, ec->size); // 1+p-v = 1-v
  ak_mpzn_modpow_montgomery( tmp2, tmp1, r, ec->p, ec->n, ec->size);// 1/(1-v)

  ak_mpzn_add_montgomery(tmp, one, ep1.y, ec->p, ec->size); // 1+v
  ak_mpzn_mul_montgomery(tmp, s, tmp, ec->p, ec->n, ec->size); // s(1+v)
  ak_mpzn_mul_montgomery(tmp, tmp, tmp2, ec->p, ec->n, ec->size);
  ak_mpzn_add_montgomery(wp->x, tmp, t, ec->p, ec->size);
  ak_mpzn_modpow_montgomery( tmp1, ep1.x, r, ec->p, ec->n, ec->size);
  ak_mpzn_mul_montgomery(wp->y, tmp, tmp1, ec->p, ec->n, ec->size);
  ak_mpzn_mul_montgomery( wp->z, ep1.z, ec->r2, ec->p, ec->n, ec->size );

  ak_wpoint_reduce(wp, ec); 

}


