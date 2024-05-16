find_path(ENet_INCLUDE_DIRS enet/enet.h)
find_library(ENet_LIBRARIES NAMES enet)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ENet DEFAULT_MSG ENet_LIBRARIES ENet_INCLUDE_DIRS)
