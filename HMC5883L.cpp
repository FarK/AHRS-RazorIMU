/* ******************************************************* */
/* I2C code for HMC5583 magnetometer                       */
/*                                                         */
/* ******************************************************* */

#include "HMC58X3.h"
#include "HMC5883L.h"

//I2C addresses 
#define COMPASS_ADDRESS 0x1E   //Write:0x3C  Read:0x3D

HMC58X3 magn;

//============================================
// Magnetormeter
//============================================
void Init_Compass() {
  // no delay needed as we have already a delay(5) in HMC5843::init()
  magn.init(false); // Dont set mode yet, we'll do that later on.
  // Calibrate HMC using self test, not recommended to change the gain after calibration.
  magn.calibrate(0); // Use gain 1=default, valid 0-7, 7 not recommended.
  // Single mode conversion was used in calibration, now set continuous mode
  magn.setMode(0);
}

void Read_Compass(s_sensor_data* sen_data, s_sensor_offsets* sen_offset) {
          float magRot_x;
          float magRot_y;
          float magRot_z;          
          
	  //TODO: Esto parece que esta mal: getValues llama a getRaw (Mirar HMC58X3.cpp)
          magn.getRaw(&sen_data->mxr, &sen_data->myr, &sen_data->mzr);
          //magn.getValues(&sen_data->mx, &sen_data->my, &sen_data->mz);

          //===============================================
          // Apply Hard iron compensation 
          // This removes effects of permanetly magnetic material
          // that is on the sensor platform
          // (offset X,Y,and Z of raw data)
          sen_data->mx = (float) sen_data->mxr - sen_offset->magnetom_offset[0];
          sen_data->my = (float) sen_data->myr - sen_offset->magnetom_offset[1];
          sen_data->mz = (float) sen_data->mzr - sen_offset->magnetom_offset[2];
}
