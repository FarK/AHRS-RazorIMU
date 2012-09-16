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
	dSEq_G = (SEq*gyr)*0.5f;
	dSEq_G.normalize();
}

void Algorithm::magnetometer(const Vector<float> &m, float deltaT){
	//Variables auxiliares para no repetir cuentas
	float Mxq0 = M.x*SEq.q0;
	float Mxq1 = M.x*SEq.q1;
	float Mxq2 = M.x*SEq.q2;
	float Mxq3 = M.x*SEq.q3;
	                   
	float Myq0 = M.y*SEq.q0;
	float Myq1 = M.y*SEq.q1;
	float Myq2 = M.y*SEq.q2;
	float Myq3 = M.y*SEq.q3;
                           
	float Mzq0 = M.z*SEq.q0;
	float Mzq1 = M.z*SEq.q1;
	float Mzq2 = M.z*SEq.q2;
	float Mzq3 = M.z*SEq.q3;

	Ms = SEq.rotateVector(M);

	Vector<float> f = Ms - m;

	dSEq_M = Quaternion(
		(Myq3-Mzq2)*f.x			+ (Mzq1-Mxq3)*f.y		+ (Mxq2-Myq1)*f.z,
       		(Myq2+Mzq3)*f.x			+ (Mxq2-2.0f*Myq1+Mzq0)*f.y	+ (Mxq3-Myq0-2.0f*Mzq1)*f.z,
		(Myq1-2.0f*Mxq2-Mzq0)*f.x	+ (Mxq1+Mzq3)*f.y		+ (Mxq0+Myq3-2.0f*Mzq2)*f.z,
		(Myq0-2.0f*Mxq3+Mzq1)*f.x	+ (Mzq2-Mxq0-2.0f*Myq3)*f.y	+ (Mxq1+Myq2)*f.z
	);
	dSEq_M = dSEq_M*2.0f;
	
	dSEq_M.normalize();
}

void Algorithm::fusion(float deltaT){
	SEq = SEq + (dSEq_G - dSEq_M*B)*deltaT;
	SEq.normalize();
}

void Algorithm::correction(){
	SEq = SEq*SScq_C;
}

void Algorithm::oarOrientationCorrection(){
	//Calculamos z_x y z_z (eq. X.25)
	float z_x = 2*(SEq.q1*SEq.q3 - SEq.q0*SEq.q2);
	float z_z = 1 - 2*(SEq.q1*SEq.q1 - SEq.q2*SEq.q2);

	//Calculamos T_a y T_b
	float iR = invSqrt(z_x*z_x + z_z*z_z);
	float T_a = -z_x*iR*0.5;	//eq. X.29
	float T_b = -z_z*iR*0.5;	//eq. X.30

	//Comprobamos de qué plano estamos más cerca para definir una rotación
	//u otra (eq. X.44)
	if(T_a >= T_b){
		SScq_C.q0 = sqrt(0.5 + T_a);
		SScq_C.q1 = 0;
		SScq_C.q2 = sqrt(0.5 - T_a);
		SScq_C.q3 = 0;
	}
	else{
		SScq_C.q0 = sqrt(0.5 + T_b);
		SScq_C.q1 = 0;
		SScq_C.q2 = sqrt(0.5 - T_b);
		SScq_C.q3 = 0;
	}
}

