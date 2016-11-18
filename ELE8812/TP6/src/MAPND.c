/*  Function MAPND     
 
  Author: Jerome Idier           Date: 11/95
  Modified by: Andrea Ridolfi 	 Date: 01/99
  (idier,ridolfi)@lss.supelec.fr
  GPI-Laboratoire des Signaux et Systemes
 
 Calling sintax:
 --------------
 [MaxPostDist,MAPlabels]=MAPND(y,pe,Pe,mu,lambda,m,sigma2,I,J,K,N)
 
 Description:
 It computes the segmented image by maximization of the maginal a posteriori
 distribution P(x/y,theta) 	
 
 Input variables (which are not changed)
 - y : original image
 - pe, Pe : Markov chain parameters
 - mu, lambda : Telegraphic parameters
 - sigma2, m : estimates of the variance and mean value of the 
               Gaussian ditribution 
 - I,J,K : dimension parameters
 - N : number of segmentation levels
 
 Output variables
 - MAPlabels : segmentation labels (MAP result)			
 - MaxPostDist : max values of the a posteriori distribution			
 			
 */
         
#include <math.h>
#include "mex.h"

#define PI		3.14159265358979
#define pow2(a)		((a)*(a))
#define	PostDist1(a,b,c,d)	PostDist[(a)*I3+(b)*J3+(c)*K3+(d)*J*I*K]
#define	PostDist2(a,b,c,d)	PostDist[(a)+(b)*I+(c)*J*I+(d)*J*I*K]
#define	MAPlabels(a,b,c)	MAPlabels[(a)+(b)*I+(c)*J*I]
#define	MaxPostDist(a,b,c)	MaxPostDist[(a)+(b)*I+(c)*J*I]
#define	Y(a,b,c)	y[(a)*I3+(b)*J3+(c)*K3]
#define	Pe(a,b)	        Pe[(a)+(b)*N]
#define	fe(a,b)	        fe[(a)+(b)*N]
#define	Fwd(a,b)	Fwd[(a)+(b)*N]
#define	Bwd(a,b)	Bwd[(a)+(b)*N]


