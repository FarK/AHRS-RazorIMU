#include "Algorithm.h"

#include <stdint.h>
#include <avr/eeprom.h>
#include <math.h>
#include "mathUtils.h"

//Nota: Cargamos y guardamos la clase Vector entera, que es igual que
//cargar/guardar sus tres atributos (x,y,z) de manera independiente.

using namespace Algoritm_Const;

Algorithm::Algorithm(){
	//Cargamos en memoria el vector M
	eeprom_read_block(&M, M_dir, sizeof(M));
}

void Algorithm::calibration(Accelerometer &acc, const Magnetometer &mag){
	//Calculamos la media de varias muestras del vector de acceleración y
	//del vector magnético
	Vector<int> tmp;
	Vector<float> a;
	Vector<float> m;
	for(int i = numSamples ; i > 0 ; --i){
		while(!acc.getData(tmp)); //Esperamos a tener un dato
		a = a + tmp/numSamples; //Actualizamos la media

		while(!acc.getData(tmp)); //Esperamos a tener un dato
		m = m + tmp/numSamples; //Actualizamos la media
	}

	float T = a.z*a.iNorm()*0.5;	//eq X.16a
	float S = sqrt(0.5 - T);	//eq X16b

	//Creamos el cuaternión para la rotación
	Quaternion ESq_rot(sqrt(0.5 + T), a.y*S, -a.x*S, 0);	//eq X.16
	ESq_rot.normalize();
	
	//Rotamos el vector m para obtener M
	ESq_rot.conjugate();	//Ahora ESq_rot = SEq_rot
	M = ESq_rot.rotateVector(m);	//eq X.11

	//Guardamos M en la eeprom
	eeprom_update_block(&M, (uint8_t*)M_dir, sizeof(M));
}

void Algorithm::gyroscope(const Vector<float> &gyr, uint8_t deltaT){
	SEq_G = SEq_G + (SEq_G*gyr)*deltaT*0.5;	//eq X.17 y X.18
}

void Algorithm::magnetometer(const Vector<int> &m){
	//Calculamos el vector r (eq. X.23)
	Vector<float> r(
			m.y*M.z - m.z*M.y,
			m.z*M.x - m.x*M.z,
			m.x*M.y - m.y*M.x
		       );

	//Calculamos el cuaternión ESq_M (eq. X.24)
	float T = (M*m)*(M.iNorm()*m.iNorm()*0.5);
	float S = sqrt(0.5 - T);
	
	ESq_M.q0 = sqrt(0.5 + T);
	ESq_M.q1 = -r.x*S;
	ESq_M.q2 = -r.y*S;
	ESq_M.q3 = -r.z*S;
}

void Algorithm::oarOrientationCorrection(){
	//Calculamos z_x y z_z (eq. X.25)
	float z_x = 2*(ESq.q1*ESq.q3 - ESq.q0*ESq.q2);
	float z_z = 1 - 2*(ESq.q1*ESq.q1 - ESq.q2*ESq.q2);

	//Calculamos T_a y T_b
	float iR = invSqrt(z_x*z_x + z_z*z_z);
	float T_a = -z_x*iR*0.5;	//eq. X.29
	float T_b = -z_z*iR*0.5;	//eq. X.30

	//Comprobamos de qué plano estamos más cerca para definir una rotación
	//u otra (eq. X.44)
	if(T_a >= T_b){
		SScq_C.q0 = sqrt(0.5 + T_a);
		SScq_C.q1 = 0;
		SScq_C.q2 = -sqrt(0.5 - T_a);
		SScq_C.q3 = 0;
	}
	else{
		SScq_C.q0 = sqrt(0.5 + T_b);
		SScq_C.q1 = 0;
		SScq_C.q2 = -sqrt(0.5 - T_b);
		SScq_C.q3 = 0;
	}
}

