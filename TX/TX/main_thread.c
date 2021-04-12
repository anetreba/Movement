#include <stdlib.h>
#include <unistd.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/PIN.h>
#include <ti_radio_config.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/rf/RF.h>
#include "ti_drivers_config.h"
#include <ti/sail/bme280/bme280.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/display/Display.h>
#include "bmi160_support.h"

#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

#define PAYLOAD_LENGTH 12

static RF_Object rfObject;
static RF_Handle rfHandle;

static uint8_t packet[PAYLOAD_LENGTH];
static uint16_t seqNumber;

I2C_Handle      i2c;
I2C_Params      i2cParams;

Display_Handle display;

void *mainThread(void *arg0) {

    I2C_init();
    GPIO_init();
    GPIO_write(GREEN_LED, CONFIG_GPIO_LED_OFF);

    /* Initialize I2C connection */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(CONFIG_I2C_BME, &i2cParams);
    if (i2c == NULL) {
        while(1);
    }

    RF_Params rfParams;
    RF_Params_init(&rfParams);

    RF_cmdPropTx.pktLen = PAYLOAD_LENGTH;
    RF_cmdPropTx.pPkt = packet;
    RF_cmdPropTx.startTrigger.triggerType = TRIG_NOW;

    /* Request access to the radio */
#if defined(DeviceFamily_CC26X0R2)
    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioSetup, &rfParams);
#else
    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);
#endif// DeviceFamily_CC26X0R2

    /* Set the frequency */
    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Open the HOST display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
       while (1);
        }
    Display_print0(display, 0, 0, "Starting the i2cbmi160 sensor example...\n\n");

    if (i2c == NULL) {
        Display_print0(display, 0, 0, "Error Initializing I2C\n");
    }
    else {
        Display_print0(display, 0, 0, "I2C Initialized!\n");
    }

    bmi160_initialize_sensor(i2c);

    char serial_num[] = "L410094Z";

    while(1) {
//            if (angle != -1) {
                /* Create packet with incrementing sequence number and values of humidity and temperature */
                 packet[0] = (uint8_t)(seqNumber >> 8);
                 packet[1] = (uint8_t)(seqNumber++);

                 uint8_t i;

                 for (i = 0; i < 8; i++) {
                     packet[i + 2] = serial_num[i];
                 }
                 packet[10] = angle;

//                 Display_print0(display, 0, 0, "\n\n*****************BLA BLA BLA****************8\n\n");


                 /* Send packet */
                 RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropTx, RF_PriorityNormal, NULL, 0);

                 GPIO_toggle(GREEN_LED);
                 RF_yield(rfHandle);
                 Display_print0(display, 0, 0, "\n\n*****************BLA BLA BLA****************8\n\n");
//            }
            sleep(1);
//            usleep(50000)

    }
}
