#include <libakrypt.h>
//#include <ak_mpzn.h>
#include <math.h>
int main( void ) {

  if ( ak_libakrypt_create( NULL ) != ak_true) {
    /* Инициализация выполнена не успешно, следовательно, выходим из программы*/
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  }

  /*ситаем эдвардса */
unsigned long long a, b, x, y, d, x_3, y_3, ab, bb, x_fe, y_fe, x_se, y_se;
  x = 1; 
  y = sqrt(2);
  a = 3;
  d = 2;
  
  x_3 = (d * x * y)/(a * x * x + y * y); 
  y_3 = (y * y - a * x * x)/(d - a * x * x - y * y);
  x_fe = (1+y_3)/(1-y_3);
  y_fe=x_3/(1-y_3);

  x_se = (1+y)/(1-y);
  y_se=x/(1-y);
  
  ab=2 * (1 - a) / d * (1 - a);
  bb=4 * (1 - a) / d * (1 - a);

   ak_mpzn512 gga, ggb; // создаём переменную типа ak_mpzn512

   memset(&gga, 0, sizeof(gga)); // инициализируем её нулями
   memset(&ggb, 0, sizeof(ggb));

   for(int i = 0; i < sizeof(ab); i++) {
        ((unsigned char*)&gga)[i] = (ab >> (8 * i)) & 0xFF; // записываем каждый байт числа ab в нашу переменную
   }

   for(int i = 0; i < sizeof(bb); i++) {
        ((unsigned char*)&ggb)[i] = (bb >> (8 * i)) & 0xFF; // записываем каждый байт числа bb в нашу переменную
   }
 
  /* инициализируем библиотеку */


  /* объявляем кривую mycurve */
  struct wcurve mycurve;
  /* копируем параметры кривой id_rfc4357_gost_3410_2001_paramSetA в mycurve */ 
  memcpy(&mycurve, &id_rfc4357_gost_3410_2001_paramSetA,
		  sizeof(id_rfc4357_gost_3410_2001_paramSetA));
  /*struct wcurve mycurve = {

   .size=goscurve.size,
   .cofactor=goscurve.cofactor,
   .a = gga,
   .b = ggb,
   .p = goscurve.p,
   .r2 = goscurve.r2,
   .q = goscurve.q,
   .r2q = goscurve.r2q,
   struct wpoint point;
   .n=goscurve.n;
   .nq=goscurve.nq;
   const char *pchar;
  };*/

  ak_mpzn512 ssx, ssy; // создаём переменную типа ak_mpzn512
  printf("test");
  memset(&ssx, 0, sizeof(ssx)); // инициализируем её нулями
  memset(&ssy, 0, sizeof(ssy)); // инициализируем её нулями

   for(int i = 0; i < sizeof(x_se); i++) {
        ((unsigned char*)&ssx)[i] = (x_se >> (8 * i)) & 0xFF; // записываем каждый байт числа 4 в нашу переменную
   }

   for(int i = 0; i < sizeof(y_se); i++) {
        ((unsigned char*)&ssy)[i] = (y_se >> (8 * i)) & 0xFF; // записываем каждый байт числа 4 в нашу переменную
   }


  strcpy(mycurve.a, gga);
  strcpy(mycurve.b, ggb);
  


  /* объявляем точки R и Q равные образующей точке эллиптической кривой*/
  struct wpoint P = {
  .x = ssx,
  .y= ssy,
  .z={0}
};

  ak_wpoint_double(&P, &mycurve);
  printf("Точка P после удвоения в Вейштрассе (т.е 2P)\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(P.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(P.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(P.z, mycurve.size));

 printf("Р после удвоения в Эдвардсе\n");
  printf("X = %s\n", x);
  printf("Y = %s\n", y);
  free(mycurve.a);
  free(mycurve.b);
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
