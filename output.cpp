#include "output.h"
#include <WProgram.h>
#include "timing.h"
#include "AHRS.h"

#define STX 0x63
#define DLE 0x64

void printdata(s_sensor_data* sen_data, euler_angles* eAngles)
{    
#if SEND_TRAMA == 1
	//Montamos y enviamos la trama
	//frame f = {0x64646464, 0x64646464, 0x64646464, 0x64646464};	//Trama para testear la máquina de estados
	//frame f = {timer, eAngles->roll, eAngles->pitch, eAngles->yaw};
	frame f = {timer,
	       eAngles->roll, eAngles->pitch, eAngles->yaw,
	       sen_data->ax, sen_data->ay, sen_data->az,
	       sen_data->gx, sen_data->gy, sen_data->gz,
	       sen_data->mx, sen_data->my, sen_data->mz,
	};
	Serial.send(f.buff, sizeof(frame));
#endif

#if PRINT_EULER == 1
	Serial.print("\nEuler: (");
	Serial.print(ToDeg(eAngles->roll));
	Serial.print(", ");
	Serial.print(ToDeg(eAngles->pitch));
	Serial.print(", ");
	Serial.print(ToDeg(eAngles->yaw));
	Serial.print(")");
#endif   

#if PRINT_QUATERNION == 1
	Serial.println("");
	Serial.print("Quaternion: (");
	Serial.print(q0, 10);
	Serial.print(", ");
	Serial.print(q1, 10);
	Serial.print(", ");
	Serial.print(q2, 10);
	Serial.print(", ");
	Serial.print(q3, 10);
	Serial.println(")");
	Serial.print("Q4:");
	Serial.println(q4, 10);
#endif

#if PRINT_SENSOR_DATA==1
	Serial.print("   ACCE: ");
	Serial.print(sen_data->ax);
	Serial.print(", ");
	Serial.print(sen_data->ay);
	Serial.print(", ");
	Serial.print(sen_data->az);
	Serial.print("   GYRO: ");
	Serial.print(sen_data->gx);
	Serial.print(", ");
	Serial.print(sen_data->gy);
	Serial.print(", ");
	Serial.print(sen_data->gz);  
	Serial.print("   MAGN: ");
	Serial.print(sen_data->mx);
	Serial.print(", ");
	Serial.print(sen_data->my);
	Serial.print(", ");
	Serial.println(sen_data->mz);    
	/*Serial.print ("; ");
	Serial.print(sen_data->magnetom_heading );     
	*/
#endif


#if PRINT_SENSOR_DATA_RAW==1
	Serial.print("RAW: ");
	Serial.print(sen_data->axr);
	Serial.print (", ");
	Serial.print(sen_data->ayr);
	Serial.print (", ");
	Serial.print(sen_data->azr);
	Serial.print(", ");
	Serial.print(sen_data->gxr);
	Serial.print(", ");
	Serial.print(sen_data->gyr);
	Serial.print(", ");
	Serial.print(sen_data->gzr);  
	Serial.print(", ");
	Serial.print((int)sen_data->mxr);
	Serial.print (", ");
	Serial.print((int)sen_data->myr);
	Serial.print (", ");
	Serial.print((int)sen_data->mzr);    
#endif

/*

#if PRINT_DCM == 1
	Serial.print (",DCM:");
	Serial.print(convert_to_dec(DCM_Matrix[0][0]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[0][1]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[0][2]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[1][0]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[1][1]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[1][2]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[2][0]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[2][1]));
	Serial.print (",");
	Serial.print(convert_to_dec(DCM_Matrix[2][2]));
#endif
*/
}

long convert_to_dec(float x)
{
	return x*10000000;
}
