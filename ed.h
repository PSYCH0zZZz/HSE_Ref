#include <libakrypt.h>
dll_export void ak_edpoint_double( ak_wpoint edp, ak_wcurve ec );
dll_export void ak_convert_weier_point_to_edward(ak_wpoint ep, ak_wpoint wp, ak_wcurve ec);
dll_export void ak_convert_edward_point_to_weier(ak_wpoint wp, ak_wpoint ep, ak_wcurve ec);

