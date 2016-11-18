
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%
%  Testing procedure for 2D segmentation of  ./data/femur;
%
%
%  Author:
%  Andrea Ridolfi (GPI-LSS)
%  Modifié par Yves Goussard pour l'article --> MOORE
% 
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

% $$$ clear;
%path(path,'/disk1/people/idier/seismic/matlab'); 
%close all;

% $$$ load ./data/femur.mat
% $$$ %y=y(:,:,14:14);
% $$$ y=y(:,:,1:35);
% $$$ %y(y<-1)=-1;


[I J K]=size(y);
% $$$ N=4; 

%~~~~ hyperparameters of sigma a priori distribution  ~~~~~~~~~~~~~~

if penal           % Si on veut une pénalisation
  alphaigamma=1;
%  alphaigamma=25;
  betaigamma=1.01;
else               % Si on n'en veut pas
  alphaigamma=0;
  betaigamma=0;
end

%~~~~ control variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 max_iter = 1000;	% max number of iterations (inf if not controlled) 
 thr_iter1 = 1e-10;     % precision threshold between successive iterations (parameters) 
 thr_iter2 = 1e-10;	% precision threshold between successive iterations (criteria)
 thr_iter3 = 1e-10;	% precision threshold between successive iterations (gradient norms)
% thr_iter1 = 1e-8;thr_iter2 = 1e-8;thr_iter3 = 1e-8;
%~~~~ parameters estimation via EM algorithm ~~~~~~~~~~~~~~~~~~~~~~~

[pe,Pe,mu,lambda,m,sigma2,NLLvect,GLxvect,mvect,sigma2vect,lambdavect,muvect]= ...
tmc_emND(y,alphaigamma,betaigamma,N,max_iter,thr_iter1,thr_iter2,thr_iter3,I,J,K);


%~~~~ MAP estimate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[MAPimage,MaxPostDist]=imageND(y,pe,Pe,mu,lambda,m,sigma2,N,I,J,K);

y=reshape(y,I,J,K);

% $$$ figure; plot(GLxvect); title('GLxvect');
% $$$ figure; plot(NLLvect); title('NLLvect');
% $$$ niter = size(mvect, 2)-1;figure
% $$$ subplot(2,2,1), plot(0:niter, mvect), title('Moyenne \it{m}')
% $$$ subplot(2,2,2), plot(0:niter, sigma2vect), title('Variance \sigma^2')
% $$$ subplot(2,2,3), plot(0:niter, lambdavect), title('Paramètre \lambda')
% $$$ subplot(2,2,4), plot(0:niter, muvect), title('Paramètre \mu')


%figure; imagesc(y(:,:,1)); xlabel('columns'); ylabel('rows') 
%colormap(gray);
%figure; imagesc(MAPimage(:,:,1)); xlabel('columns'); ylabel('rows')
%colormap(gray);
%figure; imagesc(MaxPostDist(:,:,1)); xlabel('columns'); ylabel('rows')
%colormap(gray);

%figure; imagesc(y(:,:,2)); xlabel('columns'); ylabel('rows') 
%colormap(gray);
%figure; imagesc(MAPimage(:,:,2)); xlabel('columns'); ylabel('rows')
%colormap(gray);
%figure; imagesc(MaxPostDist(:,:,2)); xlabel('columns'); ylabel('rows')
%colormap(gray);

%figure; imagesc(y(:,:,3)); xlabel('columns'); ylabel('rows') 
%colormap(gray);
%figure; imagesc(MAPimage(:,:,3)); xlabel('columns'); ylabel('rows')
%colormap(gray);
%figure; imagesc(MaxPostDist(:,:,3)); xlabel('columns'); ylabel('rows')
%colormap(gray);

%figure; imagesc(y(:,:,4)); xlabel('columns'); ylabel('rows') 
%colormap(gray);
%figure; imagesc(MAPimage(:,:,4)); xlabel('columns'); ylabel('rows')
%colormap(gray);
%figure; imagesc(MaxPostDist(:,:,4)); xlabel('columns'); ylabel('rows')
%colormap(gray);

% $$$ save ./data/femur_SEG.mat;



