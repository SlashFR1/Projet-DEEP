#ifndef AS608_H
#define AS608_H

#include <stdint.h>
#include <stddef.h>

/* Enumérations */

// Booléens spécifiques au capteur
typedef enum {
    AS608_BOOL_FALSE = 0,
    AS608_BOOL_TRUE = 1
} as608_bool_t;

// Niveau de sécurité
typedef enum {
    AS608_LEVEL_1 = 1,
    AS608_LEVEL_2,
    AS608_LEVEL_3,
    AS608_LEVEL_4,
    AS608_LEVEL_5
} as608_level_t;

// Taille des paquets
typedef enum {
    AS608_PACKET_SIZE_32 = 32,
    AS608_PACKET_SIZE_64 = 64,
    AS608_PACKET_SIZE_128 = 128,
    AS608_PACKET_SIZE_256 = 256
} as608_packet_size_t;

// Buffers
typedef enum {
    AS608_BUFFER_1 = 1,
    AS608_BUFFER_2 = 2
} as608_buffer_number_t;

// Type de capteur
typedef enum {
    AS608_SENSOR_FPC1011C = 0,
    AS608_SENSOR_S500,
    AS608_SENSOR_OTHER
} as608_sensor_type_t;

// Mode de gravure
typedef enum {
    AS608_BURN_MODE_DISABLE = 0,
    AS608_BURN_MODE_ENABLE
} as608_burn_code_mode_t;

// Type d'image
typedef enum {
    AS608_IMAGE_BINARY = 0,
    AS608_IMAGE_NO_CHARACTERISTICS,
    AS608_IMAGE_WITH_CHARACTERISTICS
} as608_image_t;

// GPIO
typedef enum {
    AS608_GPIO_0 = 0,
    AS608_GPIO_1
} as608_gpio_number_t;

typedef enum {
    AS608_GPIO_LOW = 0,
    AS608_GPIO_HIGH
} as608_gpio_level_t;

// Statuts
typedef enum {
    AS608_STATUS_OK = 0x00,
    AS608_STATUS_ERROR = 0x01,
    AS608_STATUS_NO_FINGER = 0x02,
    AS608_STATUS_IMAGE_FAIL = 0x03,
    AS608_STATUS_IMAGE_MESSY = 0x06,
    AS608_STATUS_FEATURE_FAIL = 0x07,
    AS608_STATUS_NO_MATCH = 0x08,
	AS608_STATUS_ENROOL_ERROR,
	AS608_STATUS_LIB_FULL,
    AS608_STATUS_COMM_ERROR = 0x0D,
    AS608_STATUS_UNKNOWN = 0xFF
} as608_status_t;

/* Structures */

// Paramètres du capteur
typedef struct {
    uint32_t address;
    uint16_t capacity;
    as608_packet_size_t packet_size;
    as608_level_t security_level;
    as608_sensor_type_t sensor_type;
} as608_params_t;

// Info statique
typedef struct {
    const char *name;
    const char *manufacturer;
    float voltage;
    float temperature;
} as608_info_t;

// Handle principal
typedef struct {
    void (*debug_print)(const char *fmt, ...);
    void (*delay_ms)(uint32_t ms);
    int (*uart_init)(void);
    int (*uart_deinit)(void);
    int (*uart_read)(uint8_t *buf, uint16_t len);
    int (*uart_write)(uint8_t *buf, uint16_t len);
    uint8_t *buffer;
    as608_status_t last_status;
    as608_params_t params;
} as608_handle_t;

/* Macros de liaison */

#define DRIVER_AS608_LINK_INIT(H, F)          (H).uart_init = F
#define DRIVER_AS608_LINK_DEINIT(H, F)        (H).uart_deinit = F
#define DRIVER_AS608_LINK_READ(H, F)          (H).uart_read = F
#define DRIVER_AS608_LINK_WRITE(H, F)         (H).uart_write = F
#define DRIVER_AS608_LINK_DELAY_MS(H, F)      (H).delay_ms = F
#define DRIVER_AS608_LINK_DEBUG_PRINT(H, F)   (H).debug_print = F
#define DRIVER_AS608_LINK_BUFFER(H, BUF)      (H).buffer = BUF

/* Fonctions principales */

int as608_info(as608_handle_t *handle, as608_info_t *info);
int as608_init(as608_handle_t *handle);
int as608_deinit(as608_handle_t *handle);
as608_status_t as608_get_last_status(as608_handle_t *handle);
void as608_print_status(as608_handle_t *handle, as608_status_t status);
int as608_get_image(as608_handle_t *handle, as608_image_t *image_type);
int as608_generate_feature(as608_handle_t *handle, as608_buffer_number_t buffer);
int as608_match_feature(as608_handle_t *handle, uint16_t *score);


#endif // AS608_H
