idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES e18_sensor counter Leds button wifi mqtt_app
)