#include <image/image.h>
#include <top/top.h>

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("image_factory", ImageHandler)
FCGIDAEMON_ADD_DEFAULT_FACTORY("top_factory", TopHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
