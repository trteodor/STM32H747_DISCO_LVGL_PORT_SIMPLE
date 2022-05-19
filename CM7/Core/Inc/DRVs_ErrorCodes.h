#ifndef DRVs_ErrorCodes_H
#define DRVs_ErrorCodes_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Common Error codes */
#define DRV_ERR_NONE                    0
#define DRV_ERROR_NO_INIT                -1
#define DRV_ERROR_WRONG_PARAM            -2
#define DRV_ERROR_BUSY                   -3
#define DRV_ERROR_PERIPH_FAILURE         -4
#define DRV_ERROR_COMPONENT_FAILURE      -5
#define DRV_ERROR_UNKNOWN_FAILURE        -6
#define DRV_ERROR_UNKNOWN_COMPONENT      -7
#define DRV_ERROR_BUS_FAILURE            -8
#define DRV_ERROR_CLOCK_FAILURE          -9
#define DRV_ERROR_MSP_FAILURE            -10
#define DRV_ERROR_FEATURE_NOT_SUPPORTED  -11

#ifdef __cplusplus
}
#endif

#endif /* STM32H747I_DISCO_ERRNO_H */
