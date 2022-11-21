
// Sistema
A=[-2 1; 1 -1];B=[1; 0]; C=[0 1]
sl=syslin('c',A,B,C,[0]);
// Muestreo
T = 50/1000;

// Comportamiento deseado
//Pc=[-1+%i*1*sqrt(3);-1-%i*1*sqrt(3)];
Pc =[-2; -2];
Po=[-10; -10];
// Polos deseados discretos
Zc=exp(T*Pc);
Zo=exp(T*Po);

// Sistema discreto
sld=cls2dls(sl,T);
Ad = sld(2); Bd=sld(3); Cd=C; Dd=0;

// Solucion a problemas de asignacion de polos
K = ppol(Ad,Bd,Zc)                            // Ganancia control
H = ppol(Ad',Cd',Zo)'                          // Ganancia observador

// Calculando el precompensador
Kp = -1/(C*inv(Ad-Bd*K-eye(2,2))*Bd)        // Ganancia precompensador
