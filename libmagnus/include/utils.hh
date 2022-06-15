#define SET_LOGGER(CLASS_NAME, LOGGER)                 \
    if (spdlog::get(CLASS_NAME) == nullptr) {          \
        _logger = spdlog::stderr_color_mt(CLASS_NAME); \
    } else {                                           \
        _logger = spdlog::get(CLASS_NAME);             \
    }