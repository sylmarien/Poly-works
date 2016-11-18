function  [pe,Pe,mu,lambda,m,sigma2,NLLvect,GLxvect,mvect,sigma2vect,lambdavect,muvect]= ...
  tmc_emND(y,alphaigamma,betaigamma,N,max_iter,thr_iter1,thr_iter2,thr_iter3,I,J,K)

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%
%  function [pe,Pe,mu,lambda,m,sigma2,NLLvect,GLxvect,mvect,sigma2vect,lambdavect,muvect]
%   tmc_emND(y,alphaigamma,betaigamma,N,max_iter,thr_iter1,thr_iter2,thr_iter3,I,J,K)
%  
%  Description:
%  It estimates the parameters of the N-Dimensional Hidden Markov Model,  
%  based on a Pickard Markov Random field, 
%  by mean of the EM algorithm
% 
%  Input:
%	y = observed data
%       alphaigamma, betaigamma = parameters of the a priori distributon 
%                                 (inverted gamma)
%	N = segmentation levels
%       max_iter = max number of iterations (inf if not controlled) 
%       thr_iter1 = precision threshold between successive iterations (parameters) 
%       thr_iter2 = precision threshold between successive iterations (criteria)
%       thr_iter3 = precision threshold between successive iterations (gradient norms)
%	I,J,K = dimension indexes
%               
%  Output:
%  	pe, Pe = Markov chain initial and transition probabilities 
%	mu, lambda = Markov chain telegraphic parameters
%       m, sigma2 = Gaussian distribution mean value and variance
%	NLLvect = Negative Log-Likihood values at each iteration
%       GLxvect = Likelihood gradient values at each iteration
%
%       
%  Author: Jerome Idier           Date: 11/95
%  Modified by: Andrea Ridolfi 	  Date: 01/99
%  (idier,ridolfi)@lss.supelec.fr
%  GPI-Laboratoire des Signaux et Systemes
%
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% CONSTANTS
 
 beta1=0.1;
 beta2=0.1;
 beta3=0.1;
 
 y = y(:)';		           % y as a (1,T) vector 
 T = length(y);

 if exist('I')~=1 I = T; end
 if exist('J')~=1 J = T/I; end
 if exist('K')~=1 K = T/I/J; end
 
 ImageDimensions = 3;		           % problem dimensions
 if K==1 ImageDimensions = 2; end
 if J==1 ImageDimensions = 1; end
 
 MaxChainLength = max([I,J,K]); 	% 1D vector max dimension
 
 onesN1 = ones(N,1);
 
 fprintf('---------------------------------------------\n')      
 fprintf('I J K MaxChainLength = %g %g %g %g\n',I,J,K,MaxChainLength);
 fprintf('ImageDimensions = %g \n',ImageDimensions);
 fprintf('max_iter        = %g \n',max_iter);
 fprintf('thr_iter1       = %g \n',thr_iter1);
 fprintf('thr_iter2       = %g \n',thr_iter2);
 fprintf('thr_iter3       = %g \n',thr_iter3);
 
 
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% VARIABLES INIT
 
 fe  = zeros(N,MaxChainLength); 	% gaussian distribution 
 Fwd = zeros(N,MaxChainLength); 	% forward matrix                   
 Bwd = zeros(N,MaxChainLength); 	% backward matrix 
 NNFwd = zeros(N,1);			% non normalized forward matrix                            
 M = zeros(1,MaxChainLength); 		% normalizing factor
              
 MPost = zeros(N,MaxChainLength);	% marginal a posteriori
  
 NLL = -inf; 		% neg-log-likelihood
 dNLLr = 1;		% relative increment NLL btw successive iterations 
 
 Rx = -inf;		% R function
 dRx = 1;		% increment btw successive iterations
 dRxr = 1;		% relative increment btw successive iterations
 
 Qyx = -inf;		% Qyx function
 dQyx =  1;		% increment btw successive iterations
 dQyxr = 1;		% relative increment btw successive iterations
 
 Qx = -inf;		% Qx function
 dQx =  1;		% increment btw successive iterations
 dQxr= 1;		% relative increment btw successive iterations
 
 dQr = 1;		% relative increment of Q btw successive iterations
 dQxRxr =1;		% relative error btw Qx and Rx
 
 GLxnorm=inf;		% norm of likelihood gradient w/r to TetaX parameters
 dGLxnormr=1;		% relative increment of GLxnorm btw successive iterations
 %GQxnorm=inf;
  
 dmr = 1; 		% relative increment of |m| btw successive iterations  
 dsigma2r=1;		% relative increment of |sigma2| btw successive iterations	
 dlambdar=1;		% relative increment of |lambda| btw successive iterations
 dmur=1;		% relative increment of |mu| btw successive iterations
       
 iter = 0;		% iteration counter
 
 Qyxvect=[];		% successive estimate vectors
 Qxvect=[];		%
 Rxvect=[];		%
 dQxRxvect=[];		%
 
 GLxvect=[];		%
 GLxnormvect=[];	%
 GRxvect=[];		%
 GRxnormvect=[];	%
 GQxvect=[];		%
 GLxvect=[];		%
 NLLvect=[];		%
 GLxmuparvect=[];	%
 GLxmuparnormvect=[];	%
 
 GLxlambdanormvect=[];	%
 
 mvect=[];		%
 sigma2vect=[];		%
 
 muvect=[];		%
 lambdavect=[]; 	%

 count=0;		% number of iteration performed by for loops in FwdBwdND

  
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% PARAMETERS INIT (Devijver) 
%

 %~~~ MARKOV CHAIN PARAMETERS (pe, Pe or lambda,mu or esse,eta,mu) ~~~~~~~~
 %
 
  mu = ones(N,1)/N;	      % uniform distribution
 
  pe=ones(N,1)./N;
  eta=(T-1).*pe;
  esse=0.5.*eta;
  lambda = (esse./eta -mu)./(1-mu);
  gamma=ones(N,1);
  alpha=ones(N,1);
  epsi=ones(N,1);
  Pe = (1+(N-2)*eye(N))/(2.*(N-1)); %pnn=0.5 and pni=pnj 

 
 
 %~~~ GAUSSIAN DISTRIBUTION PARAMETERS (sigma2, m) ~~~~~~~~~~~~~~~~~~~~~~~~~
 %  
                       
  [temp m] = hist(y,N);       % initial states 
  m = m';	        
  m = mean(m) + (N-0.0)/N*(m-mean(m));	% Manip pour tasser les niveaux
  
  sigma2 = mean(min((y(onesN1,:) - m(:,ones(1,T))).^2)); 	
  sigma2 = sigma2(onesN1);						
  if(T == 1)							
    sigma2 = y(1);
  end
  
 %~~~ Stockage des valeurs initiales               ~~~~~~~~~~~~~~~~~~~~~~~~~
 %  
 m_ini = m;
 sigma2_ini = sigma2;
 mu_ini = mu;
 lambda_ini = lambda;
  
 

