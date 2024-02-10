#include <libakrypt.h>
#include "ed.h"
 const struct wcurve id_tc26_gost_3410_2012_256_paramSetA_Edward = {
  ak_mpzn256_size,
  4, /* cofactor */ 
  { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
  { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
  { 0x0000000000000269LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* e */
  { 0x40c8687d966dd5b1LL, 0x1fb647d3f0757f77LL, 0xffda75588b970634LL, 0x845fa0e16716c1bbLL }, /* d */
  { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
  { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
  { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
  { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
  {
    { 0x000000000000000DLL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* px */
    { 0xF92B2592DBA300E7LL, 0xE7EF8DBE87F22E81LL, 0x8488C38FAB07649CLL, 0x60CA1E32AA475B34LL }, /* py */
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
  },
  0x46f3234475d5add9LL, /* n */
  0x035bdd1aeafdb0a9LL, /* nq */
  "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
};
 const struct wcurve id_tc26_gost_3410_2012_256_paramSetA_Weierstrass = {
  ak_mpzn256_size,
  4, /* cofactor */
  { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
  { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
  { 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* e */
  { 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* d */
  { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
  { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
  { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
  { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
  {
    { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
    { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
  },
  0x46f3234475d5add9LL, /* n */
  0x035bdd1aeafdb0a9LL, /* nq */
  "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
};
int main(int argc, char** argv)
{
    printf("Test convert E-W-E\n");

    struct wcurve ec;
    struct wcurve wc;
    memcpy(&ec, &id_tc26_gost_3410_2012_256_paramSetA_Edward,
		  sizeof(id_tc26_gost_3410_2012_256_paramSetA_Edward));
    memcpy(&wc, &id_tc26_gost_3410_2012_256_paramSetA_Weierstrass,
		  sizeof(id_tc26_gost_3410_2012_256_paramSetA_Weierstrass));
    struct wpoint wcpoint;
    struct wpoint ecpoint;

    ak_wpoint_set_wpoint(&wcpoint, &wc.point, &wc);
    ak_wpoint_set_wpoint(&ecpoint, &ec.point, &ec);

    ak_wpoint_double(&wcpoint, &wc);
    ak_edpoint_double(&ecpoint, &ec);

    ak_wpoint_reduce(&wcpoint, &wc);
    ak_wpoint_reduce(&ecpoint, &ec);

    ak_convert_edward_point_to_weier(&ecpoint, &ecpoint, &ec);
    printf("\n%s\n", "point in WForm");
    printf("    %s.x=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.x, wc.size ));
    printf("    %s.y=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.y, wc.size ));
    printf("    %s.z=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.z, wc.size ));
    printf("\n");
    ak_convert_weier_point_to_edward(&ecpoint, &ecpoint, &wc);
    printf("\n%s\n", "point in Eform");
    printf("    %s.x=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.x, ec.size ));
    printf("    %s.y=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.y, ec.size ));
    printf("    %s.z=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.z, ec.size ));
    printf("\n");
    ak_convert_edward_point_to_weier(&ecpoint, &ecpoint, &ec);
    printf("\n%s\n", "point in WForm after converts");
    printf("    %s.x=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.x, wc.size ));
    printf("    %s.y=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.y, wc.size ));
    printf("    %s.z=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.z, wc.size ));
    printf("\n");

    printf("Test convert Edouble and Wdouble\n");

    printf("\n%s\n", "Wdouble in W");
    printf("    %s.x=  0x%s\n", wcpoint, ak_mpzn_to_hexstr( wcpoint.x, wc.size ));
    printf("    %s.y=  0x%s\n", wcpoint, ak_mpzn_to_hexstr( wcpoint.y, wc.size ));
    printf("    %s.z=  0x%s\n", wcpoint, ak_mpzn_to_hexstr( wcpoint.z, wc.size ));
    printf("\n");

    printf("\n%s\n", "Edouble in W");
    printf("    %s.x=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.x, wc.size ));
    printf("    %s.y=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.y, wc.size ));
    printf("    %s.z=  0x%s\n", ecpoint, ak_mpzn_to_hexstr( ecpoint.z, wc.size ));
    printf("\n");

    return 0;
}
