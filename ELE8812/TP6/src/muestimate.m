function mu=muestimate(gamma,N)
%function [mu, gamma, de0]=muestimate(gamma,mu,N)
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%
%  function  [mu, gamma, de0]=muestimate(gamma,mu,N)
%
%  estimation of mu parameter of telegraphic markov chain
%  [Idier, Goussard] 
%   
%  Input:
%	gamma = Idier-Goussard Notation
%	N = number of possible states
%       
%        
%  Output:
%  	mu
%
%
%  Authors:
%  Jerome Idier (GPI-LSS)
%  modified by Andrea Ridolfi (GPI-LSS) 
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if N>2
   NU = 100;
   onu = ones(NU,1); 

   [gammamax imax] = max(gamma);

   numin = 4*gammamax;    
   som0 = sum(sqrt(1-gamma/gammamax))-(N-2);
     
   if (som0 <= 0)	fprintf('CAS I  \n')
        numax = N*N/(N-1)*gammamax;
        
     %   snu = linspace(1/numin,1/numax,NU)';

        snu = powspace(1/numin,1/numax,2,NU)';
        de = sqrt(1-4*snu*gamma');                  
        som = sum(de')';
%        opt = table1([som de snu],N-2);
        opt = interp1(som, [de snu],N-2);
        de0 = opt(1:N)';
        %nu0 = 1/opt(N+1);
        mu = .5 - de0/2;   
    else		fprintf('CAS II \n')
        sgamma = sum(gamma)-gammamax;
        numax = numin/(1-(gammamax/sgamma)^2);
        
     %   numin = numin/(1-som0^2);
     %   snu = linspace(1/numin,1/numax,NU)';
      if N==0 %N=3 a revoir ...
      gamma
        delta=gamma./gammamax;
        delta=sort(delta);
        ddelta=(delta(1)-delta(2))^2;
        sdelta=delta(1)+delta(2);
        a(1) = ddelta-2*sdelta-1;
        a(2) = -4*(1-sdelta);
   	a(3) = 1-ddelta;
	a(4) = a(2);
	a(5) = ddelta+2*sdelta-3;
	r = roots(a);
	r = r(~imag(r))';
	nu0 = 4*gamma(1)./(1-r.^2);
	nu0 = nu0(nu0>=numin)';
	nu0 = nu0(nu0<=numax)';
	nu0=min(nu0);
	
	de0 = sqrt(1-4*nu0*gamma'); 
        mu = 2/nu0*gamma./(1+de0);		% Pour imax, c'est 1-mu
	mu(imax) = 1-mu(imax);
	
%sumu = sqrt(1-4*gamma(1)./nu0)-sqrt(1-4*gamma(2)./nu0)-sqrt(1-4*gamma(3)./nu0);
%sumu = (3+sumu)/2;
      
      
      else
%        snu = linspace(1/numin,1/numax,NU)';
         snu = powspace(1/numin,1/numax,2,NU)';
       de = sqrt(1-4*snu*gamma');
        gamma(imax) = -gamma(imax);
        som = sum(gamma(:,onu)./(1+de'))'.*snu;	% .*snu, qu'on oublie...
        [somax jmax] = max(som);
     %   plot(som(1:jmax)); %???
%        opt = table1([som(1:jmax) de(1:jmax,:) snu(1:jmax)],0); %???  
        opt = interp1(som(1:jmax), [de(1:jmax,:) snu(1:jmax)],0); %???  
        de0 = opt(1:N)'; 
        nu0 = 1/opt(N+1);
        gamma(imax) = -gamma(imax);
	%mu0 = mu;
        mu = 2/nu0*gamma./(1+de0);		% Pour imax, c'est 1-mu
	mu(imax) = 1-mu(imax);
        %de0(imax) = -de0(imax);
      end;
   end
else
   mu=[.5;.5];
   %de0=[0;0];
end
     
%Pe = ((1-de0)./eta)*(gamma./(1+de0))';    
return