%~~~~ ITERATION LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%

while ( ( (dmr>thr_iter1)|(dsigma2r>thr_iter1)| ...
          (dlambdar>thr_iter1)|(dmur>thr_iter1)| ...
          (dQr>thr_iter2)|(dNLLr>thr_iter2)|(dGLxnormr>thr_iter3)) ...
        & (iter<max_iter) & (~isnan(dQr)) )

  iter = iter+1;   fprintf('   \n');  fprintf('iter # %g\n',iter);
   
 %~~~~ 3D PARAMETERS ESTIMATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 %  
 %     MPost = Fwd.*Bwd = P(X/Y)
 %
  
  %~~~ PREVIOUS VALUES
    
  muold=mu;
  lambdaold=lambda;
  Peold=Pe;
  peold=pe;
  
  Qxold=Qx;
  Rxold=Rx;
  GLxnormold=GLxnorm;
  %GQxnormold=GQxnorm;
        
  esseold=esse;
  etaold=eta; 
  mold=m;
  sigma2old=sigma2;
  NLLold=NLL; 					% Negative Log Likelihood
  Qyxold=Qyx;
 
  %~~~~ MEX FUNCTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  
  %y=y+0; pe=pe+0; Pe=Pe+0; mu=mu+0; lambda=lambda+0; mold=muold+0;
  %sigma2old=sigma2old+0; alphaigamma=alphaigamma+0; betaigamma=betaigamma+0;
  %I=I+0; J=J+0; K=K+0; N=N+0;
  %fe=fe+0; Fwd=Fwd+0; Bwd=Bwd+0; NNFwd=NNFwd+0; M=M+0; MPost+0; 
  
  esse=esse+0; alpha=alpha+0; epsi=epsi+0; m=m+0; sigma2=sigma2+0; NLL=NLL+0; Qyx=Qyx+0; count=count+0;
   
  FwdBwdND(y,pe,Pe,mu,lambda,mold,sigma2old,alphaigamma,betaigamma,I,J,K,N, ...
                fe,Fwd,Bwd,NNFwd,M,MPost,esse,alpha,epsi,m,sigma2,NLL,Qyx,count);  
                            
  mvect(:,iter)=m;
  sigma2vect(:,iter)=sigma2;
  NLLvect(iter)=NLL;
  Qyxvect(iter)=Qyx;  
            
  %fprintf('count                   = %g\n',count);  
  
  %~~~~ ETA and GAMMA ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  eta=alpha-epsi;
  gamma = eta - esse+beta1+beta2+beta3;		
 			
  %~~~~ O  PARAMETERS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   
  %      X     
  %
  %    Q  approximated form Q  ~ R
  %	X		     X 
  %
  %    O    = mu, lamda (= mu, diagPe) 
  %     X
                  
     %~~~~ MU ESTIMATE ~~~~~~~~~~~~~~~  
         
     mu=muestimate(gamma,N);             
     muvect(:,iter)=mu;
          
     %~~~~ LAMBDA ESTIMATE ~~~~~~~~~~~~~~~ 
     
     lambda = ((esse+beta2)./(eta+beta1+beta2)-mu)./(1-mu);          
     lambdavect(:,iter)=lambda; 
     
     %~~~~ Pe AND pe ESTIMATES ~~~~~~~~~~~~~~~  
                     
     Pe = ((1-esse./eta)./(1-mu))*mu';          
     Pe(1:N+1:N*N) = esse./eta;		% Diag(Pe) = ...
     
     pe = mu'*(1./(1-lambda));
     pe = (1/pe).*mu./(1-lambda);   %pe = eta/(T-1);
        
  %~~~~ Q & Rx FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
  % 
  % Rx = sum((eta-esse).*log(mu.*(1-lambda)) + esse.*log(esse./eta));
  %
   
     %~~~~ Rx FUNCTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
    
     Rx = (eta-esse)'*log(mu.*(1-lambda)) + esse'*log(esse./eta);
     Rxvect(iter)=Rx;
   
     %~~~~ Qx-Rx ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   
     %   
     % dQxRx=epsi'*log(mu./(1-lambda))-log(sum(mu./(1-lambda)))
     %     having sum(epsi)=1 and pe=(mu./(1-lambda))/sum(mu./(1-lambda)
     %     leads to
     % dQxRx = epsi'*log(pe)
     %
  
     dQxRx = epsi'*log(pe);  
     dQxRxvect(iter)=dQxRx;
   
     %~~~~ Qx FUNCTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~      
  
     Qx=Rx+dQxRx; 
     Qxvect(iter)=Qx;
     
     %~~~~ Qx GRADIENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~      
  
     GQxmu=(alpha-esse)./muold+esse.*etaold.*(1-lambdaold)./esseold-peold./muold;
     GQxlambda=-(2.*eta-alpha-esse)./(1-lambdaold)+esse.*etaold.*(1-muold)./esseold-peold./(1-lambdaold);
     GQxvect(iter,:)=[GQxmu' GQxlambda'];
     GQxnorm=norm(GQxvect(iter,:));
     GQxnormvect(iter)=GQxnorm;
        
     %~~~~ Rx GRADIENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~      
     GRxmu=(eta-esse)./muold+esse.*etaold.*(1-lambdaold)./esseold;
     GRxlambda=-(eta-esse)./(1-lambdaold)+esse.*etaold.*(1-muold)./esseold;
     GRxvect(iter,:)=[GRxmu' GRxlambda'];
     GRxnormvect(iter)=norm(GRxvect(iter,:));  
     
     %~~~~ Lx GRADIENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
     
     GLxmu=(alpha-esse)./muold+esse.*etaold.*(1-lambdaold)./esseold-peold./muold;
     GLxlambda=-(alpha-2*epsi-esse)./(1-lambdaold)+esse.*etaold.*(1-muold)./esseold-peold./(1-lambdaold);
          
     GLxmuparvect(iter,:)=(GLxmu(2:N)-GLxmu(1))';
     GLxmuparnormvect(iter)=norm(GLxmu(2:N)-GLxmu(1));
     GLxvect(iter,:)=[GLxmuparvect(iter,:) GLxlambda'];
     
     GLxlambdanormvect(iter)=norm(GLxlambda);
     GLxnorm=norm(GLxvect(iter,:));
     GLxnormvect(iter)=GLxnorm;
     
     
  
  %~~~~ TEST VARIABLES ON NLL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~      		
  DNLLr =    (NLL-NLLold)/abs(NLL);
  dNLLr = abs(NLL-NLLold)/abs(NLL);      
  
  %~~~~ TEST VARIABLE ON PARAMETERS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 	
  dmr = norm(mold-m)/norm(m);   
  dsigma2r = norm(sigma2old-sigma2)/norm(sigma2); 
  dmur = norm(muold-mu)/norm(mu);
  dlambdar = norm(lambdaold-lambda)/norm(lambda);
  
  %~~~~ TEST VARIABLE ON Pe ~~~~~~~~~~~~~~~
  %dPer = norm(Peold-Pe)/norm(Pe);        
  
  %~~~~ TEST VARIABLES ON GQx ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
  %dGQxnormr=abs(GQxnorm-GQxnormold)./abs(GQxnorm);
     
  %~~~~ TEST VARIABLES ON GLx ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
  dGLxnormr=abs(GLxnorm-GLxnormold)./abs(GLxnorm);
    
  %~~~~ TEST VARIABLES ON Rx ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
  dRx = Rx - Rxold;
  dRxr = abs(dRx)/abs(Rx);   
     
  %~~~~ TEST VARIABLES ON Qx & Qyx ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
  dQyx = Qyx - Qyxold;
  dQyxr=abs(dQyx)/abs(Qyx);
  dQx = Qx - Qxold;
  dQxr=abs(dQx)/abs(Qx); 
  dQr=abs(dQx+dQyx)/abs(Qx+Qyx);                        
  
  fprintf('---------------------------------------------\n')      
  fprintf('dmr                    = %g\n',dmr);
  fprintf('dsigma2r               = %g\n',dsigma2r);
  fprintf('dmur                   = %g\n',dmur);
  fprintf('dlambdar               = %g\n',dlambdar);      
  fprintf('min value sigma2       = %g\n',min(sigma2));             
  fprintf('---------------------------------------------\n');
  fprintf('NLL                    = %g\n',NLL);
  fprintf('dNLLr                  = %g\n',dNLLr);
%  fprintf('---------------------------------------------\n');
%  fprintf('Qyx                    = %g\n',Qyx);
%  fprintf('dQyx                   = %g\n',dQyx);
%  fprintf('dQyxr                  = %g\n',dQyxr);
%  fprintf('---------------------------------------------\n');
%  fprintf('Qx                     = %g\n',Qx);
%  fprintf('dQx                    = %g\n',dQx);
%  fprintf('dQxr                   = %g\n',dQxr);
  fprintf('---------------------------------------------\n');
  fprintf('dQr                    = %g\n',dQr);
%  fprintf('---------------------------------------------\n');
%  fprintf('Rx                     = %g\n',Rx); 
%  fprintf('dRx                    = %g\n',dRx);
%  fprintf('dRxr                   = %g\n',dRxr);
%  fprintf('---------------------------------------------\n');
%  fprintf('dQxRx                  = %g\n',dQxRx);
  fprintf('---------------------------------------------\n');
  fprintf('GLxnorm                = %g\n',GLxnorm); 
  fprintf('dGLxnormr              = %g\n',dGLxnormr); 
end %(while)

%~~~~ END OF ITERATION LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

%clear fe Fwd Bwd NNFwd M MPost;
%save ./data/TETA.mat;

% Ajout des valeurs initiales aux vexteurs mvect, sigma2vect, muvect et
% lambdavect

mvect = [m_ini, mvect];
sigma2vect = [sigma2_ini, sigma2vect];
lambdavect = [lambda_ini, lambdavect];
muvect = [mu_ini, muvect];

return;
