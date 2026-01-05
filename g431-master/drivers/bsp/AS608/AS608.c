#include "AS608.h"
#include <string.h> // Pour memcpy si besoin

/**
 * @brief Fonction interne pour envoyer une commande au capteur.
 * @note Cette fonction est à implémenter selon le protocole du capteur.
 */
static int as608_send_command(as608_handle_t *handle, uint8_t *cmd, uint16_t len) {
    return handle->uart_write(cmd, len);
}

/**
 * @brief Fonction interne pour lire la réponse du capteur.
 */
static int as608_read_response(as608_handle_t *handle, uint8_t *buf, uint16_t len) {
    return handle->uart_read(buf, len);
}

as608_status_t as608_get_last_status(as608_handle_t *handle) {
    return handle->last_status;
}

void as608_print_status(as608_handle_t *handle, as608_status_t status) {
    if (handle->debug_print == NULL) return;
    switch (status) {
        case AS608_STATUS_OK: handle->debug_print("Statut : OK\n"); break;
        case AS608_STATUS_NO_FINGER: handle->debug_print("Pas de doigt détecté\n"); break;
        case AS608_STATUS_IMAGE_FAIL: handle->debug_print("Échec capture image\n"); break;
        case AS608_STATUS_NO_MATCH: handle->debug_print("Aucune correspondance\n"); break;
        case AS608_STATUS_ENROOL_ERROR: handle->debug_print("Erreur d'enrôlement\n"); break;
        case AS608_STATUS_LIB_FULL: handle->debug_print("Bibliothèque pleine\n"); break;
        case AS608_STATUS_COMM_ERROR: handle->debug_print("Erreur de communication\n"); break;
        default: handle->debug_print("Statut inconnu : 0x%02X\n", status); break;
    }
}

int as608_get_image(as608_handle_t *handle, as608_image_t *image_type) {
    uint8_t cmd[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
    if (as608_send_command(handle, cmd, sizeof(cmd)) != 0) {
        handle->last_status = AS608_STATUS_COMM_ERROR;
        return -1;
    }
    uint8_t resp[12];
    if (as608_read_response(handle, resp, sizeof(resp)) != 0) {
        handle->last_status = AS608_STATUS_COMM_ERROR;
        return -1;
    }
    handle->last_status = (as608_status_t)resp[9];
    if (image_type) *image_type = (as608_image_t)resp[9];
    return 0;
}

int as608_generate_feature(as608_handle_t *handle, as608_buffer_number_t buffer) {
    uint8_t cmd[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, buffer, 0x00, 0x00};
    cmd[12] = 0x06 + buffer;
    if (as608_send_command(handle, cmd, sizeof(cmd)) != 0) {
        handle->last_status = AS608_STATUS_COMM_ERROR;
        return -1;
    }
    uint8_t resp[12];
    if (as608_read_response(handle, resp, sizeof(resp)) != 0) {
        handle->last_status = AS608_STATUS_COMM_ERROR;
        return -1;
    }
    handle->last_status = (as608_status_t)resp[9];
    return 0;
}

int as608_match_feature(as608_handle_t *handle, uint16_t *score) {
    uint8_t cmd[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x03, 0x00, 0x07};
    if (as608_send_command(handle, cmd, sizeof(cmd)) != 0) {
        handle->last_status = AS608_STATUS_COMM_ERROR;
        return -1;
    }
    uint8_t resp[14];
    if (as608_read_response(handle, resp, sizeof(resp)) != 0) {
        handle->last_status = AS608_STATUS_COMM_ERROR;
        return -1;
    }
    handle->last_status = (as608_status_t)resp[9];
    if (score) *score = (resp[10] << 8) | resp[11];
    return 0;
}

// Les fonctions suivantes (enroll, delete_feature, identify) sont à compléter selon protocole AS608
