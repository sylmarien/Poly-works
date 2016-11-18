/*  Function FwdBwdND
  
 Author: Jerome Idier           Date: 11/95
 Modified by: Andrea Ridolfi 	Date: 01/99
 (idier,ridolfi)@lss.supelec.fr
 GPI-Laboratoire des Signaux et Systemes
 
 Calling sintax:
 --------------
 FwdBwND(y,pe,Pe,mu,lambda,mold,sigma2old,alphaigamma,betaigamma,I,J,K,N, 
 	      fe,Fwd,Bwd,NNFwd,M,MPost,esse,alpha,epsi,m,sigma2,NLL,Qxy,count);
 
 Description: 
 It is called at each EM iteration in order to re-estimate 
 the parameters of the N-Dimensional Hidden Markov Model,  
 based on a Pickard Markov Random field, 
 by mean of the Forward-Backward algorithm
  
 Input variables (which are not changed)
 - y : original image
 - pe, Pe : Markov chain parameters
 - mu, lambda : Telegraphic parameters
 - sigma2old, muold : previously estimated Gaussian ditribution 
                      variance and mean value. 
                      They are used to compute the new values of the 
                      gaussian distribution.
 - alphaigamma, betaigamma : a priori ditribution (inverted gamma) parameters
 - I,J,K : dimension parameters
 - N : number of segmentation levels
 
 Global variables (they are used and updated -only- by the mex-function.
                   They behave as a "stay in memory variables" and
                   are not used in the calling matlab function)
 - fe : Gaussian distribution
 - Fwd, Bwd : Forward and Backward variable
 - NNFwd, M : Forward-Backward normalizing factors
 - MPost : a posteriori ditribution
 
 Output variables
 - esse, alpha, epsi : basic quantities for updating the estimates of the 
                       Telegraphic parameters mu and lambda
 - m, sigma2 : updated Gaussian distribution variance and mean value
 - NLL : updated negative-log-likelihood
 - Qxy : ?
 - count : ?
 			
 */
         
#include <math.h>
#include "mex.h"

#define PI		3.14159265358979
#define pow2(a)		((a)*(a))
#define	y(a,b,c)	y[(a)+(b)*I+(c)*J*I]
#define	Y(a,b,c)	y[(a)*I3+(b)*J3+(c)*K3]
#define	Pe(a,b)	        Pe[(a)+(b)*N]
#define	fe(a,b)	        fe[(a)+(b)*N]
#define	Fwd(a,b)	Fwd[(a)+(b)*N]
#define	Bwd(a,b)	Bwd[(a)+(b)*N]
#define	MPost(a,b)	MPost[(a)+(b)*N]

