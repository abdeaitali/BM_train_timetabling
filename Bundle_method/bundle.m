function [ mu_new, lambda_new, diff] = bundle(mu, Phi, g)
%bundle computes the new dual iterate using aggregate bundle quadratic method

% Global variables from the main program
global ids
global requests
global network
global ordering

% Parameters
m = 0.7; % for taking serious steps
u = 1; % step-size control

% Get some useful data
B = size(mu,1);
T = size(mu,2);
k = size(mu,3);
mu_current = mu(:,:,end);

% Define the quadratic problem
ij = (2:B*T+1)'; v = u*ones(B*T,1);
H = sparse(ij,ij,v);
f = [1; -u*mu_current(:)]; sparse(f);
A = [-ones(k,1), zeros(k,B*T)]; %% add mu >= 0
for j=1:k 
    temp = g(:,:,j);
    A(j,2:end) = temp(:)';
end
A = sparse(A);
b = zeros(k,1);
for j=1:k 
    temp = sum(sum(g(:,:,j).*mu(:,:,j)));
    b(j) = temp - Phi(j) - 0.5*u*norm(mu_current,2);
end
sparse(b);

% Solve the quadratic problem (Sol = [y|mu_new]')
[xval, fval, ~, ~, lambda_struct] = quadprog(H,f,A,b);

% Get the mu and the multipliers
mu_computed = xval(2:end);
lambda_new = lambda_struct.ineqlin;

% get the achieved and the expected descent
achieved = Phi(k) - (fval - 0.5*u*norm(mu_computed-reshape(mu_current,size(mu_computed)),2));
[Phi_expected, ~] = MexShortestPathSeq(ids, requests, network, ordering, reshape(mu_computed,[B T]));
expected = Phi(k) - Phi_expected;
ratio = achieved/expected;
% Check if we take a serious step
if ratio >= m
    mu_new = reshape(mu_computed,[B,T]);  
else
    mu_new = mu_current;
end
diff = expected - achieved;