/* --------  GATEWAY  ---------- */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

 /* --- INPUT VARIABLES DECLARATION -------- */
 
 double *y,*pe,*Pe,*mu,*lambda,*m,*sigma2;
 int I,J,K,N;
 
 /* --- OUTPUT VARIABLES DECLARATION -------- */
 
 double *MAPlabels, *MaxPostDist;
 
 /* --- LOCAL VARIABLES DECLARATION -------- */
 
 double *fe,*Fwd,*Bwd,*NNFwd,*M,*PostDist;
 double sumNNFwd,temp;
        /* fe : Gaussian distribution
           Fwd,Bwd : Forward and Backward variables
           NNFwd, M,sumNNFwd : Forward and Backward normalizing factors
           PostDist : a posteriori distribution  */
 int I1[3],J1[3],K1[3],I2[3],J2[3],K2[3],I3,J3,K3;
 int i,j,k,d,ImageDimensions,n,t,T,MaxChainLength;
 int label;
 
 /* --- CHECK CORRECT NUMBER OF ARGUMENTS -------- */
  
  if (nrhs!=11) 
       {mexErrMsgTxt("11 inputs required !!");}                  
  if (nlhs!=2) {mexErrMsgTxt("2 outputs required !!");} 
  
 /* --- POINTER ASSIGNEMENT FOR INPUT VARIABLES --- */

 y     = mxGetPr(prhs[0]);		
 pe    = mxGetPr(prhs[1]);
 Pe    = mxGetPr(prhs[2]);
 mu    = mxGetPr(prhs[3]);
 lambda= mxGetPr(prhs[4]);
 m     = mxGetPr(prhs[5]);
 sigma2= mxGetPr(prhs[6]);
 I = (int) mxGetScalar(prhs[7]);
 J = (int) mxGetScalar(prhs[8]);
 K = (int) mxGetScalar(prhs[9]);
 N = (int) mxGetScalar(prhs[10]);
 
 /* --- DINAMIC MEMORY ALLOCATION FOR OUTPUT VARIABLES --- */
 
 plhs[0]=mxCreateDoubleMatrix(1,I*J*K,mxREAL);
 plhs[1]=mxCreateDoubleMatrix(1,I*J*K,mxREAL);
 
 /* --- POINTER ASSIGNEMENT FOR OUTPUT VARIABLES --- */
 
		
 MAPlabels = mxGetPr(plhs[0]); 
 MaxPostDist = mxGetPr(plhs[1]);

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

 ImageDimensions = 3;	                        /* Dimensions Variable */
 if(K==1)			
   ImageDimensions = 2;
 if(J==1)
   ImageDimensions = 1;  
    
 if(I>K){ if(I>J) MaxChainLength=I;
          else MaxChainLength=J; }
 else   { if(K>J) MaxChainLength=K;
          else MaxChainLength=J; }
 
 /* printf(" MaxChainLength %d\n",MaxChainLength); */
 

   
 /* --- DINAMIC MEMORY ALLOCATION FOR LOCAL ARRAYS --- */
 
 fe=(double *)mxCalloc(N*MaxChainLength, sizeof(double));
 Fwd=(double *)mxCalloc(N*MaxChainLength, sizeof(double));
 Bwd=(double *)mxCalloc(N*MaxChainLength, sizeof(double));
 NNFwd=(double *)mxCalloc(N, sizeof(double));
 M=(double *)mxCalloc(MaxChainLength, sizeof(double));
 PostDist=(double *)mxCalloc(N*I*J*K, sizeof(double));  
   
 /* --- LOCAL ARRAY INITIALIZATION --- */
 
 for(i=0;i<N*MaxChainLength;fe[i]=Fwd[i]=Bwd[i]=0, i++); 
 for(i=0;i<N;NNFwd[i]=0,i++); 
 for(i=0;i<MaxChainLength;M[i]=0,i++); 
 for(i=0;i<N*I*J*K;PostDist[i]=1,i++); 

 /* ------- MARGINAL A POSTERIORI COMPUTATION  ------ */
 printf(" MAPND mex file \n");
 printf("  marginal a posteriori distribution\n");   
 for(d=0; d<ImageDimensions; d++)	/* 3D for LOOP */
  {			
   printf("   dimension # %d\n",d+1);

   I3 = I1[d];
   J3 = J1[d];
   K3 = K1[d];
   T  = I2[d];
 		   
   for(k=0; k<K2[d]; k++)
    for(j=0; j<J2[d]; j++)
     {      

      /* -------- GAUSSIAN DISTRIBUTION --------- */
      
      for(i=0; i<T; i++)
       for(n=0; n<N; n++)     
        fe(n,i) = exp(-pow2(Y(i,j,k)-m[n])/sigma2[n]/2)/sqrt(2*PI*sigma2[n]);               
      
      /* ---- FORWARD & BACKWARD INIT (t=0, T=T-1) ---- */ 
           
      for(n=0,sumNNFwd=0; n<N; n++)
       {
        NNFwd[n] = pe[n]*fe(n,0);
        sumNNFwd += NNFwd[n];
       }
      M[0] = 1/sumNNFwd;   
      for(n=0; n<N; n++)
       { 
        Fwd(n,0) = M[0]*NNFwd[n]; 
        Bwd(n,T-1) = 1;     
       } 
       
      /* ------- FORWARD VARIABLE -------- */
      
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
        for(n=0; n<N; n++)
         Fwd(n,t+1) = M[t+1]*NNFwd[n];
       }
   
      /* ------- BACKWARD VARIABLE ------- */
      
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
  
      /* ---- MARGINAL A POSTERIORI ---- */         
      
      for(i=0; i<T; i++)
       for(n=0; n<N; n++) {              
        PostDist1(i,j,k,n)*= Fwd(n,i)*Bwd(n,i);  
        }
                        
     }	/* j & k for LOOPS */

  }	/* d for LOOP */
  
 printf("   ... done!\n");  
 /* ------- MAP ESTIMATE -------------------- */

 printf("  maximum values of the marginal a posteriori distribution\n");   
 for(k=0; k<K; k++)
  for(j=0; j<J; j++)	  
   for(i=0; i<I; i++)
    {  
     for(n=0,temp=0; n<N; n++)
     if(temp <= PostDist2(i,j,k,n))
       {
        temp = PostDist2(i,j,k,n); 
        label  = n+1; 
       }
      MaxPostDist(i,j,k) = temp;
      MAPlabels(i,j,k) = label;
     }
 printf("  ... done!\n");   
 /* --- DINAMIC MEMORY DEALLOCATION */
  
/* mxFree(fe);
 mxFree(Fwd);
 mxFree(Bwd);
 mxFree(NNFwd);
 mxFree(M);
 mxFree(PostDist); */
 
}	/* END OF MAPND */


