/* --------  GATEWAY  ---------- */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

 /* --- VARIABLES DECLARATION -------- */
 
 double *y,*pe,*Pe,*mu,*lambda;
 double *mold,*sigma2old,*alphaigamma,*betaigamma;
 int I,J,K,N;
 double *fe,*Fwd,*Bwd,*NNFwd,*M,*MPost;
 double *esse,*alpha,*epsi,*m,*sigma2,*NLL,*Qxy,*count;
 double sumNNFwd,temp;
 int I1[3],J1[3],K1[3],I2[3],J2[3],K2[3],I3,J3,K3;
 int i,j,k,d,ImageDimensions,n,t,T;
 
 /* --- CHECK CORRECT NUMBER OF ARGUMENTS -------- */
  if (nrhs!=27) 
       {mexErrMsgTxt("27 inputs required !!");}
      
 /* --- PROCEDURE INPUT PARAMETERS --- */

 y     = mxGetPr(prhs[0]);		
 pe    = mxGetPr(prhs[1]);
 Pe    = mxGetPr(prhs[2]);
 mu    = mxGetPr(prhs[3]);
 lambda= mxGetPr(prhs[4]);
 mold  = mxGetPr(prhs[5]);
 sigma2old = mxGetPr(prhs[6]);
 alphaigamma = mxGetPr(prhs[7]);
 betaigamma = mxGetPr(prhs[8]);
 I = (int) mxGetScalar(prhs[9]);
 J = (int) mxGetScalar(prhs[10]);
 K = (int) mxGetScalar(prhs[11]);
 N = (int) mxGetScalar(prhs[12]);

 /* --- PROCEDURE GLOBAL PARAMETERS --- */

 fe    = mxGetPr(prhs[13]);		
 Fwd   = mxGetPr(prhs[14]);
 Bwd   = mxGetPr(prhs[15]);
 NNFwd = mxGetPr(prhs[16]);
 M     = mxGetPr(prhs[17]);
 MPost = mxGetPr(prhs[18]);

 /* --- PROCEDURE OUTPUT PARAMETERS --- */

 esse  = mxGetPr(prhs[19]);
 alpha = mxGetPr(prhs[20]);	
 epsi  = mxGetPr(prhs[21]);
 m     = mxGetPr(prhs[22]);
 sigma2= mxGetPr(prhs[23]);
 NLL   = mxGetPr(prhs[24]);
 Qxy   = mxGetPr(prhs[25]);
 count = mxGetPr(prhs[26]);
 
 /* --- SCANNING VARIABLES ---------- 

   - d = 0 : i,j,k (column scanning)
   - d = 1 : j,i,k (line scanning)
   - d = 2 : k,i,j (vertical line scanning) */

 I1[0] = J1[1] = J1[2] = 1;	/* Scanning Increments  */
 I1[1] = J1[0] = K1[2] = I;
 I1[2] = K1[0] = K1[1] = I*J;

 I2[0] = J2[1] = J2[2] = I;	/* Scanning Dimensions  */
 I2[1] = J2[0] = K2[2] = J;
 I2[2] = K2[0] = K2[1] = K;

 ImageDimensions = 3;
 if(K==1)			/* Dimensions Variable */
   ImageDimensions = 2;
 if(J==1)
   ImageDimensions = 1;

 /* ------- PARAMETERS INIT ---------- */

 *NLL = 0;
 *Qxy = 0;
 *count=0;
 
 for(n=0; n<N;esse[n] = epsi[n] = alpha[n] = m[n] = sigma2[n] = 0, n++);   
  
 /* ---------- MAIN LOOP  ------------ */
 printf(" FwdBwdND mex file ... \n");
 
 for(d=0; d<ImageDimensions; d++)   /* 3D for LOOP */
  {
   printf("  dimension # %d\n",d+1);			
   I3 = I1[d];
   J3 = J1[d];
   K3 = K1[d];
   T  = I2[d];	      /* length of unidimensional vectors */

   for(k=0; k<K2[d]; k++)
    for(j=0; j<J2[d]; j++)
     {

      /* -------- GAUSSIAN DISTRIBUTION ------------- */
    
      for(i=0; i<T; i++)
       for(n=0; n<N; n++)     
        fe(n,i) = exp(-pow2(Y(i,j,k)-mold[n])/sigma2old[n]/2)/sqrt(2*PI*sigma2old[n]);         
      
      /* -------- FORWARD & BACKWARD INIT (t=0, T=T-1)------------- */
    
      for(n=0,sumNNFwd=0; n<N; n++)
       {
        NNFwd[n] = pe[n]*fe(n,0);
        sumNNFwd += NNFwd[n];
       }
      
      M[0] = 1/sumNNFwd;
      *NLL += log(M[0]);    
      for(n=0; n<N; n++)
       {     
        Fwd(n,0) = M[0]*NNFwd[n]; 
        Bwd(n,T-1) = 1;     
       }

      /* ------- FORWARD ----------------- */
      /* Fwd(n,t)=
       ((sum(Fwd(i,t+1)*(lambda[i]))*mu[n]+Fwd[n]*Pe(n,n))*fe(n,t+1))*M[t+1] */
 
      for(t=0; t<T-1; t++)
       {    
        for(n=0,sumNNFwd=0; n<N; n++)
         {      
          for(i=0,temp=0; i<n; i++)
           temp += (1-lambda[i])*Fwd(i,t);     
          for(i=n+1; i<N; i++)
           temp += (1-lambda[i])*Fwd(i,t);         
          NNFwd[n] = (temp*mu[n]+Fwd(n,t)*Pe(n,n))*fe(n,t+1);
          sumNNFwd += NNFwd[n];
         }    
        M[t+1] = 1/sumNNFwd;  
        *NLL += log(M[t+1]);
        for(n=0; n<N; n++)
         Fwd(n,t+1) = M[t+1]*NNFwd[n];
       }
   
    
      /* ------- 1D BACKWARD ----------------- */
      /* Bwd(n,t)=
       (sum(Bwd(i,t+1)*mu[i]*fe(i,t+1))*(lambda[n])+
        Bwd(n,t+1)*fe(n,t+1)*Pe(n,n))*M[t+1]             */
   
      for(t=T-2; t>=0; t--)
       {      
        for(n=0; n<N; n++)
         {
          for(i=0,temp=0; i<n; i++)
           temp+=Bwd(i,t+1)*mu[i]*fe(i,t+1);
          for(i=n+1; i<N; i++)
           temp+=Bwd(i,t+1)*mu[i]*fe(i,t+1);
          Bwd(n,t)=(temp*(1-lambda[n])+Bwd(n,t+1)*fe(n,t+1)*Pe(n,n))*M[t+1];
         }				
       }                                   
  
      /* ------- 1D MARGINAL A POSTERIORI ------- */
    
      for(n=0; n<N; n++)
       {
        for(t=0; t<T; t++)
         {
          *count=*count+1;
          MPost(n,t) = Fwd(n,t)*Bwd(n,t);
          alpha[n] += MPost(n,t);			/* 3D summation */
          m[n] += MPost(n,t)*Y(t,j,k);			/* 3D summation */
          sigma2[n] += MPost(n,t)*pow2(Y(t,j,k));	/* 3D summation */
         }
        epsi[n] += MPost(n,0)+MPost(n,T-1);		/* 3D summation */
        for(t=0, temp=0; t<T-1; t++)
         temp += Fwd(n,t)*Bwd(n,t+1)*fe(n,t+1)*M[t+1];
        temp *= Pe(n,n);
        esse[n] += temp; 					/* 3D summation */     
       }      
     }	/* j and k for loop */

  }	/* d for loop */

 for(n=0, temp=0; n<N; n++)
  { 
   /*      
      The estimation formula for sigma is normally given by  
      sigma2 = ( alphaigamma + 0.5 ( sum(MPost Y^2)-m^2 sum(MPost) ) )/( betaigamma + 0.5 sum(MPost) ) 
      where sum(MPost)=alpha. 
      Considering that the actual value of sigma2 is sum(MPost Y^2), It follows
      sigma2 = ( alphaigamma + 0.5 ( sigma2-m ) )/( beta + 0.5 alpha ).   */
    
  m[n] /= alpha[n];
  
  temp = sigma2[n];
  		
  sigma2[n] = (*alphaigamma+0.5*(sigma2[n]-pow2(m[n])*alpha[n]))/(*betaigamma+0.5*alpha[n]);
  
  *Qxy += -alpha[n]*log(sigma2[n])+(pow2(m[n])*alpha[n]-temp)/sigma2[n];  
  
  epsi[n] /= 2; 
  
  }
  
 *Qxy /= 2;
 printf("  ... done!\n");

}	/* END OF FwdBwd3D */

































