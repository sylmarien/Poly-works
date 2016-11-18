function [MAPimage,MaxPostDist]=imageND(y,pe,Pe,mu,lambda,m,sigma2,N,I,J,K);

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%
%  function [MAPimage,MaxPostDist]=imageND(y,pe,Pe,mu,lambda,m,sigma2,N,I,J,K);
%   
%  Input:
%	y = observed data
%	pe, Pe = Markov chain initial and transition probabilities 
%	mu, lambda = Matkov chain telegraphic parameters
%       m, sigma2 = Gaussian distribution mean value and variance
%	N = segmentation levels
%	I,J,K = dimension indexes
%               
%  Output:
%  	maxPpost = maximum a posteriori distribution  
%	ximage = segmented image
%       
%  Author: Jerome Idier           Date: 11/95
%  Modified by: Andrea Ridolfi 	  Date: 01/99
%  (idier,ridolfi)@lss.supelec.fr
%  GPI-Laboratoire des Signaux et Systemes
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 y = y(:)';		% Organisation en (1,T)
 T = length(y);

 if exist('I')~=1 I = T; end
 if exist('J')~=1 J = T/I; end
 if exist('K')~=1 K = T/I/J; end

% Dim = 3;		% Nb de dimensions du probleme
% if K==1 Dim = 2; end
% if J==1 Dim = 1; end
%D = max([I,J,K]); 	 

 
%fe  = zeros(N,D);      % 
%Fwd = zeros(N,D);       	% forward matrix                   
%Bwd = zeros(N,D);              % backward matrix 
%NNFwd = zeros(N,1);    % non normalized forward matrix                            
%M = zeros(1,D); 
 
%xstate=y;
%maxPpost=y;
%MPost=ones(1,N*I*J*K); 
            
[MAPlabels,MaxPostDist]=MAPND(y,pe,Pe,mu,lambda,m,sigma2,I,J,K,N);
 			
MAPimage = reshape(m(MAPlabels),I,J,K);
MaxPostDist = reshape(MaxPostDist,I,J,K);


