idf_component_register(
    SRCS "button.c" "Button.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp_driver_gpio
)