#include "algorithm.h"

#include <stdint.h>
#include <avr/eeprom.h>
#include <math.h>
#include "mathUtils.h"

//Nota: Cargamos y guardamos la clase Vector entera, que es igual que
//cargar/guardar sus tres atributos (x,y,z) de manera independiente.

using namespace Algoritm_Const;

Algorithm::Algorithm(){
	//Cargamos en memoria el vector M y su módulo
	eeprom_read_block(&M, M_dir, sizeof(M));
	eeprom_read_block(&MiNorm, MiNorm_dir, sizeof(MiNorm));
}

void Algorithm::calibration(Accelerometer &acc, Magnetometer &mag){
	//Calculamos la media de varias muestras del vector de acceleración y
	//del vector magnético
	Vector<int> tmpA(0,0,0);
	Vector<float> tmpM(0,0,0);
	Vector<float> a(0,0,0);
	Vector<float> m(0,0,0);

	//Descartamos las primeras muestras (sulen ser erroneas)
	for(int i = numSamples ; i > 0 ; --i, acc.getData(tmpA), mag.getData(tmpM));

	//Calculamos la media de unas cuantas muestras
	for(int i = numSamples ; i > 0 ; --i){
		while(!acc.getData(tmpA)); //Esperamos a tener un dato
		//Actualizamos la media
		a.x += tmpA.x*iNumSamples;
		a.y += tmpA.y*iNumSamples;
		a.z += tmpA.z*iNumSamples;

		while(!mag.getData(tmpM)); //Esperamos a tener un dato
		//Actualizamos la media
		m.x += tmpM.x*iNumSamples;
		m.y += tmpM.y*iNumSamples;
		m.z += tmpM.z*iNumSamples;
	}

	float T = a.z*a.iNorm()*0.5;	//eq X.16a
	float S = sqrt(0.5 - T);	//eq X16b
	Vector<float> r(-a.y, a.x, 0);
	r.normalize();

	//Creamos el cuaternión para la rotación
	Quaternion ESq_rot(sqrt(0.5 + T), r.x*S, r.y*S, 0);	//eq X.16
	
	//Rotamos el vector m para obtener M
	M = ESq_rot.rotateVector(m);	//eq X.11
	MiNorm = M.iNorm();

	//Guardamos M y su módulo en la eeprom
	eeprom_update_block(&M, (uint8_t*)M_dir, sizeof(M));
	eeprom_update_block(&MiNorm, (uint8_t*)MiNorm_dir, sizeof(MiNorm));
}

void Algorithm::gyroscope(const Vector<float> &gyr, float deltaT){
	SEq_G = SEq_G + (SEq_G*gyr)*deltaT*0.5;	//eq X.17 y X.18
}

void Algorithm::magnetometer(const Vector<float> &m){
	//Calculamos el vector r (eq. X.23)
	Vector<float> r(
			m.y*M.z - m.z*M.y,
			m.z*M.x - m.x*M.z,
			m.x*M.y - m.y*M.x
		       );

	//Calculamos el cuaternión ESq_M (eq. X.24)
	float miNorm = m.iNorm();
	float T;
	if(MiNorm < 0.01 || miNorm < 0.01)
		T = 0;
	else
		T = (M*m)*(MiNorm*miNorm*0.5);

	float S = sqrt(0.5 - T);
	
	ESq_M.q0 = sqrt(0.5 + T);
	ESq_M.q1 = -r.x*S;
	ESq_M.q2 = -r.y*S;
	ESq_M.q3 = -r.z*S;

	ESq_M.normalize();
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

